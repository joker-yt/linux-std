#include <iostream>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

using namespace std;

// http://d.hatena.ne.jp/s-kita/20080713/1215940088
mqd_t g_mqd;
struct mq_attr g_attr;

void sigHandler(int sig) {
  cout << __FUNCTION__ << ": receive signal!!! -> " << strsignal(sig) << endl;
}

void child(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  // sigprocmask(SIG_BLOCK, &set, NULL);

  struct sigaction new_act, old_act;
  sigemptyset(&new_act.sa_mask);
  new_act.sa_flags = 0;
  new_act.sa_handler = sigHandler;
  if (sigaction(SIGUSR1, &new_act, &old_act)) {
    cerr << __FUNCTION__ << ": sigaction error -> " << strerror(errno) << endl;
  }

  sigevent ev;
  ev.sigev_notify = SIGEV_SIGNAL;
  ev.sigev_signo = SIGUSR1;
  if (mq_notify(g_mqd, &ev)) {
    cerr << __FUNCTION__ << ": mq_notify error -> " << strerror(errno) << endl;
  }

  sigemptyset(&set);

  int i = 5;
  while (i--) {
    cout << __FUNCTION__ << ": start suspending!" << endl;
    sigsuspend(&set);

    char buf[32];
    memset(buf, 0, sizeof(buf));
    ssize_t sz = mq_receive(g_mqd, buf, g_attr.mq_msgsize, 0);
    if (sz < 0) {
      cerr << __FUNCTION__ << ": receive error -> " << strerror(errno) << endl;
    }
    cout << __FUNCTION__ << ": msg(" << sz << ")[" << buf << "]" << endl;

    mq_notify(g_mqd, &ev);
  }

  cout << __FUNCTION__ << ": end" << endl;
}

void parent(pid_t pid) {
  cout << __FUNCTION__ << ": start" << endl;

  char buf[32] = "message?";
  size_t len = strlen(buf);
  int i = 5;
  while (i--) {
    buf[len - 1] = static_cast<char>(static_cast<int>('0') + i);
    cout << __FUNCTION__ << ": send message[" << buf << "]" << endl;
    if (mq_send(g_mqd, buf, g_attr.mq_msgsize, 0)) {
      cerr << __FUNCTION__ << ": mq_send error -> " << strerror(errno) << endl;
    }
    sleep(2);
  }

  int status;
  waitpid(pid, &status, WNOHANG);
  if (WIFSIGNALED(status)) // child received signal or not
    cout << __FUNCTION__ << ": child process received signal.."
         << strsignal(WTERMSIG(status)) << endl;
  else {
    if (WIFEXITED(status))
      cout << __FUNCTION__ << ": child process exit normally" << endl;
    else
      cout << __FUNCTION__ << ": child process status?? -> " << status << endl;
  }

  cout << __FUNCTION__ << ": end" << endl;
}

int main(int argc, char *argv[]) {
  g_mqd = mq_open("/test", O_RDWR | O_CREAT | O_EXCL, 0666, NULL);
  if (g_mqd < 0) {
    cerr << __FUNCTION__ << ": mq_open error -> " << strerror(errno) << endl;
  }

  mq_getattr(g_mqd, &g_attr);
  cout << __FUNCTION__ << ": msgmax -> " << g_attr.mq_maxmsg << " msgsize ->  "
       << g_attr.mq_msgsize << endl;

  pid_t pid = fork();
  if (pid == 0) {
    child(pid);
  } else {
    cout << __FUNCTION__ << ": child's pid -> " << pid << endl;
    sleep(3);
    parent(pid);

    cout << __FUNCTION__ << ": start unlink " << pid << endl;
    if (mq_unlink("/test")) {
      cerr << __FUNCTION__ << ": unlink..err " << strerror(errno) << endl;
    }
    cout << __FUNCTION__ << ": unlink..end " << pid << endl;
  }
  return 0;
}
