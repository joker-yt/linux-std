#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZ 1000

int main(int argc, char *argv[]) {
  int fd = shm_open(argv[1], O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
  if (fd == -1) {
    printf("shm_open: %s\n", strerror(errno));
    return -1;
  }

  if (ftruncate(fd, SIZ) == -1) {
    printf("ftruncate: %s\n", strerror(errno));
    return -1;
  }

  void *addr = mmap(NULL, SIZ, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    printf("mmap: %s\n", strerror(errno));
    return -1;
  }

  return 0;
}
