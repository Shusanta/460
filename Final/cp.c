#include "ucode.c"


int main(int argc, char *argv[]){
    char buf[BLKSIZE];
    int fd_src, fd_dest;
    int n, bytes = 0;


    prints("*****SHUSANTAS CP*******");

    if(argc < 3)
    {
        exit(1);
    }


    fd_src = open(argv[1],O_RDONLY);


    if(fd_src < 0)
    {
        exit(2);
    }

    fd_dest = open(argv[2], O_WRONLY | O_CREAT);

    if(fd_dest <0){
        exit(3);
    }

    while (n = read(fd_src,buf, BLKSIZE))
    {
        write(fd_dest, buf,n);
    }

    close(fd_src);
    close(fd_dest);
    exit(0);


}