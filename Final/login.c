#include "ucode.c"
// Upon entry argv[0] = login, argv[1] = /dev/ttyX
int in,out,err;

char username[128], password[128];

int tokenize(char * line,char *buf[],char ch)
{
  // tokenize directly from crt0.c
  int len = 0;
  char *cp = line;

  while(*cp != 0){
      //skips the white space
      while (*cp == ' ')
        *cp++ = 0;


      // scans the string
      if (*cp != 0)
        buf[len++] = cp;
      

      //splits the line
      while(*cp != ch && *cp != 0)
        cp++;
      
      if(*cp != 0)
        *cp = 0;

      else
        break;

      cp++;
  }
  return len;
}


int main(int argc, char *argv[]) {
    // 1. close file descriptors 0,1 inherited from INIT
    int i;
    char *lines[128], *acc_info[128], file_read[1024]; 
    close(0);
    close(1);


    // 2. open argv[1] 3 times as in(0), out(1),err(2)

    in = open(argv[1], 0);
    out = open(argv[1], 1);
    err = open(argv[1], 2);

    // 3. settty(argv[1])
    fixtty(argv[1]);

    //4. open /etc/passwd file for READ to pull info

    while(1)
    {
        prints("SHU SIGN LOGIN PROGRAM: ");

        int file = open("/etc/passwd",O_RDONLY);

        prints("\nLogin: ");
        gets(username);

        prints("\nPassword: ");
        gets(password);

        read(file,file_read,1024);

        int len = tokenize(file_read,lines,'\n');

        for(i=0; i<len;i++)
        {
            tokenize(lines[i],acc_info,':');

            //if user has a valid acc
            int username_is_valid = strcmp(username, acc_info[0]) == 0;
            int password_is_valid = strcmp(password, acc_info[1]) == 0;

            if (username_is_valid && password_is_valid){
                // change uid, gid to user's uid, gid;
                chuid(atoi(acc_info[2]), atoi(acc_info[3]));
                
                // change cwd to users home DIR
                chdir(acc_info[5]);

                // close opened /etc/passwd file 
                close(file);


                // exec to program in user account // exec
                exec(acc_info[6]);

                break;
            }
        }
        prints("Wrong login \n");


    }
} 