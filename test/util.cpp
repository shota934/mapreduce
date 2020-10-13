/********************************************************************


   Copyright (C) 2017 Shota Hisai. Released under the MIT license.


*********************************************************************/
#include <util.h>

using namespace wordcnt;
bool SymbolSeplarator::isSeparator(char ch){

  bool flag = false;
  if((ch == 0x20) || (ch == 0x0A) 
     || (ch == 0x00) || (ch == 0x2e) || 
     (ch == 0x2c)){
    flag = true;
  }

  return flag;
}

bool SymbolSeplarator::isTerminalCh(char ch){
  return !ch;
}
