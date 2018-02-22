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

const int SIG_RT1 = SIGRTMIN;
const int SIG_RT2 = SIG_RT1 + 1;

void sigAction1(int sig, siginfo_t *inf, void *dt) {
  cout << __FUNCTION__ << ": received signal!!!! -> "
       << strsignal(inf->si_signo) << endl;
}

void sigAction2(int sig, siginfo_t *inf, void *dt) {
  cout << __FUNCTION__ << ": received signal!!!! -> "
       << strsignal(inf->si_signo) << endl;
}

void child(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;
  struct sigaction new_act, old_act;
  new_act.sa_flags = SA_SIGINFO;
  new_act.sa_sigaction = sigAction1;
  sigemptyset(&new_act.sa_mask);
  sigaddset(&new_act.sa_mask, SIG_RT1);
  sigaddset(&new_act.sa_mask, SIG_RT2);
  sigaction(SIG_RT1, &new_act, &old_act);

  new_act.sa_sigaction = sigAction2;
  sigaction(SIG_RT2, &new_act, &old_act);

  sigset_t prev;
  sigprocmask(SIG_BLOCK, &new_act.sa_mask, &prev);
  cout << __FUNCTION__ << ": ready...blocking" << endl;
  sleep(8);
  cout << __FUNCTION__ << ": unblocking" << endl;
  sigprocmask(SIG_SETMASK, &prev, NULL);

  // sleep() can be used because sleep() is made by signal,
  // so sleep() returns just after received any signal.....
  int i = 3;
  while (i--) {
    pause();
    cout << __FUNCTION__ << ": wake up!" << endl;
  }

  cout << __FUNCTION__ << ": end" << endl;
}

void parent(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;
  cout << __FUNCTION__ << ": send real time signal -> " << strsignal(SIG_RT1)
       << endl;
  union sigval val;
  val.sival_int = 1;
  sigqueue(pid, SIG_RT1, val);

  sleep(1);

  cout << __FUNCTION__ << ": send real time signal -> " << strsignal(SIG_RT2)
       << endl;
  val.sival_int = 2;
  sigqueue(pid, SIG_RT2, val);

  sleep(1);

  cout << __FUNCTION__ << ": send real time signal -> " << strsignal(SIG_RT1)
       << endl;
  val.sival_int = 1;
  sigqueue(pid, SIG_RT1, val);

  // ----------------------------------------
  // real time signal has priority!!!!
  // the receiving order is SIG_RT1, SIG_RT1, SIG_RT2
  // ----------------------------------------
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
