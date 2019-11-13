
#include "uart.c"

#define NPIPE 10
#define PSIZE 16

typedef struct pipe {
    char buf[PSIZE];
    int head,tail;
    int data,room;
    int status;
}PIPE;

PIPE pipe;
PIPE *kpipe;


int read_pipe(PIPE *p, char *buf, int n) {
  int r = 0;
  if (n <= 0)
    return 0;
  // validate PIPE pointer p; // p->status must not be FREE
  while (n) {
    while (p->data) {
      *buf++ = p->buf[p->tail++]; // read a byte to buf
      p->tail %= PSIZE;
      p->data--;
      p->room++;
      r++;
      n--;
      if (n == 0)
        break;
    }
    kwakeup(&p->room); // wakeup writers
    if (r)             // if has read some data
      return r;
    // pipe has no data
    ksleep(&p->data);
    // sleep for data
  }
}

int write_pipe(PIPE *p, char *buf, int n) {
  int r = 0;
  if (n <= 0)
    return 0;
  while (n) {
    printf("writer %d writing pipe\n", running->pid);
    while (p->room) {
      p->buf[p->head++] = *buf++; // write a byte to pipe;
      p->head %= PSIZE;
      p->data++;
      p->room--;
      r++;
      n--;
      if (n == 0)
        break;
    }
    kwakeup(&p->data);
    ksleep(&p->room);
    if(n == 0){
      return r;
    }
  }
}


int pipe_writer() // pipe writer task code
{
  struct uart *up = &uart[0];
  color = YELLOW;
  char line[128];
  while (1) {
    uprintf("Enter a line for task1 to get : ");
    printf("task%d waits for line from UART0\n", running->pid);
    ugets(up, line);
    uprints(up, "\r\n");
    printf("task%d writes line=[%s] to pipe\n", running->pid, line);
    write_pipe(kpipe, line, strlen(line));
  }
}

int pipe_reader() // pipe reader task code
{
  char line[128];
  int i, n;
  while (1) {
    printf("task%d reading from pipe\n", running->pid);
    n = read_pipe(kpipe, line, 20);
    printf("task%d read n=%d bytes from pipe : [", running->pid, n);
    for (i = 0; i < n; i++)
      kputc(line[i]);
    printf("]\n");
  }
}