// uart.c file
#include <stdarg.h>
#include <stdint.h>

#define UDR   0x00
#define UFR   0x18

#define RXFE 0x10
#define TXFF 0x20
typedef unsigned int u32;

char *ctable = "0123456789ABCDEF";

typedef struct uart{
  char *base;
  int n;
}UART;

UART uart[4];

int uart_init()
{
  // ARM versatilepb's 4 UARTs are at the base addresses
  //     0x101F1000, 0x101F2000, 0x101F3000, 0x10009000
  // Write code to initialize the 4 UART structs.

  // page 31 chapter 2 data registers diagram and addresses

   int i; UART *up;

    for (i=0; i<4; i++) // note uart0 and uart2 are adjacent
    {
        up = &uart[i];
        up->base = (char *)(0x101F1000 + i*0x1000);
        up->n = i;
    }
    uart[3].base = (char *) (0x10009000); // uart 3 at 0x10009000
}

int ugetc(UART *up)
{
  // up points at a UART struct;
  // Write code to return an input char from the UART

  while (*(up->base + UFR) & RXFE); // Loop if UFR is REFE
  return *(up->base + UDR); // write char in UDR
}

int uputc(UART *up, char c)
{
  // up points at a UART struct;
  // Write code to output a char to the UART


  while (*(up->base+UFR) & TXFF); // Loop if UFR is TXFF
  *(up->base+UDR) = c; //
}

int ugets(UART *up, char s[ ])
{
  // Write code to input a string of chars from a UART
  while ((*s = (char)ugetc(up)) != '\r')
  {
    uputc(up, *s);
    s++;
  }
  *s = 0;

}

int uprints(UART *up, char *s)
{
  // Write code to print a string s to UART
  while(*s)
  uputc(up, *s++);
}

int rpu(UART *up, u32 x, int BASE)
{  
    char c;
    if (x){
       c = ctable[x % 10];
       rpu(up, x / 10, BASE);
       // putchar(c);
       uputc(up, c);
    }
}

int uprintu(UART *up, u32 x)
{
   (x==0)? uputc(up, '0') : rpu(up, x, 10);
   //putchar(' ');
   uputc(up, ' ');

}

int uprintd(UART *up, int x)
{
  if (x < 0) 
  {
    x = (x)*(-1);
  }
  rpu(up, x, 10);
}


// major work:
int ufprintf(UART *up, char *fmt, ...)
{
  // Write code to print char, string, unsigned integer, integer
  //                 by   %c     %s           %x           %d
  va_list args;
	va_start(args, fmt);
	for(; *fmt != '\0'; fmt++)
	{
		if(*fmt == '%')
		{
			fmt++;
			switch(*fmt)
			{
				case 'c': uputc(up, va_arg(args, int));
					break;
				case 's': uprints(up, va_arg(args, char*));
					break;
				case 'u': uprintu(up, va_arg(args, uint32_t));
					break;
				case 'd': uprintd(up, va_arg(args, uint32_t));
			}
		}
		else
		{
			uputc(up, *fmt);
		}
  }
  va_end(args);
}



// major work:
int uprintf(char *fmt, ...)
{
  UART *up = &uart[0];
  // Write code to print char, string, unsigned integer, integer
  //                 by   %c     %s           %x           %d
  va_list args;
	va_start(args, fmt);
	for(; *fmt != '\0'; fmt++)
	{
		if(*fmt == '%')
		{
			fmt++;
			switch(*fmt)
			{
				case 'c': uputc(up, va_arg(args, int));
					break;
				case 's': uprints(up, va_arg(args, char*));
					break;
				case 'u': uprintu(up, va_arg(args, uint32_t));
					break;
				case 'd': uprintd(up, va_arg(args, uint32_t));
			}
		}
		else
		{
			uputc(up, *fmt);
		}
  }
  va_end(args);
}