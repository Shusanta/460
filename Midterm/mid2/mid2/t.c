
int color;

#include "type.h"
#include "string.c"
// #include "queue.c"  // use provided queue.obj  during linking
// #include "kbd.c"    // use provided kbd.obj    during linking
#include "vid.c"
#include "exceptions.c"
#include "kernel.c"
#include "pipe.c"
#include "wait.c"

void copy_vectors(void) {
    extern u32 vectors_start;
    extern u32 vectors_end;
    u32 *vectors_src = &vectors_start;
    u32 *vectors_dst = (u32 *)0;

    while(vectors_src < &vectors_end)
       *vectors_dst++ = *vectors_src++;
}
int kprintf(char *fmt, ...);

void IRQ_handler()
{
    int vicstatus, sicstatus;
    int ustatus, kstatus;

    // read VIC status register to find out which interrupt
    vicstatus = VIC_STATUS; // VIC_STATUS=0x10140000=status reg
    sicstatus = SIC_STATUS;  
    if (vicstatus & 0x80000000){
       if (sicstatus & 0x08){
          kbd_handler();
       }
    }
}

int body();

int main() {
  fbuf_init();
  kprintf("Welcome to Wanix in ARM\n");
  uart_init();
  kbd_init();
  int i;
  kpipe = &pipe;
  kpipe->head = kpipe->tail = 0;
  kpipe->data = 0; kpipe->room = PSIZE;
  // initialize PIPEs
  
  init();                // kernel init
  kprintf("P0 kfork tasks\n");
  kfork((int)pipe_writer, 1); // pipe writer process
  kfork((int)pipe_reader, 1); // pipe reader process
  printf("forked pipes\n");
  while (1) {
      tswitch();
  }
}


