/********************************************************************


   Copyright (C) 2017 Shota Hisai. Released under the MIT license.


*********************************************************************/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <map>
#include <mapreduce.h>
#include <task.h>

using namespace std;
using namespace mapreduce;

void createIterator(map<string,list<object> *> *hash,list<Intermediate *> *lst);

class MapEngine : public Task {
  
private:
  MapInput *mapInput;
  Mapper *mapper;
  int *taskCnt;
  ThreadPool *threadpool;
  pthread_mutex_t *mutex;
  list<list<Intermediate *> *> *intermediateList;

public:
  MapEngine(Mapper *mapper,MapInput *mapInput){
    this->mapper = mapper;
    this->mapInput = mapInput;
  }
  
  virtual ~MapEngine(){ }
  
  void execute(){
    
    list<Intermediate *> *lst;    
    lst = this->mapper->map(this->mapInput);
    pthread_mutex_lock(mutex);
    this->intermediateList->push_back(lst);
    delete this->mapInput;
    (*taskCnt)--;
    if(*taskCnt <= 0){
      threadpool->shutdown();
    }
    pthread_mutex_unlock(mutex);
    
    return;
  }

  void setThreadPool(ThreadPool *threadpool){ 
    this->threadpool = threadpool; 
    return;
  }
  
  void setMutex(pthread_mutex_t *mutex){
    this->mutex = mutex;
    return;
  }

  void setTaskCnt(int *taskCnt){
    this->taskCnt = taskCnt;
    return;
  }

  void setIntermediateList(list<list<Intermediate *> *> *intermediateList){
    this->intermediateList = intermediateList;
    return;
  }
};

class ReduceEngine : public Task {

private:
  string *key;
  list<object> *lst;
  Reducer *reducer;
  ThreadPool *threadpool;
  int *taskCnt;
  pthread_mutex_t *mutex;
  
public:
  ReduceEngine(string *key,list<object> *lst,Reducer *reducer){ 
    this->key = key;
    this->lst = lst;
    this->reducer = reducer;
  }
  
  virtual ~ReduceEngine(){ }
  void execute(){

    this->reducer->reduce(this->key,this->lst);
    pthread_mutex_lock(mutex);
    (*taskCnt)--;
    if(*taskCnt <= 0){
      threadpool->shutdown();
    }
    pthread_mutex_unlock(mutex);

    return;
  }

  void setThreadPool(ThreadPool *threadpool){ 
    this->threadpool = threadpool; 
    return;
  }

  void setMutex(pthread_mutex_t *mutex){
    this->mutex = mutex;
    return;
  }

  void setTaskCnt(int *taskCnt){
    this->taskCnt = taskCnt;
    return;
  }

};

MapReduce::MapReduce(Mapper *mapper,Reducer *reducer,int maxThreadNum){

  this->mapper = mapper;
  this->reducer = reducer;
  this->mapThreadPool = ThreadPool::createThreadPool(maxThreadNum);
  this->reduceThreadPool = ThreadPool::createThreadPool(maxThreadNum);
}

void MapReduce::execute(list<MapInput *> &lst){
  
  list<list<Intermediate *> *> intermediateList;
  int size;
  list<MapEngine *> mapEngineList;
  map<string,list<object> *> hash;
  pthread_mutex_t mutex;
  int taskCnt;
  MapEngine *me;
  list<list<Intermediate *> *>::iterator itIntermediateList;
  list<MapInput *>::iterator it;
  map<string,list<object> *>::iterator itHash;
  
  size = lst.size();
  
  pthread_mutex_init(&mutex,NULL);
  if(size <= 0){
    printf("No input files\nPlease set input files.\n");
    return;
  }

  if(!this->mapper){
    printf("There is no mapper object in mapreduce object.\n");
    return;
  }

  if(!this->reducer){
    printf("There is no reducer obejct in mapreduce obejct.\n");
    return;
  }

  
#ifdef __DEBUG__
  printf("map phase\n");
#endif
  taskCnt = 0;
  for(it = lst.begin(); it != lst.end(); ++it){
    me = new MapEngine(this->mapper,(MapInput *)(*it));
    me->setTaskCnt(&taskCnt);
    me->setMutex(&mutex);
    me->setIntermediateList(&intermediateList);
    me->setThreadPool(this->mapThreadPool);
    taskCnt++;
    mapThreadPool->submitTask(me);
  }
  mapThreadPool->join();
  
#ifdef __DEBUG__
  printf("shuffle phase\n");
#endif
  for(itIntermediateList = intermediateList.begin(); itIntermediateList != intermediateList.end(); ++itIntermediateList){
    list<Intermediate *>  *intermediate = (list<Intermediate *> *)(*itIntermediateList);
    this->sort(&hash,intermediate);
    delete intermediate;
  }

#ifdef __DEBUG__
  printf("reduce phase\n");
#endif
  pthread_mutex_init(&mutex,NULL);
  taskCnt = 0;
  
  for(itHash = hash.begin(); itHash != hash.end(); ++itHash){
    string *key = (string *)(&(*itHash).first);
    list<object> *iterator = (*itHash).second;
    ReduceEngine *reduceEngine = new ReduceEngine(new string(*key),iterator,this->reducer);
    reduceEngine->setThreadPool(this->reduceThreadPool);
    reduceEngine->setTaskCnt(&taskCnt);
    reduceEngine->setMutex(&mutex);
    pthread_mutex_lock(&mutex);
    taskCnt++;
    pthread_mutex_unlock(&mutex);
    reduceThreadPool->submitTask(reduceEngine);
  }
  reduceThreadPool->join();

  return;
}

void MapReduce::sort(map<string,list<object> *> *hash,list<Intermediate *> *lst){
  
  list<Intermediate *>::iterator it;
  int result = 0;
  list<object> *tmpLst;
  
  for(it = lst->begin(); it != lst->end(); ++it){
    Intermediate *intermediate = ((Intermediate *)*it);
    if(hash->find(*intermediate->getKey()) != hash->end()){
      tmpLst = (*hash)[(*intermediate->getKey())];
      tmpLst->push_back(intermediate->getValue());
    } else {
      tmpLst = new list<object>();
      tmpLst->push_back(intermediate->getValue());
      hash->insert(pair<string,list<object> *>((*intermediate->getKey()),tmpLst));
    }
    delete intermediate;
  }
  
  return;
}
