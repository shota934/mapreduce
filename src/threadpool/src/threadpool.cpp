/********************************************************************


   Copyright (C) 2017 Shota Hisai. Released under the MIT license.


*********************************************************************/

#include "threadpool.h"
#include "thread.h"
#include "cond.h"
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

static void *threadEnctryFunc(void *args);
static void *threadPoolEntry(void *args);

ThreadPool *ThreadPool::createThreadPool(int maxThreadNum){
  return new ThreadPool(maxThreadNum);
}

ThreadPool::ThreadPool(int maxThreadNum){
  
  this->maxThreadNum = maxThreadNum;
  this->threadCnt = 0;
  this->state = ACTIVE;

  this->taskQueue = new queue<Task*>();
  this->taskPoolQueue = new queue<Task*>();

  //
  //
  // for worker threads
  pthread_cond_init(&(this->cond),NULL);
  pthread_mutex_init(&(this->mutex),NULL);
  //
  //


  //
  // for self
  pthread_cond_init(&(this->pool_cond),NULL);
  pthread_mutex_init(&(this->pool_mutex),NULL);
  pthread_create(&thread,NULL,&threadPoolEntry,this);
  //
  //
}

ThreadPool::~ThreadPool(){
  delete this->taskQueue;
  delete this->taskPoolQueue;
}

void ThreadPool::submitTask(Task *task){

  if(task == NULL){
    return;
  }
  
  pthread_mutex_lock(&(this->pool_mutex));
  taskPoolQueue->push(task);
  pthread_mutex_unlock(&(this->pool_mutex));
  pthread_cond_signal(&(this->pool_cond));
  
  return;
}

void ThreadPool::join(){

  pthread_join(thread,NULL);

  return;
}

void ThreadPool::shutdown(){

  
  pthread_mutex_lock(&(this->pool_mutex));  
  this->state = SHUTDOWN;
  pthread_mutex_unlock(&(this->pool_mutex));

  pthread_mutex_lock(&(this->mutex));
  pthread_cond_broadcast(&(this->cond));
  pthread_mutex_unlock(&(this->mutex));
  
  pthread_cond_signal(&(this->pool_cond));
  
  return;
}

void ThreadPool::createThread(int id,Task *task,Condition *cond){

  int ret;
  
  if(task == NULL){
    return;
  }

  Thread *thread = new Thread(id,taskQueue,cond,this);
  
  if(thread == NULL){
    return;
  }
  
  threadList.push_back(thread);
  ret = pthread_create(thread->getThreadInfo(),NULL,threadEnctryFunc,thread);
  
  return;
}

void ThreadPool::dispatchTask(Task *task){

  pthread_mutex_lock(&this->mutex);
  taskQueue->push(task);
  if(this->threadCnt < this->maxThreadNum){
    int id = this->threadCnt;
    this->threadCnt++;
    pthread_mutex_unlock(&this->mutex);
    Condition condition(&cond,&mutex,&state);
    createThread(id,task,&condition);
  } else {
    pthread_mutex_unlock(&this->mutex);
    pthread_cond_signal(&this->cond);
  }

  return;
}

pthread_cond_t *ThreadPool::getPoolCond(){
  return &this->pool_cond;
}

pthread_mutex_t *ThreadPool::getPoolMutex(){
  return &this->pool_mutex;
}

queue<Task *> *ThreadPool::getTaskQueue(){
  return this->taskPoolQueue;
}

list<Thread*> *ThreadPool::getThreadList(){
  return &this->threadList;
}

State ThreadPool::getState(){
  return state = this->state;
}

State ThreadPool::getStateWithSync(){
  pthread_mutex_lock(&(this->pool_mutex));
  state = this->state;
  pthread_mutex_unlock(&(this->pool_mutex));
  return state;
}

State *ThreadPool::getStateRef(){
  return &this->state;
}

static void *threadEnctryFunc(void *args){
  
  Thread *thread = (Thread *)args;
  Task *task;
  int id = thread->getId();
  queue<Task *> *taskQueue = thread->getQueue();
  ThreadPool *threadPool = thread->getThreadPool();
  
  while(true){
	
    thread->lockMutex();
    if((taskQueue->size() <= 0) && (ACTIVE == threadPool->getStateWithSync())){
      pthread_cond_wait(thread->getCond(),thread->getMutex());
    }
    
    if((taskQueue->size() <= 0) && (SHUTDOWN == threadPool->getStateWithSync())){
      thread->unlockMutex();
      break;
    }
	
    if(0 < taskQueue->size()){
      task = taskQueue->front();
      taskQueue->pop();
      thread->unlockMutex();
      thread->execute(task);
      delete task;
    } else {
      thread->unlockMutex();
    }
  }

  return NULL;
}

static void *threadPoolEntry(void *args){

  
  ThreadPool *threadPool = (ThreadPool*)args;
  Task *task;
  queue<Task *> *taskPoolQueue = threadPool->getTaskQueue();
  
  while(true){
    pthread_mutex_lock(threadPool->getPoolMutex());
    if((taskPoolQueue->size() <= 0) && (threadPool->getState() == ACTIVE)){
      pthread_cond_wait(threadPool->getPoolCond(),threadPool->getPoolMutex());
    }

    if((taskPoolQueue->size() <= 0) && (threadPool->getState() == SHUTDOWN)){
      pthread_mutex_unlock(threadPool->getPoolMutex());
      break;
    }

    if(0 < taskPoolQueue->size()){
      task = taskPoolQueue->front();
      taskPoolQueue->pop();
      pthread_mutex_unlock(threadPool->getPoolMutex());
      threadPool->dispatchTask(task);
    }
  }
  
  Thread *thread;
  list<Thread *> *list = threadPool->getThreadList();
  int size = (int)list->size();
  for(int i = 0; i < size; i++){
    thread = list->front();
    list->pop_front();
    pthread_join(*thread->getThreadInfo(),NULL);
    delete thread;
  }

  return NULL;
}
