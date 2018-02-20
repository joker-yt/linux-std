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

void sigHandler(int sig) {
  static int count = 0;
  count++;
  cout << __FUNCTION__ << ":!!!!! " << strtime() << "(" << strsignal(sig) << ")"
       << count << endl;
}

void child(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;
  sleep(2);

  sigset_t block, prev;

  sigemptyset(&block);
  sigaddset(&block, SIGUSR1);
  // sigprocmask(SIG_BLOCK, &block, &prev);

  // set action for signal received
  struct sigaction new_act, old_act;
  new_act.sa_handler = sigHandler;
  if (sigaction(SIGUSR1, &new_act, &old_act)) {
    cerr << __FUNCTION__ << ": sigaction -> " << strerror(errno) << endl;
    return;
  }

  int i = 5;
  while (i--) {
    cout << __FUNCTION__ << ": now blocking ...SIGUSR1" << endl;
    sigprocmask(SIG_BLOCK, &block, &prev);
    sleep(1);
    cout << __FUNCTION__ << ": unblocked ...SIGUSR1, and 2 sec sleep" << endl;
    sigprocmask(SIG_SETMASK, &prev, NULL);
    int j = 2;
    while (j--)
      cout << __FUNCTION__ << ": ............" << endl;
  }

  cout << __FUNCTION__ << ": end" << endl;
}

void parent(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;

  while (1) {
    cout << __FUNCTION__ << ": send signal[" << strsignal(SIGUSR1) << "]"
         << "(" << strtime() << ")" << endl;
    kill(pid, SIGUSR1);

    // WNOHANG is specified, so return immediately
    // if childs' status has not changed yet.
    int status;
    if (waitpid(pid, &status, WNOHANG) != 0) {
      if (WIFSIGNALED(status)) // child received signal or not
        cout << __FUNCTION__ << ": child process received signal.."
             << strsignal(WTERMSIG(status)) << endl;
    }

    cout << __FUNCTION__ << ": sleep 1 sec" << endl;
    sleep(1);
  }

  cout << __FUNCTION__ << ": end" << endl;
}

int main(int argc, char *argv[]) {
  pid_t pid = fork();

  if (pid == 0) {
    // child
    child(pid);
  } else {
    // parent
    sleep(5);
    parent(pid);
  }

  return 0;
}
