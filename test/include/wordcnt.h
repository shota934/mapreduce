/********************************************************************


   Copyright (C) 2017 Shota Hisai Released under the MIT license.


*********************************************************************/
#ifndef  __INCLUDE__WORD_CNT__
#define  __INCLUDE__WORD_CNT__
#include <vector>
#include <mapreduce.h>

using namespace mapreduce;

void deleteResultMap();

class WordCntMapper : public Mapper {
  
 public:
  WordCntMapper(){ }
  virtual ~WordCntMapper(){ }

  list<Intermediate *> *map(MapInput *mapInput);
};

class WordCntReducer : public Reducer{
  
 public:
  WordCntReducer(){ }
  ~WordCntReducer(){ }
  void reduce(string *key,list<object> *lst);
};

class WordCntIntermediate : public Intermediate {
 public:
  WordCntIntermediate(string *key,int value){
    this->key = key;
    this->value = new int(value);
  }
  virtual ~WordCntIntermediate() { 
    
  }
};

class WordCntSplitter : public Splitter {
  
 public:
  WordCntSplitter(){ }
  virtual ~WordCntSplitter(){ }
  virtual list<MapInput *> *split(InputData *input);
};

class WordCntMapInput : public MapInput{
  
 private:
  string*data;
 public:
 WordCntMapInput(string *data,int no) : MapInput(no){ 
    this->data = data;
  }
  virtual ~WordCntMapInput(){
    delete this->data;
 }
 string *getData(){ return this->data; }
};

class WordCntOutPutWriter : public OutPutWriter {
 public:
  WordCntOutPutWriter(){ }
  virtual ~WordCntOutPutWriter(){ }
  void write();

};

class WordCntInputData : public InputData {
 private:
  vector<string *>*vec;
 public:
  WordCntInputData(){ }
  virtual ~WordCntInputData(){
    delete this->vec;
  }
  
  void setWordSet(vector<string *>*vec){ 
    this->vec = vec;
    return;
  }

  vector<string *>*getWordSet(){
    return this->vec;
  }
};
#endif
