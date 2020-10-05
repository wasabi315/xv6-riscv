#include "kernel/types.h"
#include "user/user.h"

int main(void) {
  int parent = getpid();
  int child = fork();
  if (child == 0) {
    printf("CHILD: parent=%d child=%d getpid()=%d getppid=()=%d\n",
           parent, child, getpid(), getppid());
  } else {
    wait(0);
    printf("PARENT: parent=%d child=%d getpid()=%d getppid=()=%d\n",
           parent, child, getpid(), getppid());
  }
  exit(0);
}
