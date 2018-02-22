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

void sigHandler(int sig) {
  cout << __FUNCTION__ << ": received signal!!!" << endl;
}

void child(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;

  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  struct sigaction new_act, old_act;
  new_act.sa_handler = sigHandler;
  sigaction(SIGUSR1, &new_act, &old_act);

  cout << __FUNCTION__ << ": now pausing..." << endl;
  pause();
  cout << __FUNCTION__ << ": finish pausing..." << endl;
}

void parent(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;

  cout << __FUNCTION__ << ": send signal" << endl;
  kill(pid, SIGUSR1);

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
    sleep(3);
    parent(pid);
  }

  return 0;
}
