/********************************************************************


   Copyright (C) 2017 Shota Hisai Released under the MIT license.


*********************************************************************/
#ifndef __INCLUDE__CSV__PARSER__
#define __INCLUDE__CSV__PARSER__
#include "parser.h"
#include "typedef.h"

using namespace std;

class ParserCSV : public Parser {

 public:
  ParserCSV(){ }
  ~ParserCSV(){ }
  vector<string *> *parce(string *name);
  vector<string *> *parce(uchar *bytes,unsigned long int size);
};

#endif
