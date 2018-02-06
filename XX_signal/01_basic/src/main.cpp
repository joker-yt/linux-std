#include <errno.h>
#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

using namespace std;

class Cready;
static int sig_count = 0;

//####################################
char *strtime() {
  static char *buf = new char[32];

  time_t t = time(NULL);
  struct tm *ptm = localtime(&t);
  strftime(buf, sizeof(buf), "%A %Y-%m-%d %H:%M:%S", ptm);

  return buf;
}
//####################################

static void sigHandler(int sig) {
  cout << "receive signal: " << strsignal(sig) << endl;
}

static void handler(int sig) {
  static int count = 0;
  cout << __FUNCTION__ << " receive[" << count++ << "]" << endl;
}

static void no_func(int sig) { cout << __FUNCTION__ << endl; }

void child(pid_t pid) {
  cout << __FUNCTION__ << "[" << getpid() << "] -> start" << endl;
  signal(SIGUSR1, handler);
  while (1) {
    sleep(1);
  }
}

void parent(pid_t pid) {
  cout << __FUNCTION__ << "[" << getpid() << "] -> start" << endl;

  while (1) {
    cout << __FUNCTION__ << "[" << getpid() << "]"
         << ":send signal[" << sig_count++ << "] to " << pid << endl;
    kill(pid, SIGUSR1);
    sleep(2);

    int status;
    waitpid(pid, &status, WNOHANG);

    const string signaled{"signaled"};
    const string no_signaled{"NOT signaled"};
    cout << __FUNCTION__ << "[" << getpid() << "]"
         << " child status is ";
    if (WIFSIGNALED(status)) // child received signal or not
      cout << "(" << signaled << ")";
    else
      cout << "(" << no_signaled << ")";

    cout << " <- " << strsignal(WTERMSIG(status)) << endl;
  }
}

int main(int argc, char *argv[]) {
  pid_t pid = fork();
  cout << __FUNCTION__ << ":[" << getpid() << "]: fork result ->" << pid
       << endl;
  if (pid < 0) {
    cerr << strerror(errno) << endl;
    return -1;
  } else if (pid == 0) {
    // child
    child(pid);
  } else {
    // parent
    sleep(1);
    parent(pid);
  }

  cout << pid << " is return" << endl;

  return 0;
}
