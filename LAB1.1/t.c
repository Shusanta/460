/*********  t.c file *********************/

int prints(char *s)
{
    while(*s != '\0'){
        putc(*s++);
    }
}

int gets(char *s)
{
    char temp;
    while((temp = getc()) != '\r'){
        putc((*s++ = temp));
    }
    *s= '\0';
}

char ans[64];

main()
{
  while(1){
    prints("What's your name? ");
    gets(ans);  prints("\n\r");

    if (ans[0]==0){
      prints("return to assembly and hang\n\r");
      return;
    }
    prints("Welcome "); prints(ans); prints("\n\r");
  }
}