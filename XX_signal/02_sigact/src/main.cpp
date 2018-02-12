#include "mytime.h"
#include <errno.h>
#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

using namespace std;

///////////////////////////
void sigHandler(int sig) {
  cout << __FUNCTION__ << ": received signal, and go to sleep..." << endl;
  cout << __FUNCTION__ << "sleep start: " << strtime() << endl;
  sleep(3);
  cout << __FUNCTION__ << "sleep end: " << strtime() << endl;
  cout << __FUNCTION__ << ": wake up!" << endl;
}

void child(bool *flg) {
  sigset_t set;
  struct sigaction new_act, old_act;

  new_act.sa_handler = sigHandler;
  sigemptyset(&set);
  sigaction(SIGUSR1, &new_act, &old_act);
  sigaddset(&set, SIGUSR1);

  cout << __FUNCTION__ << ": launched successfully." << endl;

  *flg = true;
  sleep(2);

  cout << __FUNCTION__ << ": add set" << endl;
}
///////////////////////////
void parent(pid_t pid) {
  int i = 10;
  while (i) {
    cout << __FUNCTION__ << ": send signal!"
         << "[" << i << "] : " << strtime() << endl;
    kill(pid, SIGUSR1);
    i--;
  }

  int status;
  waitpid(pid, &status, WNOHANG);
  if (WIFSIGNALED(status)) // child received signal or not
    cout << __FUNCTION__ << ": child process received signal.." << endl;
}
///////////////////////////
int main(int argc, char *argv[]) {
  cout << strtime() << endl;

  bool *flg = (bool *)mmap(NULL, sizeof(bool), PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (flg == MAP_FAILED) {
    cerr << "mmap error: " << strerror(errno) << endl;
  }

  *flg = false;

  pid_t pid = fork();
  if (pid == 0) {
    // child
    child(flg);
  } else {
    while (!*flg) {
      cout << "parent waiting for child launched..." << endl;
      sleep(2);
    }
    // parent
    parent(pid);
  }

  return 0;
}
