#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
  if (shm_unlink(argv[1]) == -1) {
    printf("shm_unlink: %s\n", strerror(errno));
    return -1;
  }

  return 0;
}
