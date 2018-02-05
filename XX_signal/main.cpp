#include <errno.h>
#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

static int sig_count = 0;

static void sigHandler(int sig) {
  cout << "receive signal: " << strsignal(sig) << endl;
}

static void handler(int sig) {
  static int count = 0;
  cout << __FUNCTION__ << " receive[" << count++ << "]" << endl;
}

static void no_func(int sig) { cout << __FUNCTION__ << endl; }

void child(pid_t pid) {
  cout << __FUNCTION__ << "[" << getpid() << "]" << endl;
  signal(SIGUSR1, handler);
  while (1) {
    sleep(1);
  }
}

void parent(pid_t pid) {
  while (1) {
    cout << __FUNCTION__ << "[" << getpid() << "]"
         << ":send signal[" << sig_count++ << "] to " << pid << endl;
    kill(pid, SIGUSR1);
    sleep(2);

    int status;
    waitpid(pid, &status, WNOHANG);
    cout << "child returns: " << WIFSIGNALED(status) << " -> "
         << WTERMSIG(status) << endl;
  }
}

int main(int argc, char *argv[]) {
  pid_t pid = fork();
  cout << "[" << getpid() << "]:" << pid << endl;
  if (pid < 0) {
    cerr << strerror(errno) << endl;
    return -1;
  } else if (pid == 0) {
    // child
    child(pid);
  } else {
    // parent
    // sleep(1);
    signal(SIGUSR1, no_func);
    parent(pid);
  }

  cout << pid << " is return" << endl;

  return 0;
}
