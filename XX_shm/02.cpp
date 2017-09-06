#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

const char test_string[] = u8"this is sample string";

int main(int argc, char *argv[]) {
  int fd = shm_open(argv[1], O_RDWR, 0);
  if (fd == -1) {
    printf("shm_open: %s\n", strerror(errno));
    return -1;
  }

  size_t len = strlen(test_string);
  if (ftruncate(fd, len) == -1) {
    printf("ftruncate: %s\n", strerror(errno));
    return -1;
  }

  void *addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    printf("mmap: %s\n", strerror(errno));
    return -1;
  }

  close(fd);

  memcpy(addr, test_string, len);

  return 0;
}
