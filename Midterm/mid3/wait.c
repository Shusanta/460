void send_child(){
  PROC *orphan = running->child;
  if (orphan){
    PROC *orphanage = &proc[1];
    

    while(orphan){
      orphan->ppid = 1;
      orphan->parent = &proc[1];
      orphan = orphan->sibling;
    }

    orphanage = orphanage->child;

    while(orphanage->sibling){
      orphanage = orphanage->sibling;
    }

    orphanage->sibling = running->child;

  }
}

int ksleep(int event)
{
  int sr = int_off();
  printf("proc %d going to sleep on event = %x\n", running->pid);
  running->event = event;
  running->status = SLEEP;
  enqueue(&sleepList,running);
  tswitch();
  int_on(sr);
}

int kwakeup(int event)
{
  int sr = int_off();
  PROC *p;
  PROC *temp = 0;

  while(p = dequeue(&sleepList)){
    if( p->event ==event){
      p->status = READY;
      enqueue(&readyQueue,p);
    }
    else{
      enqueue(&temp,p);
    }
  }
  sleepList = temp;
  int_on(sr);

}



      
int kexit(int exitCode)
{
  int i;
  PROC *p;

  if(running->pid == 1) {
    printf("CANT EXIT 1 \n");
    return -1;
  }

  //2.dispose of children process, if any
  
  send_child(running);  // give children to P1
  //3. record exit values
  running->exitCode = exitCode;
  //4. make into a zombie
  running->status = ZOMBIE;
 
  kwakeup((int)running->parent);

  //6.switch process to give to cpu
  tswitch();

  
}

int kwait(int *status)
{
  int i;
  PROC *p;
  if(!running->child){
    return -1;
  }
  p = running->child;
  while (1){
    while(p){
      if(p->status == ZOMBIE){
        *status = p->exitCode;
        delete_child(p);
        p->status = FREE;
        enqueue(&freeList,p);
        return p->pid;
      }

      p= p->sibling;
    }
  }
  ksleep((int)(running));

}


