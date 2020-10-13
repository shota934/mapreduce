/********************************************************************


   Copyright (C) 2017 Shota Hisai Released under the MIT license.


*********************************************************************/
#include <vector>
#include <stdlib.h>
#include <string.h>
#include "parsercsv.h"
#include "file.h"

using namespace std;
string readOnlyBinaryMode = "rb";

vector<string *> *ParserCSV::parce(string *name){
  
  vector<string *> *vec;
  File file(*name,readOnlyBinaryMode);
  if(file.open()){
    unsigned long int size = file.getSize();
    uchar *bytes = file.read();
    vec = parce(bytes,size);
    delete bytes;
  } else {
    vec = new vector<string *>();
  }
  
  return vec;
}

vector<string *> *ParserCSV::parce(uchar *bytes,unsigned long int size){
 
  vector<string *> *vec = new vector<string *>();
  unsigned long int i = 0;
  while(i < size){
    string *str = new string();
    while(((bytes[i] != ',') && (bytes[i] != '\n')) && (i < size)){
      str->push_back((char)bytes[i]);
      i++;
    }
    if(str->length() != 0){
      vec->push_back(str);
    }
    i++;
  }

  return vec;
}
