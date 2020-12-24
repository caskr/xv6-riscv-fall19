#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]){
    
    char word[MAXARG][32];
    char buf, *p[MAXARG];
    int i, j;
    int flag;   //标记是否有有效字符

    if(argc<2){
        fprintf(2,"it must be at least 1 argument for xargs\n");
        exit();
    }
    
    while (1)
    {
        j = 0;
        flag = 0;
        memset(word, 0, MAXARG * 32);

        for(i = 1; i < argc; i++) {
            strcpy(word[j++], argv[i]);
        }

        i = 0;  
        while (i < MAXARG-1) {
            if (read(0, &buf, 1) <= 0) {
                //ctrl+d
                exit();
            }
            if (buf == '\n') {
                for (i = 0; i < MAXARG-1; i++) {
                    p[i] = word[i];
                }
                p[MAXARG-1] = 0;
                if (fork() == 0) {
                    exec(argv[1], p);
                }
                wait();
            }
            else if (buf == ' ') {
                if (flag) {
                j++;
                i = 0;
                flag = 0;
                }
            }
            else{
                word[j][i++] = buf;
                flag = 1;
            }
        }
    }
    exit();
}
