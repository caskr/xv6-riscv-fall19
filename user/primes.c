#include "user.h"

void screen (int num[], int n){

    int prime = num[0];
    printf("prime %d\n",prime);
    if(n == 1)
        exit();

    int p[2];
    pipe(p);
    
    int i;
    for(i = 0; i < n; i++){
        write(p[1],&num[i],1);
    }

    close(p[1]);
    if(fork() == 0){
        int buf[1];
        i = 0;
        while(read(p[0],buf,1)!=0){
            if(buf[0] % prime != 0){
                num[i] = buf[0];
                i++;
            }
        }
        screen(num,i);
        exit();
    }
    wait();
}

int
main(){
    int num[34];
    for(int i = 0; i<=33 ;i++){
        num[i]= i + 2;
    }
    screen(num, 34);
    exit();
}