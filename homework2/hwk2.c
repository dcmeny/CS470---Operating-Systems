#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
  pid_t pid, pid1;

  pid = fork();
  if (pid < 0){
    fprintf(stderr, "Ford failed");
    return 1;
  }

  else if (pid == 0) {
    pid1 = getpid();
    printf("child: pid = %d", pid);
    printf("child: pid = %d", pid1);
  }

  else {
    pid1 = getpid();
    printf("parent: pid = %d", pid);
    printf("parent: pid = %d", pid1);
    //wait(NULL);
  }

  return 0;

}