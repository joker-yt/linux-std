#include <iostream>
#include <mqueue.h>
#include <signal.h>

// http://d.hatena.ne.jp/s-kita/20080713/1215940088
struct sigevent &create_ev() {
  static sigevent ev;
  ev.sigev_notify = SIGEV_SIGNAL;
  ev.sigev_signo = SIGUSR1;

  return ev;
}

int main(int argc, char *argv[]) { return 0; }
