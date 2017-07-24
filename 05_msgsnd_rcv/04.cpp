#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

const long MsgType_1 = 1;
const long MsgType_2 = 2;
const long MsgType_3 = 3;

struct st_msgbuf {
  long mtype;
  char mtext[32];
};

int main(int argc, char *argv[]) {
  key_t key = ftok("/tmp/yqueue", 'z');
  int qid = msgget(key, IPC_CREAT | 0666);
  pid_t pid;

  if (fork() == 0) {
    // child
    pid = getpid();
    struct st_msgbuf msg = {pid, {0}};
    sleep(5);
    std::cout << "child: start of msgrcv"
              << "\n";

    for (long l = MsgType_1; l <= MsgType_3; l++) {
      int ret = msgrcv(qid, &msg, sizeof(msg.mtext), l, MSG_NOERROR);
      std::cout << "child: end of msgrcv[" << msg.mtext << "]"
                << "\n";
    }
    std::cout << "child: end of msgrcv"
              << "\n";
  } else {
    // parent
    pid = getpid();
    struct st_msgbuf msg = {0, "this is message"};
    size_t msg_end = strlen(msg.mtext);
    std::cout << "parent: start of msgsnd"
              << "\n";
    for (long l = MsgType_3; l > 0; l--) {
      msg.mtype = l;
      msg.mtext[msg_end] = '0' + l;
      int ret = msgsnd(qid, &msg, sizeof(msg.mtext), MSG_NOERROR);
      std::cout << "parent: msg[" << l << "] sent"
                << "\n";
    }
    std::cout << "parent: end of msgsnd"
              << "\n";
  }

  return 0;
}
