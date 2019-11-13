int umenu()
{
  uprintf("-------------------------------\n");
  uprintf("getpid getppid ps chname switch fork exit wait\n");
  uprintf("-------------------------------\n");
}

int getpid()
{
  int pid;
  pid = syscall(0,0,0,0);
  return pid;
}    

int ufork(){
  char temp[256];
  uprintf("\nplease enter a file name, u1 or u2\n");
  ugetline(temp);
  int pid;
  int childp;
  pid = getpid();

  childp = syscall(5,temp,0,0);

  printf(" syscall fork :Process %d forked a child = %d\n",pid,childp);
  return childp;
}

int uexit(){
  int exitCode;

  exitCode = geti();

  printf("\nsyscall exit: exit with code %d\n",exitCode);

  syscall(6,exitCode,0,0);

  return exitCode;

}

int uwait(){
  int waiting;
  uprintf("input status \n");
  waiting = geti();

  return syscall(7,&waiting,0,0);
  
}

int getppid()
{ 
  return syscall(1,0,0,0);
}

int ugetpid()
{
  int pid = getpid();
  uprintf("pid = %d\n", pid);
}

int ugetppid()
{
  int ppid = getppid();
  uprintf("ppid = %d\n", ppid);
}

int ups()
{
  return syscall(2,0,0,0);
}

int uchname()
{
  char s[32];
  uprintf("input a name string : ");
  ugetline(s);
  printf("\n");
  return syscall(3,s,0,0);
}

int uswitch()
{
  return syscall(4,0,0,0);
}

int ugetc()
{
  return syscall(90,0,0,0);
}

int uputc(char c)
{
  return syscall(91,c,0,0);
}

int getPA()
{
  return syscall(92,0,0,0);
}

