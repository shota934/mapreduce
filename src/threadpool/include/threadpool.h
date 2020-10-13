/********************************************************************


   Copyright (C) 2017 Shota Hisai Released under the MIT license.


*********************************************************************/
#ifndef __INCLUDE__THREAD__POOL__
#define __INCLUDE__THREAD__POOL__
#include <queue>
#include <list>
#include <pthread.h>
#include "thread.h"
#include "task.h"
#include "state.h"
#include "cond.h"

using namespace std;

typedef class Thread Thread;

class ThreadPool {

 public:
  static ThreadPool *createThreadPool(int maxThreadNum);
 private:
  State state;
  int threadCnt;
  int maxThreadNum;

  pthread_t thread;
  pthread_cond_t pool_cond;
  pthread_mutex_t pool_mutex;

  pthread_cond_t cond;
  pthread_mutex_t mutex;
  queue<Task *> *taskPoolQueue;
  queue<Task *> *taskQueue;
  list<Thread*> threadList;
  
 private:
  ThreadPool(int maxThreadNum);
 public:
  virtual ~ThreadPool();
  void submitTask(Task *task);
  void shutdown();
  void join();
  State getState();
  State *getStateRef();
  State getStateWithSync();
  pthread_cond_t *getPoolCond();
  pthread_mutex_t *getPoolMutex();
  queue<Task *> *getTaskQueue();
  void dispatchTask(Task *task);
  list<Thread*> *getThreadList();
  
 private:
  void createThread(int id,Task *task,Condition *cond);
  void deleteThread();
   
};


#endif
