#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
#include <iomanip>
#include <errno.h>

const char *TestDirName = "/test_dir";
const int BUF_MAX = 128;
int main(int argc, char *argv[]) {
  int fd;

  fd = inotify_init();
  if (fd == -1) {
    std::cerr << "err:inotify_init"
              << "\n";
    return -1;
  }

  char *test_path;
  {
    char *current_dir = getcwd(NULL, 0);
    if (current_dir == NULL) {
      std::cerr << "err:getcwd"
                << "\n";
      return -1;
    }

    size_t path_len;
    path_len = strlen(current_dir) + 1; // "+1" added for "\0"
    path_len += strlen(TestDirName);

    size_t path_area_sz = sizeof(char) * path_len;
    test_path = (char *)malloc(path_area_sz);
    sprintf(test_path, "%s%s", current_dir, TestDirName);
  }

  int ret = inotify_add_watch(fd, test_path, IN_ALL_EVENTS);
  if (ret == -1) {
    std::cerr << "err:inotify_add_watch"
              << "\n";
    return -1;
  }

  char buf[BUF_MAX];
  ssize_t len;
#if 0
  char *pbuf = &buf[0];
  size_t buf_sz = sizeof(struct inotify_event);
#endif
  while (1) {
// if read retunrs error once, after that inotify doesn't work successfully
#if 1
    memset(buf, 0, sizeof(BUF_MAX));
    len = read(fd, buf, BUF_MAX);
#else
    memset(pbuf, 0, buf_sz);
    len = read(fd, pbuf, buf_sz);
    if (len == -1) {
      // if size of buffer underrun compared with that of event,
      // read returns "Invalid argument".
      std::cout << "read:" << strerror(errno) << "\n";
      buf_sz = sizeof(char) * BUF_MAX;
      pbuf = (char *)malloc(buf_sz);
      if (pbuf == NULL) {
        std::cerr << "malloc for read"
                  << "\n";
        return -1;
      }
    }
#endif
    int i = 0;
    while (i < len) {
      struct inotify_event *event = (struct inotify_event *)&buf[i];

      std::cout << "-----------" << std::endl;
      std::cout << "wd :" << event->wd << "\n";
      std::cout << "mask :" << std::hex << event->mask << "\n";
      std::cout << "cookie :" << event->cookie << "\n";
      std::cout << "len :" << event->len << "\n";
      std::cout << "dir :" << ((event->mask & IN_ISDIR) ? "yes" : "no") << "\n";
      if (event->len) {
        std::cout << "name :" << event->name << "\n";
      }
      std::cout << "-----------" << std::endl;
      i += sizeof(struct inotify_event) + event->len;
    }
  }

  close(fd);
  return 0;
}
