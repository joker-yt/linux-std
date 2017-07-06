#include <iostream>
#include <unistd.h>
#define __PATTERN3__

//====================================================
void *thread_func1_1(void *arg);
void func1_1() {
  pthread_t tid;
  if (0 != pthread_create(&tid, NULL, thread_func1_1, NULL)) {
    std::cout << "create error"
              << "\n";
  }

  int *pRet = nullptr;
  pthread_join(tid, (void **)&pRet);
  std::cout << "have joined!"
            << "\n";
  return;
}
void *thread_func1_1(void *arg) {
  std::cout << __FUNCTION__ << " started! sleeping...."
            << "\n";

  sleep(3);

  std::cout << __FUNCTION__ << " waked up! return!"
            << "\n";
  return nullptr;
}
//====================================================
void *thread_func2_1(void *arg);
pthread_mutex_t mutex2_1 = PTHREAD_MUTEX_INITIALIZER;
void func2_1() {
  pthread_t tid;

  if (0 != pthread_create(&tid, NULL, thread_func2_1, NULL)) {
    std::cout << "create error"
              << "\n";
  }
  sleep(1);

  pthread_mutex_lock(&mutex2_1);

  std::cout << __FUNCTION__ << " got lock!"
            << "\n";

  sleep(1);
  std::cout << __FUNCTION__ << " unlock!"
            << "\n";
  pthread_mutex_unlock(&mutex2_1);

  int *pRet = nullptr;
  pthread_join(tid, (void **)&pRet);
  std::cout << "have joined!"
            << "\n";
}
void *thread_func2_1(void *arg) {
  std::cout << __FUNCTION__ << " started!"
            << "\n";

  pthread_mutex_lock(&mutex2_1);

  std::cout << __FUNCTION__ << " got lock!"
            << "\n";

  sleep(1);

  std::cout << __FUNCTION__ << " unlock!"
            << "\n";
  pthread_mutex_unlock(&mutex2_1);

  return nullptr;
}
//====================================================
void *thread_func3_1(void *arg);
pthread_mutex_t mutex3_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond3_1 = PTHREAD_COND_INITIALIZER;
void func3_1() {
  pthread_t tid;

  if (0 != pthread_create(&tid, nullptr, thread_func3_1, nullptr)) {
    std::cout << "create error"
              << "\n";
  }
  std::cout << __FUNCTION__ << " created thread!" << std::endl;

  sleep(1);
  pthread_mutex_lock(&mutex3_1);
  std::cout << __FUNCTION__ << " got lock!"
            << "\n";

  std::cout << __FUNCTION__ << " got lock and signal!" << std::endl;

  pthread_cond_signal(&cond3_1);

  sleep(5);

  pthread_mutex_unlock(&mutex3_1);
  std::cout << __FUNCTION__ << " unlock!"
            << "\n";

  int *pRet = nullptr;
  pthread_join(tid, (void **)&pRet);
  std::cout << "have joined!"
            << "\n";

  return;
}
void *thread_func3_1(void *arg) {
  std::cout << __FUNCTION__ << " start!" << std::endl;
  pthread_mutex_lock(&mutex3_1);
  std::cout << __FUNCTION__ << " got lock!"
            << "\n";

  std::cout << __FUNCTION__ << " cond_wait!" << std::endl;
  pthread_cond_wait(&cond3_1, &mutex3_1);
  std::cout << __FUNCTION__ << " receive signal!" << std::endl;

  pthread_mutex_unlock(&mutex3_1);
  std::cout << __FUNCTION__ << " unlock!"
            << "\n";

  return nullptr;
}
//====================================================

//====================================================

int main(int argc, char *argv[]) {
#ifdef __PATTERN1__
  func1_1();
#endif

#ifdef __PATTERN2__
  func2_1();
#endif

#ifdef __PATTERN3__
  func3_1();
#endif

  return 0;
}
