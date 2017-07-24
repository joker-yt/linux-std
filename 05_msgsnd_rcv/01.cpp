#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <unistd.h>

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
    int ret = msgrcv(qid, &msg, sizeof(msg.mtext), 0, MSG_NOERROR);
    std::cout << "child: received size: " << ret << "\n";
    std::cout << "child: received msg from [" << msg.mtype << "]-> "
              << msg.mtext << "\n";
  } else {
    sleep(2);
    pid = getpid();
    struct st_msgbuf msg = {pid, "this is msg"};
    int ret = msgsnd(qid, &msg, sizeof(msg.mtext), MSG_NOERROR);
    if (ret == 0)
      std::cout << "parent: sent msg"
                << "\n";
    else
      std::cout << "parent: msg error"
                << "\n";
  }

  std::cout << "process pid:" << pid << " is finished"
            << "\n";

  return 0;
}
