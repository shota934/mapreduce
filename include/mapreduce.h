/********************************************************************


   Copyright (C) 2017 Shota Hisai Released under the MIT license.


*********************************************************************/
#ifndef __INCLUDE__MAPREDUCE__
#define __INCLUDE__MAPREDUCE__
#include <iostream>
#include <list>
#include <map>
#include <typedef.h>
#include <threadpool.h>

using namespace std;
void init();
void printWordCont();

namespace mapreduce {
  typedef class InputData InputData;
  typedef class MapInput MapInput;
  
  class Intermediate{
  protected:
    string *key;
    object value;
  protected:
    Intermediate(){  }
  public:
    virtual ~Intermediate(){
      delete this->key;
    } 
    
  public:
    string *getKey(){
      return this->key;
    }
    object getValue(){
      return this->value;
    }
  };
  
  class Mapper {
  protected:
    Mapper(){ }
  public:
    virtual ~Mapper(){ }
    virtual list<Intermediate *> *map(MapInput *mapInput) = 0;
  };

  class Reducer {
  protected:
    Reducer(){ }
  public:
    virtual ~Reducer(){ }
    virtual void reduce(string *key,list<object> *lst) = 0;
  };
  
  class MapReduce {

  private:
    Mapper *mapper;
    Reducer *reducer;
    ThreadPool *mapThreadPool;
    ThreadPool *reduceThreadPool;
    
  public:
    MapReduce(Mapper *mapper,Reducer *reducer,int maxThreadNum);
    virtual ~MapReduce(){
      delete mapper;
      delete reducer;
      delete mapThreadPool;
      delete reduceThreadPool;
    }
    void execute(list<MapInput *> &lst);
  private:
    void sort(map<string,list<object> *> *hash,list<Intermediate *> *lst);
  };  

  class Splitter {
  protected:
    int diviedNum;
  protected:
    Splitter(){ 
      this->diviedNum = 0;
    }
  public:
    virtual ~Splitter(){ }
    virtual list<MapInput *> *split(InputData *input) = 0;
    void deleteMapInput(list<MapInput *> *lst);
    
    void setDiviedNum(int diviedNum){
      this->diviedNum = diviedNum;
      return;
    }
  };
  
  class InputData {
  protected:
    InputData(){ }
  public:
    ~InputData(){ }
  };
  
  class MapInput {
  private:
    int no;
  public:
    MapInput(int no){
      this->no = no;
    }
    virtual ~MapInput(){ }
  };
  
  class OutPutWriter {
  protected:
    OutPutWriter(){ }
  public:
    virtual ~OutPutWriter(){ }
    virtual void write() = 0;
  };
};

#endif
