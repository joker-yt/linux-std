#include "mytime.h"
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

using namespace std;

#define DEBUG_RESTART
const int rd_siz = 100000;
bool flg = false;

void sigHandler(int sig) { /* code */
  cout << __FUNCTION__ << ": " << strtime() << endl;
}

void child(pid_t pid) { /* code */
  sigset_t set;

  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  struct sigaction new_act, old_act;
  new_act.sa_handler = sigHandler;
#ifdef DEBUG_RESTART
  new_act.sa_flags = SA_RESTART;
#endif
  sigaction(SIGUSR1, &new_act, &old_act);

  int fd = open("/dev/zero", O_RDONLY);
  if (fd < 0) {
    cout << __FUNCTION__ << ": " << strerror(errno) << endl;
    return;
  }

  size_t siz;
  char *buf = new char[rd_siz];

  while (1) {
    cout << __FUNCTION__ << ": start reading! " << strtime() << endl;
    siz = read(1, buf, rd_siz);
    // siz = read(fd, buf, rd_siz);
    cout << __FUNCTION__ << ": "
         << "[" << siz << "]" << strerror(errno) << " : " << endl;

    if (siz != rd_siz) {
      cout << __FUNCTION__ << ": siz -> " << siz << endl;
      if (errno == EINTR) {
        cout << __FUNCTION__ << "[1]: " << strerror(errno) << endl;
        break;
      } else {
        cout << __FUNCTION__ << "[2]: " << strerror(errno) << endl;
        break;
      }
    }
    cout << __FUNCTION__ << ": finish reading! " << strtime() << endl;

    sleep(3);
    cout << "hoge" << endl;
  }
}

void parent(pid_t pid) { /* code */
  while (1) {
    cout << __FUNCTION__ << ": send signal : " << strtime() << endl;
    kill(pid, SIGUSR1);
    sleep(5);

    int status;
    waitpid(pid, &status, WNOHANG);
    if (WIFSIGNALED(status)) // child received signal or not
      cout << __FUNCTION__ << ": child process received signal.."
           << strsignal(WTERMSIG(status)) << endl;
  }
}

int main(int argc, char *argv[]) {
  pid_t pid = fork();
  if (pid == 0) {
    child(pid);
  } else {
    sleep(8);
    parent(pid);
  }

  return 0;
}
