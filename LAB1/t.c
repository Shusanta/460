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


char buf1[BLK], buf2[BLK];
int color = 0x0A;
int prints(char *s)
{
    while(*s){
        putc(*s++);
    }
}

// int gets(char *s)
// { 
//     char temp;
//     while((temp = getc()) != '\r'){ // Enter key
//         putc((*s++ = temp));
//     }
//     *s = 0; //add null char at the end
// }

u16 getblk(u16 blk, char *buf)
{
    readfd( (2*blk)/CYL, ( (2*blk)%CYL)/TRK, ((2*blk)%CYL)%TRK, buf);
}

u16 search(INODE *ip, char *name)
{
int i; char c; DIR* dp;
for (i = 0; i<12; i++){
 if ((u16)ip->i_block[i]){

  getblk((u16)ip->i_block[i], buf1);
  dp = (DIR *)buf1;

      while ((char *)dp < &buf1[BLK]){
        c = dp->name[dp->name_len];  //save last byte
        dp->name[dp->name_len] = 0;  // make name into string
        prints(dp->name); putc(' '); // show dp->name string
        if ( strcmp(dp->name, name) == 0){
          prints("\n\r");
          return((u16)dp->inode);
        }
        dp->name[dp->name_len] = c;
        dp = (char *)dp + dp->rec_len;
      }
    }
  }
error();
}



main()
{ 
    char *cp, *name[2];
    u16 i, ino,blk,iblk;
    u32 *up;

    GD    *gp;
    INODE *ip;
    DIR   *dp;

    
    name[0] = "boot";
    name[1] = "mtx";

    prints("bootname: ");

    getblk(2, buf1); // get the  group descriptor  to find the start block of the inodes table
    gp = (GD *)buf1;
    iblk = (u16)gp->bg_inode_table;
    getblk(iblk,buf1); // read in the start

    ip = (INODE *)buf1 +1; // go to the root inode

    // from the root Inodes data blocks
    //search for the first component "boot"
    // Once the entry boot is found, we know its inode number
    // we use mailmans algorithm
    for (i=0; i<2; i++){ 
        ino = search(ip,name[i])-1;
        if(ino <0) error();
        getblk(iblk +(ino/8),buf1);
        ip = (INODE*)buf1 +(ino%8);

    }

    
    getblk((u16)ip->i_block[12],buf2);
    
    
    setes(0x1000);

    for(i = 0; i < 12; i++){
        getblk((u16)ip->i_block[i],buf1);
        putc('*');
    }
    if((u16)ip->i_block[12])
    {
        getblk((u16)ip->i_block[12], buf2);
    }
    setes(0x1000);
    for(i = 0; i < 12; i++){
        getblk((u16)ip->i_block[i],0);
        inces(); putc('*');
    }

    if ((u16)ip->i_block[12]){
        up = (u32 *)buf2;
        while(*up){
            getblk((u16)*up,0);
            inces(); putc('.');
            up++;
        }
    }
    prints("ready to go? "); 
    getc();
    return 1;
}
