/*******************************************************
*                      t.c file                        *
*******************************************************/
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define TRK 18
#define CYL 36
#define BLK 1024

#include "ext2.h"
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;
GD    *gp;
INODE *ip;
DIR   *dp;

char buf1[BLK], buf2[BLK];
int color = 0x0A;
u8 ino;
int prints(char *s)
{
    while(*s){
        putc(*s++);
    }
}

int gets(char *s)
{ 
    char temp;
    while((temp = getc()) != '\r'){ // Enter key
        putc((*s++ = temp));
    }
    *s = 0; //add null char at the end
}



u16 NSEC = 2;
char buf1[BLK], buf2[BLK];

u16 getblk(u16 blk, char *buf)
{
    readfd( (2*blk)/CYL, ( (2*blk)%CYL)/TRK, ((2*blk)%CYL)%TRK, buf);
}


main()
{ 
    u16 i, iblk;
    char temp[256];

    prints("read block 2 GD \n");

    getblk(2,buf1);

    iblk = ((GD*)buf1)->bg_inode_table;

    getblk(iblk,buf1);

    ip = (INODE*)buf1+1;

    prints(" ****ROOT DIRECTORY*******\n");

    for(i = 0; i< 12 && ip->i_block[i];i++){
        getblk((u16)ip->i_block[i],buf2);

        dp = ((DIR*)buf2);

        while((char*)dp < buf2 + BLK){
            strncpy(temp,dp->name,dp->name_len);
            temp[dp->name_len] = 0;

            prints(temp);

            prints("\n\r");

            dp = (DIR*) ((char*)dp + dp->rec_len);
        }
    }
    prints("FINISHED");

    return 1;
}
