/********************************************************************


   Copyright (C) 2017 Shota Hisai Released under the MIT license.


*********************************************************************/
#include <string.h>
#include "file.h"

const int lf_len = 1;

bool File::open(){

  bool flag = false;
  fp = fopen(this->name.c_str(),this->mode.c_str());
  if(fp != NULL){
    flag = true;
  }

  return flag;
}

uchar *File::read(){

  uchar *byte;
  int size = getSize();
  byte = new uchar[size];
  memset(byte,INIT_VALUE,size);
  fread(byte,sizeof(uchar),size,fp);

  return byte;
}

string *File::readAsString(){
  
  char *line;
  string *str;
  size_t len;
  
  line = NULL;
  len = 0;
  str = new string();
  for(; getline(&line,&len,fp) > 0; line = NULL){
    str->append(line);
    delete line;
  }
  
  return str;
}

void File::write(uchar *bytes,int size){

 fwrite(bytes,sizeof(uchar),size,fp);
  
  return;
}

unsigned long int File::getSize(){

  unsigned long int size;
  fseek(fp,0, SEEK_SET);
  fseek(fp,0,SEEK_END);
  size = ftell(fp);
  fseek(fp,0, SEEK_SET);

  return size;
}

void File::close(){

  fclose(fp);

  return;
}
