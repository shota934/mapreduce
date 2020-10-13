/********************************************************************


   Copyright (C) 2017 Shota Hisai. Released under the MIT license.


*********************************************************************/
#include <vector>
#include <string>
#include <mapreduce.h>
#include <wordcnt.h>
#include <file.h>
#include <parsercsv.h>

using namespace std;
using namespace mapreduce;

const int numOfProc = 4;

int main(int argc,char *argv[]){
  
  vector<string *> *vec;
  list<MapInput *> *lst;
  
  if(argc <= 1){
    cout<<"Input File not found"<<endl;
    return -1;
  }

  string *str = new string(argv[1]);
  Parser *parcer = new ParserCSV();
  WordCntInputData *inputData = new WordCntInputData();
  OutPutWriter *outPutWriter = NULL;
  Mapper *mapper = new WordCntMapper();
  Reducer *reducer = new WordCntReducer();
  Splitter *splitter = new WordCntSplitter();
  MapReduce *mapReduce = new MapReduce(mapper,reducer,numOfProc);
  init();
  outPutWriter = new WordCntOutPutWriter();
  vec = parcer->parce(str);
  if(0 < (int)vec->size()){
    inputData->setWordSet(vec);
    lst = splitter->split(inputData);
    mapReduce->execute(*lst);
    if(outPutWriter != NULL){
      outPutWriter->write();
      delete outPutWriter;
    }
  } else {
    printf("No input files\nPlease set input files.\n");    
  }
  
  delete vec;
  delete lst;
  deleteResultMap();
  delete mapReduce;
  
  return 0;
}
