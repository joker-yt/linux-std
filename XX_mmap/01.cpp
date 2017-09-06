#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

void error_return();
int main(int argc, char *argv[]) {
  char *addr;
  int fd;
  struct stat sb;

  fd = open(argv[1], O_RDONLY);
  if (fd == -1)
    error_return();

  if (fstat(fd, &sb) == -1)
    error_return();

  addr = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (addr == MAP_FAILED)
    error_return();

  if (write(1, addr, sb.st_size) != sb.st_size)
    error_return();

  munmap(addr, sb.st_size);
  close(fd);

  return 0;
}

void error_return() {
  printf("error: %s\n", strerror(errno));

  exit(-1);
}
