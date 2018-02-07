#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <string.h>
#include <sys/stat.h>

using namespace std;

int main(int argc, char *argv[]) {
  cout << "Com2 start" << endl;

  // if specify O_EXCL, return error in case of already existed.
  struct stat st;
  sem_t *sp;
  if (stat("/dev/shm/sem_test", &st)) {
    sp = sem_open("/sem_test", 0, 0777, 0);
    if (SEM_FAILED == sp) {
      cerr << "Com1 open error :" << strerror(errno) << endl;
      return -1;
    }
  } else {
    cerr << "Com1 open error :" << strerror(errno) << endl;
    return -1;
  }

  cout << "Com2 open successfully" << endl;

  cout << "Com2 sleeping" << endl;

  int ret = sem_post(sp);
  if (ret == -1) {
    cerr << "Com2 post error" << endl;
    return -1;
  }

  cout << "Com2 end." << endl;

  sem_close(sp);

  return 0;
}
