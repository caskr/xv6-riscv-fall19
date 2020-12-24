#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
 
#define MAXCMD 100
#define MAXARGS 10
#define MAXWORD 20

char args[MAXARGS][MAXWORD];
int pipe_l[MAXARGS];//管道标志位置
int pipe_num = 0;//管道标志数目
int pipe_i = 0; //管道标志已遍历数目

void runpipe(char *argv[],int argc);

/*借鉴sh.c*/
/*代码意义：读入键盘输入并写入buf中*/
int
getcmd(char *buf, int nbuf)
{
  fprintf(2, "@ ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);  //读入单词
  if(buf[0] == 0) // EOF    //无单词
    return -1;
  return 0;
}

int parsecmd(char buf[], char *argv[]){
    
    for(int i = 0; i < MAXARGS; i++){//赋予指针指向
        argv[i] = & args[i][0];
    }

    int i = 0;  //参数个数
    int j = 0;  //buf_index
    memset(pipe_l,0,MAXARGS);
    while(buf[j]!= '\0'&&buf[j]!='\n'){
        
        while(buf[j]==' '||buf[j]=='\n'){
            j++;
        }
        
        argv[i] = buf + j;
        while(buf[j]!=' '&&buf[j]!='\n'){
            j++;
        }

        buf[j] = '\0';  //将空格或换行符写为\0作字符分割
        j++;
        if(strcmp(argv[i],"|")==0){
            pipe_l[pipe_num++] = i;//记录管道标志位置
        }
        i++;//参数个数+1
    }
    argv[i] = 0;//最后一个为0
    return i;//返回参数个数
}


void runcmd(char *argv[], int argc){
    if(pipe_num != pipe_i){//先执行管道指令
        runpipe(argv,argc);
        return;
    }
    for(int i=1;i<argc;i++){
        if(!strcmp(argv[i],">")){//输出重定向
            close(1);//关闭标准输出
            if(open(argv[i+1], O_CREATE|O_WRONLY) < 0){
                fprintf(2, "open %s failed\n", argv[i+1]);
                exit(-1);
            }
            argv[i]=0;//重定向标志设0
        }
        if(!strcmp(argv[i],"<")){//输入重定向
            close(0);//关闭标准输入
            if(open(argv[i+1], O_RDONLY) < 0){
            fprintf(2, "open %s failed\n", argv[i+1]);
            exit(-1);
            }
            argv[i]=0;//重定向标志设0
        }
    }
    exec(argv[0],argv);
}


void runpipe(char *argv[],int argc){
    argv[pipe_l[pipe_i]] = 0;//将管道标志设0
    int i = pipe_l[pipe_i++];
    int p[2];
    if (pipe(p) < 0){
        fprintf(2,"pipe create error!\n");
    }
    if(fork() == 0){
        close(1);//关闭标准输出
        dup(p[1]);//将管道输入端设为标准输出
        close(p[0]);
        close(p[1]);
        runcmd(argv,i);//管道左侧
    }
    close(0);//关闭标准输入
    dup(p[0]);//将管道输出端设为标准输入
    close(p[0]);
    close(p[1]);
    runcmd(argv+i+1,argc-i-1);//管道右侧
    wait(0);
}


int main(){
    char buf[MAXCMD];
    while(getcmd(buf, sizeof(buf)) >= 0){
        if(fork()==0){
            char *argv[MAXARGS];
            int argc = -1;
            argc = parsecmd(buf,argv);
            runcmd(argv,argc);
        }
        wait(0);
    }
    exit(0);
}