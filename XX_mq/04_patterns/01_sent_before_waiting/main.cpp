#include <condition_variable>
#include <errno.h>
#include <fcntl.h> /* For O_* constants */
#include <iostream>
#include <mqueue.h>
#include <mutex>
#include <string.h>
#include <string>
#include <sys/stat.h> /* For mode constants */
#include <thread>
#include <unistd.h>

using namespace std;
mqd_t mqd;
struct mq_attr attr;

mutex mtx;
condition_variable cv;
bool is_ready = false;
const string qname{"/test"};

///////////////////////////////////////
// send thread
///////////////////////////////////////
void thread_send() {
  cout << "snd: launched" << endl;

  cout << "snd: just before sending message" << endl;
  string msg{"this is a message"};
  ssize_t sz = mq_send(mqd, msg.c_str(), msg.length(), 0);
  cout << "snd: just after sending message" << endl;
  if (sz < 0)
    cerr << "recv: " << strerror(errno) << endl;
}

///////////////////////////////////////
// receive thread
///////////////////////////////////////
void thread_receive() {
  cout << "rcv: launched, but sleeping" << endl;
  sleep(3);

  cout << "rcv: ####### wake up #######" << endl;
  char *buffer = new char[8192];

  cout << "rcv: just before waiting message" << endl;
  ssize_t sz = mq_receive(mqd, buffer, attr.mq_msgsize, nullptr);
  if (sz < 0)
    cerr << "rcv: " << strerror(errno) << endl;

  cout << "rcv: " << buffer << endl;
}

///////////////////////////////////////
// main
///////////////////////////////////////
int main(int argc, char *argv[]) {
  mq_unlink(qname.c_str());

  mqd = mq_open(qname.c_str(), O_RDWR | O_EXCL | O_CREAT, 0666, NULL);
  if (mqd < 0) {
    cerr << "rcv: " << strerror(errno) << endl;
  }
  mq_getattr(mqd, &attr);
  cout << attr.mq_msgsize << endl;

  thread ts(thread_send);
  thread tr(thread_receive);

  ts.join();
  tr.join();

  mq_close(mqd);

  return 0;
}
