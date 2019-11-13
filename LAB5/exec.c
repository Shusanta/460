
int exec(char *cmdline) // cmdline=VA in Uspace
{
  int i, upa, usp; 
  char *cp, kline[128], file[32], filename[32]; 
  PROC *p = running;
 
  printf("EXEC: proc %d cmdline=%x\n", running->pid, cmdline);

  kstrcpy(kline, cmdline);
  printf("EXEC: proc %d kline = %s\n", running->pid, kline); 

  // first token of kline = filename
  cp = kline; i=0;
  while(*cp != ' ' && *cp != '\n'){
    filename[i] = *cp;
    i++; cp++;
  } 
  filename[i] = 0;

  upa = p->pgdir[2049] & 0xFFFF0000; // PA of Umode image
  kprintf("load file %s to %x\n", filename, upa);

  // load filename to Umode image 
  if (load(filename, p) <= 0){
    printf("loading failed\n");
    return -1;
  }
  //  printf("after loading ");

  // copy kline to high end of Ustack in Umode image
  usp = upa + 0x100000 - 128;
  printf("usp=%x ", usp);
  kstrcpy((char *)usp, kline); // TODO: why do we copy command to top of stack?
  p->usp = (int *)VA(0x100000 - 128);
  printf("p->usp = %x ", p->usp);

  // set up syscall kstack frame to return to new image at VA=0
  //   usp             
  // --|-----goUmode--------------------------------
  //  r0 r1 r2 r3 r4 r5 r6 r7 r8 r9 r10 r11 r12 ULR|
  //-------------------------------------------------
  //  14 13 12 11 10 9  8  7  6  5  4   3    2   1
  for (i=2; i<14; i++)  // clear Umode regs r1-r12
      p->kstack[SSIZE - i ] = 0;
  p->kstack[SSIZE-14] = (int)VA(p->usp);
  p->kstack[SSIZE-1]  = (int)VA(0);

  kprintf("kexec exit\n");
  return (int)p->usp; // will replace saved r0 in kstack
}