
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc!=2){  //判断参数个数是否合法
    fprintf(2, "it must be 1 argument for sleep\n");//错误提示
    exit();
  }
  int sleepnum = atoi(argv[1]);
  printf("(nothing happens for a little while)\n");
  sleep(sleepnum); 
  exit();
}
