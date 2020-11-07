
#include "user.h"

int
main(){

    int p1[2];  
    int p2[2];
    char buf[]={"hello\n"};
    //创建管道函数
    pipe(p1);   //父进程写，子进程读
    pipe(p2);   //父进程读，子进程写

    //close(p1[0]);
    //父进程写
    if(write(p1[1],buf,sizeof(buf))!=sizeof(buf)){
            fprintf(2, "parent->son write error!\n");
            exit();
        }
    
    //子进程
    if(fork() == 0){
        
        close (p1[1]);
        //子进程读
        if(read(p1[0],buf,sizeof(buf))!=sizeof(buf)){
            fprintf(2, "parent->son read error!\n");
            exit();
        }
        printf("%d: received ping\n", getpid());

        close (p2[0]);
        //子进程写
        if(write(p2[1],buf,sizeof(buf))!=sizeof(buf)){
            fprintf(2, "son->parent write error!\n");
            exit();
        }
        exit();
    }

    close(p2[1]);
    //父进程读
    if(read(p2[0],buf,sizeof(buf))!=sizeof(buf)){
        fprintf(2, "son->parent read error!\n");
        exit();
    }
    printf("%d: received pong\n", getpid());

    wait();
    exit();
}