#ifndef __INCLUDE__UTIL__
#define __INCLUDE__UTIL__
/********************************************************************


   Copyright (C) 2017 Shota Hisai Released under the MIT license.


*********************************************************************/
namespace wordcnt {
  class SymbolSeplarator{
  public:
    SymbolSeplarator(){ }
    ~SymbolSeplarator(){ }
    bool isSeparator(char ch);
    bool isTerminalCh(char ch);
  };
};

#endif
