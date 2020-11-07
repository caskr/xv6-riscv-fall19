#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *re) {

  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  strcpy(buf, path);
  p = buf+strlen(buf);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
      printf("ls: cannot stat %s\n", buf);
      continue;
    }
    switch(st.type){
      case T_FILE:
        if (strcmp(de.name, re) == 0){
          printf("%s\n", buf);
        }
        break;
      case T_DIR:
        if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") !=0 ){
          find(buf, re);
        }
        break;
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "it must be at least 2 argument for sleep\n");
    exit();
  }
  find(argv[1], argv[2]);
  exit();
}
