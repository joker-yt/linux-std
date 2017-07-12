#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>

const int BUF_MAX = 64 * 2; //
const int TIM_STR_OFFSET = 0;
const int TIM_STR_SIZE = 64;
const int MSG_OFFSET = TIM_STR_SIZE;
const int MSG_SIZE = BUF_MAX - TIM_STR_SIZE;
const char *PARENT = "parent";
const char *CHILD = "child";
const char *EOT = "0";

/*! \fn void time_string(char *buf, size_t buf_siz)
 * @brief write the string of current time to buffer
 * @detail write the string of current time to buffer
 */
void time_string(char *buf, size_t buf_siz) {
  time_t t;
  struct tm *tms;

  t = time(NULL);
  tms = localtime(&t);
  strftime(buf, buf_siz, "%a %Y-%m-%d %H:%M:%S %Z", tms);
}

/*! \fn void function_PARENT()
 * @brief parent block
 * @detail parent block
 */
void function_PARENT(int read_fd) {
  char buf[BUF_MAX] = {0};
  size_t sz = 0;

  while (1) {
    sz = read(read_fd, &buf[MSG_OFFSET], MSG_SIZE - 1);
    if (sz == 0) {
      std::cout << "no data received."
                << "\n";
      break;
    }

    time_string(&buf[TIM_STR_OFFSET], TIM_STR_SIZE);

    std::cout << PARENT << ": received msg["
              << std::string(&buf[TIM_STR_OFFSET], TIM_STR_SIZE) << "] ->"
              << std::string(&buf[MSG_OFFSET], MSG_SIZE) << "\n";

    // Finish?
    if (strcmp(EOT, &buf[MSG_OFFSET]) == 0) {
      std::cout << PARENT << ": Finish"
                << "\n";
      break;
    }
    memset(buf, 0, sizeof(buf));
  }
}

/*! \fn void function_CHILD()
 * @brief child block
 * @detail child block
 */
void function_CHILD(int write_fd) {
  char buf[BUF_MAX * 2] = {0};
  size_t sz = 0;
  struct timespec sleeptime = {0, 1000 * 500};

  while (1) {
    std::cin >> &buf[MSG_OFFSET];
    sz = write(write_fd, &buf[MSG_OFFSET], MSG_SIZE - 1);

    time_string(&buf[TIM_STR_OFFSET], TIM_STR_SIZE - 1);

    std::cout << CHILD << ": sent msg["
              << std::string(&buf[TIM_STR_OFFSET], TIM_STR_SIZE) << "] ->"
              << std::string(&buf[MSG_OFFSET], MSG_SIZE) << "\n";

    // Finish?
    if (strncmp(EOT, &buf[MSG_OFFSET], MSG_SIZE) == 0) {
      std::cout << CHILD << ": Finish"
                << "\n";
      break;
    }

    memset(buf, 0, sizeof(buf));
    nanosleep(&sleeptime, NULL);
  }
}

/*! \fn int main(int argc, char *argv[])
 * @brief main
 * @detail main
 */
int main(int argc, char *argv[]) {
  int fd[2];
  if (pipe(fd) != 0) {
    std::cout << "pipe creation error : " << strerror(errno) << "\n";
    return -1;
  }

  pid_t pid = fork();
  if (pid == 0) {
    // child
    function_CHILD(fd[1]);
  } else {
    // parent
    function_PARENT(fd[0]);
  }

  const char *process = pid == 0 ? CHILD : PARENT;
  std::cout << "Process: " << process << " is finished."
            << "\n";

  return 0;
}
