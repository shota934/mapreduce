/********************************************************************


   Copyright (C) 2017 Shota Hisai Released under the MIT license.


*********************************************************************/
#ifndef __INCLUDE__FILE__
#define __INCLUDE__FILE__
#include <iostream>
#include <stdio.h>
#include "typedef.h"

#define INIT_VALUE 0x00

using namespace std;

class File{
 private:
  string name;
  string mode;
  FILE *fp;
 public:
  File(string name,string mode){ 
    this->name = name;
    this->mode = mode;
  }
  ~File(){ }
  bool open();
  uchar *read();
  string *readAsString();
  void write(uchar *bytes,int size);
  unsigned long int getSize();
  void close();
  string *getName(){
    return &this->name;
  }
};

#endif



