/********************************************************************


   Copyright (C) 2017 Shota Hisai. Released under the MIT license.


*********************************************************************/
#include <map>
#include <vector>
#include <wordcnt.h>
#include <mapreduce.h>
#include <util.h>
#include <file.h>

using namespace std;
using namespace mapreduce;
using namespace wordcnt;

map<string *,int> resultMap;
pthread_mutex_t resultMutex;

void createIterator(map<string,list<object> *> *hash,list<Intermediate *> *lst);
static Intermediate *emitIntermediate(string *key,int value);

void emit(string *key,int value);
void init();

list<Intermediate *> *WordCntMapper::map(MapInput *mapInput){
  
  WordCntMapInput *wcsd = (WordCntMapInput *)mapInput;
  SymbolSeplarator symbolSeplarator;
  string *text = wcsd->getData();
  list<Intermediate *> *lst = new list<Intermediate *>();
  string::iterator it;
  
  it = text->begin();
  string *word = new string();
  
  while(true){
    if(!symbolSeplarator.isSeparator((*it))){
      word->append(1,(*it));
    } else {
      if(word->length() != 0){
	Intermediate *intermediate = emitIntermediate(word,1);
	lst->push_back(intermediate);
	word = new string();
      }
    }
    
    if(symbolSeplarator.isTerminalCh((*it))){
      break;
    }
    ++it;
  }
  
  return lst;
}

void WordCntReducer::reduce(string *key,list<object> *lst){
  
  list<object>::iterator it;
  int result = 0;

  for(it = lst->begin(); it != lst->end(); ++it){
    int *val = (int*)(*it);
    result += *val;
    delete val;
  }
  
  emit(key,result);
  delete lst;
  
  return;
}

list<MapInput *> *WordCntSplitter::split(InputData *input){
  
  int no;
  int size;
  
  no = 0;
  WordCntInputData *wordCntInputData = (WordCntInputData *)input;
  list<MapInput *> *lst = new list<MapInput *>();
  vector<string *> *vec = wordCntInputData->getWordSet();
  vector<string *>::iterator itWordCntMapInput;
  size = vec->size();
  if(size <= 0){
    printf("No input files\nPlease set input files.\n");
    return lst;
  }
  
  for(itWordCntMapInput = vec->begin(); itWordCntMapInput != vec->end(); ++itWordCntMapInput){
    string *fileName = (string *)(*itWordCntMapInput);
    File file (*fileName,"rb");
    if(file.open()){
      string *input = file.readAsString();
      lst->push_back(new WordCntMapInput(input,no));
      file.close();
    } else {
      printf("This %s file does not exist.\n",file.getName()->c_str());
    }
  }
  
  return lst;
}

void init(){

  pthread_mutex_init(&resultMutex,NULL);
  
  return;
}

void emit(string *key,int value){
  
  pthread_mutex_lock(&resultMutex);
  
  resultMap.insert(pair<string *,int>(key,value));

  pthread_mutex_unlock(&resultMutex);

  return;
}

static Intermediate *emitIntermediate(string *key,int value){
  
  Intermediate *intermediate = new WordCntIntermediate(key,value);

  return intermediate;
}

void WordCntOutPutWriter::write(){

  map<string *,int>::iterator it;
  
  string str;
  string *key;
  int value;

  File outputFileFile("output.csv","wb");
  str.append("word  , cnt\n");
  for(it = resultMap.begin(); it != resultMap.end(); ++it){
    key = (*it).first;
    value = (*it).second;
    str.append(*key);
    str.append(",");
    str.append(std::to_string(value));
    str.append("\n");
  }
  outputFileFile.open();
  outputFileFile.write((uchar *)str.c_str(),str.length());
  outputFileFile.close();
  
  return;
}

void deleteResultMap(){

  map<string *,int>::iterator it;
  string *key;
  int value;

  for(it = resultMap.begin(); it != resultMap.end(); ++it){
    key = (*it).first;
    value = (*it).second;
    delete key;
  }
  
  resultMap.clear();
  
  return;
}
