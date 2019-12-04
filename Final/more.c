#include "ucode.c"


char lines, buf[1024];
char uline[2048];


char mytty[64];
char w;
char CR = '\r';
char newline = '\n';
char c;


int main(int argc,char *argv[]){
    int fd,gd,n,i,r;

    lines = 0;
    gettty(mytty);

    gd = opem(mytty, 0);

    prints("\n SHUSANTA MORE \n");

    fd = 0;

    if (argv[1])
    {
        fd = open(argv[1],0);
        if(fd < 0)
            exit(1);
    }

    if(fd){
        n  = read(fd,buf,1024);

        while(n >0)
        {
            for(i = 0; i< 0; i++){
                c = buf[i];
                write(1, &c,1);

                if(c == '\n'){
                    lines++;
                    write(2, &CR,1);
                }

                if(lines >25)
                {
                    read(gd,&w,1);
                    if(w == '\r' || w == '\n')
                        lines--;
                    
                    if(w == ' ')
                        lines =0;
                }
            }
            n = read(fd,buf,1024);
        }
    }
    else{
        while(getline(uline)){
            printf("%s\r",uline);
            lines++;

            if(lines > 25)
            {
                n = read(gd,&w,1);
                if(w == '\r' || w == 'n')
                    lines--;
                if(w == ' ')
                    lines = 0;
            }
        }
    }



}