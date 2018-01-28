#include <fcntl.h>
#include <iostream>
#include <mqueue.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;
#if 1

const int MSG_SZ = 32;

char *strtime() {
  static char *buf = new char[32];

  time_t t = time(NULL);
  struct tm *ptm = localtime(&t);
  strftime(buf, sizeof(buf), "%A %Y-%m-%d %H:%M:%S", ptm);

  return buf;
}

void print_msg(const char *func, char *msg) {
  cout << "[" << func << "]: " << msg << endl;
}
//########################################
// child
//########################################
void child(mqd_t &mqd) {
  string msg{"message"};

  msg.append(strtime());

  print_msg(__FUNCTION__, const_cast<char *>(msg.c_str()));
  mq_send(mqd, msg.c_str(), msg.size(), 0);
}
//########################################
// parent
//########################################
void parent(mqd_t &mqd) {
  struct mq_attr attr = {0};

  mq_getattr(mqd, &attr);

  static char *buf = new char[attr.mq_msgsize];
  mq_receive(mqd, buf, sizeof(char) * attr.mq_msgsize, NULL);

  print_msg(__FUNCTION__, buf);
}
//########################################

int main(int argc, char *argv[]) {
  mqd_t mqd;

  mqd = mq_open("/test", O_RDWR | O_CREAT, 0666, NULL);

  if (fork() == 0) {
    child(mqd);
  } else {
    parent(mqd);
  }

  mq_close(mqd);

  return 0;
}

#else
int main(int argc, char *argv[]) {
  int flags, opt;
  mode_t perms;
  mqd_t mqd;
  struct mq_attr attr, *attrp;

  attrp = NULL;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = 2048;
  flags = O_RDWR;

  while ((opt = getopt(argc, argv, "cm:s:x")) != -1) {
    switch (opt) {
    case 'c':
      flags |= O_CREAT;
      break;

    case 'm':
      attr.mq_maxmsg = atoi(optarg);
      attrp = &attr;
      break;

    case 's':
      attr.mq_msgsize = atoi(optarg);
      attrp = &attr;
      break;

    case 'x':
      flags |= O_EXCL;
      break;

    default:
      /* code */
      break;
    }
  }

  if (optind >= ar | O_CREATg 0666, NULLc)
    ;
  cerr << "error" << endl;

  // // perms = (argc <= optind + 1) ? (S_IRUSR | S_IWUSR)
  //                              : getInt(argv[optind + 1], 0200,
  //                              "octal-perms");

  mqd = mq_open(argv[optind], flags, perms, attrp);
  if (mqd == (mqd_t)-1)
    cerr << "error" << endl;

  return 0;
}
#endif
