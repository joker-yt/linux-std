#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

const char *FIFO_NAME = "fifo";

int main(int argc, char *argv[]) {
  struct stat st = {0};
  int stat_result = stat(FIFO_NAME, &st);
  if (stat_result != 0) {
    // not exist, create fifo
    mkfifo(FIFO_NAME, 0777);
  } else {
    // already existed, confirm whether it is a fifo or not.
    if (!S_ISFIFO(st.st_mode)) {
      std::cerr << "fifo has already exists, buf not been fifo. "
                << "\n";
      return -1;
    }
    // "fifo" has exist!
  }

  pid_t pid = fork();
  int fd;
  size_t sz;
  char buf[32] = {0};

  if (pid == 0) {
    // child
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
      std::cerr << "child: can't open fifo"
                << "\n";
      return -1;
    }

    std::cout << "child: wait for a message from parent."
              << "\n";
    sz = read(fd, buf, sizeof(buf));
    time_t t = time(NULL);
    struct tm *ptm = localtime(&t);
    char t_buf[32] = {0};
    strftime(t_buf, sizeof(t_buf), "%A %Y-%m-%d %H:%M:%S", ptm);
    std::cout << "child:[" << t_buf << "] receive message. exit."
              << "\n";
  } else {
    // parent
    fd = open(FIFO_NAME, O_RDWR);
    if (fd == -1) {
      std::cerr << "parent: can't open fifo"
                << "\n";
      return -1;
    }
    sprintf(buf, "message from parent");
    sz = write(fd, buf, sizeof(buf));
    time_t t = time(NULL);
    struct tm *ptm = localtime(&t);
    char t_buf[32] = {0};
    strftime(t_buf, sizeof(t_buf), "%A %Y-%m-%d %H:%M:%S", ptm);
    std::cout << "parent:[" << t_buf << "] send message. exit."
              << "\n";
  }

  return 0;
}
