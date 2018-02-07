#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
  int fd;
  if ((fd = shm_open("/shm_sem", O_RDWR | O_CREAT, 0)) < 0) {
    cout << "shm_open error:" << strerror(errno);
  }

  ftruncate(fd, sizeof(sem_t));

  void *addr =
      mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    cout << "mapping error: " << strerror(errno) << endl;
  }
  sem_t *psem = static_cast<sem_t *>(addr);

  // 2nd param "1" means sharing semaphore between processes.
  int rslt = sem_init(psem, 1, 0);
  if (rslt < 0) {
    cout << "init error: " << strerror(errno) << endl;
    return -1;
  }

  pid_t pid = fork();
  if (pid == 0) {
    // child
    cout << "child is sleeping for 3 sec..." << endl;
    sleep(3);

    cout << "child wakes up, and post semaphore." << endl;
    if (sem_post(psem) < 0) {
      cout << "error occured in child process when post" << endl;
      cout << strerror(errno) << endl;
    }

    int val;
    if (sem_getvalue(psem, &val) < 0) {
      cout << "error occured in child process when get_value" << endl;
      cout << strerror(errno) << endl;
    }
    cout << "child get value of sem:" << val << endl;

    cout << "child is done." << endl;
  } else {
    // parent
    cout << "parent is waiting for semaphore" << endl;

    int val;
    if (sem_getvalue(psem, &val) < 0) {
      cout << "error occured in parent process when get_value" << endl;
      cout << strerror(errno) << endl;
    }
    cout << "parent get value of sem:" << val << endl;

    if (sem_wait(psem) < 0) {
      cout << "error occured in parent process when wait" << endl;
      cout << strerror(errno) << endl;
    }

    cout << "parent is activated again, and destroy semaphore." << endl;
    shm_unlink("/shm_sem");
    sem_destroy(psem);

    cout << "parent is done." << endl;
  }

  return 0;
}
