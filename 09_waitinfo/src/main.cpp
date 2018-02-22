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

void child(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;

  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);

  ///// sigprocmask must be needed before sigwaitinfo!!
  sigprocmask(SIG_SETMASK, &set, NULL);

  cout << __FUNCTION__ << ": start waiting ..." << endl;
  siginfo_t inf;
  sigwaitinfo(&set, &inf);

  cout << __FUNCTION__ << ": received signal!!!! -> " << strsignal(inf.si_signo)
       << endl;

  cout << __FUNCTION__ << ": end" << endl;
}

void parent(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;

  cout << __FUNCTION__ << ": sleeping for a while ..." << endl;
  sleep(5);

  cout << __FUNCTION__ << ": send signal!" << endl;
  kill(pid, SIGUSR1);

  int status;
  waitpid(pid, &status, WNOHANG);
  if (WIFSIGNALED(status)) // child received signal or not
    cout << __FUNCTION__ << ": child process received signal.."
         << strsignal(WTERMSIG(status)) << endl;
  else {
    if (WIFEXITED(status))
      cout << __FUNCTION__ << ": child process exit normally" << endl;
    else
      cout << __FUNCTION__ << ": child process status?? -> " << status << endl;
  }
  sleep(5);

  cout << __FUNCTION__ << ": end" << endl;
}

int main(int argc, char *argv[]) {
  pid_t pid = fork();

  if (pid == 0) {
    child(pid);
  } else {
    parent(pid);
  }
  return 0;
}
