//// grep.c

#include "ucode.c"




int main(int argc, char * argv[])
{
    char buffer[BLKSIZE];
    char tempBuf[BLKSIZE];

    char * tb = tempBuf;
    int fd;

    char grepStart;

    if(argc < 2)
    {
        prints("USAGE: grep pattern [filee] \n");
        return 0;
    }

    if(argc > 2)
    {
        close(0);
        fd = open(argv[2], O_RDONLY);
    }

    if (fd <0){
        exit(1);
    }

    while(read(fd,&grepStart,1)){
        
        if(grepStart == '\r' || grepStart == '\n'){
            tb = tempBuf;

            if(strstr(tempBuf,argv[1])){
                printf("%s\n\r",tempBuf);
            }

            memset(tempBuf,0,BLKSIZE);
        }
        else{
            *tb++ = grepStart;
        }
        
    }

    close(fd);
}