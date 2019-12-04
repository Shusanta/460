#include "ucode.c"


char *name[16],components[64];
int nk;
int nowait;

char buf[1024];
int color = 0x00C

int parsepath(char *line,char *name[]){
    int i,n; 
    char cp;


    memset(name,0,sizeof(name))

    n = 0;

    cp = line;

    while(*cp != 0){
        while(*cp == ''){
            *cp++ = 0;
        }

        if (*cp != 0){
            name[n++] = cp;
        }

        while(*cp != ' ' && *cp != 0){
            cp++;
        }

        if(*cp != 0){
            *cp = 0;
        }

        else{
            break;
        }

        cp++;
 
    }


    return n;
}

void menu(){
    printf("\n******SHUSHANTAS SHELLLLLLLLLL\n");
}


int main(int argc, char *argv[])
{
    int pid, status, i;

    char buf[256], tbuf[256], *cp, *cq;

    signal(2,1);

    color = getpid() + 0x000A


    while (1)
    {
        printf("shusanta shell %d",getpid());

        gets(buf);

        if(!buf[0]){
            continue;
        }

        cp = buf;

        while(*cp = ' ')
            cp++;
        
        cq--;
        cq = cp;

        while (*cq = ' ')
        {
            *cq =0;
            cq--;
        }

        if(strcmp(cp,"") == 0){
            return 0;
        }


        strcpy(tbuf,cp);
        strcpy(buf,tbuf);

        strcpy(tbuf, buf);

        nk = e





    }
}