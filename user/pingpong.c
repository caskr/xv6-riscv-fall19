
#include "user.h"

int
main(){

    int p1[2];  
    int p2[2];
    char buf[5];
    
    pipe(p1);   //父进程写1，子进程读0
    pipe(p2);   //父进程读0，子进程写1

    //子进程
    if(fork() == 0){
        close (p1[1]);
        //子进程读
        if(read(p1[0],buf,sizeof(buf))!=sizeof(buf)){
            fprintf(2, "parent->son read error!\n");
            exit();
        }
        printf("%d: received %s\n", getpid(),buf);

        close (p2[0]);
        //子进程写
        if(write(p2[1],"pong",5)!=5){
            fprintf(2, "son->parent write error!\n");
            exit();
        }
        exit();
    }
    close(p1[0]);
    //父进程写
    if(write(p1[1],"ping",5)!=5){
        fprintf(2, "parent->son write error!\n");
        exit();
    }

    close(p2[1]);
    //父进程读
    if(read(p2[0],buf,sizeof(buf))!=sizeof(buf)){
        fprintf(2, "son->parent read error!\n");
        exit();
    }
    printf("%d: received %s\n", getpid(),buf);

    wait();
    exit();
}