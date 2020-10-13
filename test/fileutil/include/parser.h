#ifndef __INCLUDE__PARSER__
#define __INCLUDE__PARSER__
#include <iostream>
#include <vector>

using namespace std;

class Parser{
 public:
  virtual vector<string *> *parce(string *name) = 0;
};
#endif
