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
  cout << __FUNCTION__ << ": received signal!!!!! -> "
       << strsignal(inf->si_signo) << endl;
}

void child(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;
  sigset_t block, tmp;
  sigemptyset(&block);
  sigaddset(&block, SIGUSR1);
  sigprocmask(SIG_BLOCK, &block, &tmp);

  cout << __FUNCTION__ << ": now sleeping for setting" << endl;

  sleep(5);

  struct sigaction new_act, old_act;
  new_act.sa_flags = SA_SIGINFO;
  new_act.sa_sigaction = sigAction;
  sigemptyset(&new_act.sa_mask);
  //!!!!!!!!!!!!!!!!!!!
  // sigaddset(&new_act.sa_mask, SIGUSR1);
  sigaction(SIGUSR1, &new_act, &old_act);

  sigsuspend(&tmp);
  cout << __FUNCTION__ << ": through out suspend..." << endl;

  cout << __FUNCTION__ << ": end" << endl;
}

void parent(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;

  int i = 10;
  while (i--) {
    cout << __FUNCTION__ << ": sending signal" << endl;
    kill(pid, SIGUSR1);
    sleep(1);
  }

  int status;
  waitpid(pid, &status, WNOHANG);
  if (WIFSIGNALED(status)) // child received signal or not
    cout << __FUNCTION__ << ": child process received signal.."
         << strsignal(WTERMSIG(status)) << endl;

  cout << __FUNCTION__ << ": end" << endl;
}

int main(int argc, char *argv[]) {
  pid_t pid = fork();

  if (pid == 0) {
    child(pid);
  } else {
    sleep(1);
    parent(pid);
  }

  return 0;
}
