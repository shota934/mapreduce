/********************************************************************


   Copyright (C) 2017 Shota Hisai Released under the MIT license.


*********************************************************************/
#ifndef __INCLUDE__THREAD__
#define __INCLUDE__THREAD__
#include <queue>
#include <pthread.h>
#include "task.h"
#include "cond.h"
#include "state.h"
#include "threadpool.h"

using namespace std;
typedef class ThreadPool ThreadPool;

class Thread {
 private:
  int id;
  pthread_t thread;
  pthread_mutex_t *mutex;
  pthread_cond_t *cond;
  queue<Task *> *taskQueue;
  Task *task;
  State *state;
  ThreadPool *threadPool;

 public:
  Thread(int id,queue<Task *> *taskQueue,Condition *cond,ThreadPool *threadPool){
    this->id = id;
    this->taskQueue = taskQueue;
    this->mutex = cond->getMutex();
    this->cond = cond->getCond();
    this->state = cond->getState();
	this->threadPool = threadPool;
  }
  
  ~Thread (){ }
  int getId(){ return this->id; }
  pthread_t *getThreadInfo(){ return &this->thread;}
  void execute(Task *task);
  
  pthread_mutex_t *getMutex(){ return this->mutex; }
  pthread_cond_t *getCond(){ return this->cond; }
  queue<Task *> *getQueue(){ return taskQueue; }
  int lockMutex();
  int unlockMutex();
  ThreadPool *getThreadPool(){ return this->threadPool;}
};


#endif
