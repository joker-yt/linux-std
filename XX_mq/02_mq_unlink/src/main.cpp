#include <errno.h>
#include <iostream>
#include <mqueue.h>
#include <string.h>

using namespace std;

const int err = -1;
int main(int argc, char *argv[]) {

  int ret = mq_unlink("/test");
  if (ret == err) {
    cout << __FUNCTION__ << ": " << strerror(errno) << endl;
  } else {
    cout << __FUNCTION__ << ": mq_unlink -> success!" << endl;
  }
  return 0;
}
