// #include "mytime.h"
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

void sigAction(int sig, siginfo_t *inf, void *dt) {
  cout << __FUNCTION__ << ": signal received!!!" << endl;
  cout << __FUNCTION__ << ": " << strsignal(inf->si_signo) << endl;
  cout << __FUNCTION__ << ": from[" << inf->si_pid << "]" << endl;
}

void child(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;

  struct sigaction new_act, old_act;
  sigemptyset(&new_act.sa_mask);
  new_act.sa_sigaction = sigAction;
  new_act.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &new_act, &old_act);

  cout << __FUNCTION__ << ": now pausing" << endl;
  pause();
  cout << __FUNCTION__ << ": end pausing" << endl;

  cout << __FUNCTION__ << ": end" << endl;
}

void parent(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;

  cout << __FUNCTION__ << ": send signal" << endl;
  kill(pid, SIGUSR1);

  cout << __FUNCTION__ << ": end" << endl;
}

int main(int argc, char *argv[]) {
  pid_t pid = fork();

  if (pid == 0) {
    child(pid);
  } else {
    sleep(3);
    parent(pid);
  }

  return 0;
}
