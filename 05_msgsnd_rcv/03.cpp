#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <unistd.h>

struct st_msgbuf {
  long mtype;
  char mtext[32];
};

#define TEST02
void func01();
void func02();

int main(int argc, char *argv[]) {
#ifdef TEST01
  func01();
#endif

#ifdef TEST02
  func02();
#endif

  return 0;
}

/*! \fn func01
 * @brief brief comment
 * @detail detail comment
 */
void func01() {
  key_t key = ftok("/tmp/yqueue", 'z');
  int qid = msgget(key, IPC_CREAT | 0666);
  pid_t pid;

  if (fork() == 0) {
    // child
    pid = getpid();
    struct st_msgbuf msg = {pid, {0}};
    sleep(2);
    std::cout << "child: start of msgrcv"
              << "\n";
    int ret = msgrcv(qid, &msg, sizeof(msg.mtext), 0, MSG_NOERROR);
    std::cout << "child: end of msgrcv[" << ret << "]"
              << "\n";
  } else {
    // parent
    pid = getpid();
    struct st_msgbuf msg = {pid, "this is message"};
    std::cout << "parent: start of msgsnd"
              << "\n";
    int ret = msgsnd(qid, &msg, sizeof(msg.mtext), MSG_NOERROR);
    if (ret == 0) {
      std::cout << "parent: end of msgsnd"
                << "\n";
    }
  }
}

/*! \fn func02
 * @brief brief comment
 * @detail detail comment
 */
void func02() {
  key_t key = ftok("/tmp/yqueue", 'z');
  int qid = msgget(key, IPC_CREAT | 0666);
  pid_t pid;

  if (fork() == 0) {
    // child
    pid = getpid();
    struct st_msgbuf msg = {pid, {0}};
    std::cout << "child: start of msgrcv"
              << "\n";
    int ret = msgrcv(qid, &msg, sizeof(msg.mtext), 0, MSG_NOERROR);
    std::cout << "child: end of msgrcv[" << ret << "]"
              << "\n";
  } else {
    // parent
    pid = getpid();
    struct st_msgbuf msg = {pid, "this is message"};
    sleep(2);
    std::cout << "parent: start of msgsnd"
              << "\n";
    int ret = msgsnd(qid, &msg, sizeof(msg.mtext), MSG_NOERROR);
    if (ret == 0) {
      std::cout << "parent: end of msgsnd"
                << "\n";
    }
  }
}
