#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <string.h>
#include <sys/stat.h>

using namespace std;

int main(int argc, char *argv[]) {
  cout << "Com1 start" << endl;

  // if specify O_EXCL, return error in case of already existed.
  struct stat st;
  sem_t *sp;
  if (stat("/dev/shm/sem_test", &st)) {

    sem_unlink("sem_test");
    sp = sem_open("/sem_test", O_CREAT, 0666, 0);
    if (SEM_FAILED == sp) {
      cerr << "Com1 open error2 :" << strerror(errno) << endl;
      return -1;
    }

  } else {
    sp = sem_open("/sem_test", 0, 0777, 0);
    if (SEM_FAILED == sp) {
      cerr << "Com1 open error :" << strerror(errno) << endl;
      return -1;
    }
  }

  cout << "Com1 open successfully" << endl;

  // sem_trywait, sem_timedwait
  int ret = sem_wait(sp);
  if (ret == -1) {
    cerr << "Com1 wait error" << endl;
    return -1;
  }

  cout << "Com1 end." << endl;

  sem_close(sp);
  sem_unlink("/sem_test");

  return 0;
}
