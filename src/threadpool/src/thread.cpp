/********************************************************************


   Copyright (C) 2017 Shota Hisai. Released under the MIT license.


*********************************************************************/
#include <stdio.h>
#include "thread.h"
#include "cond.h"

void Thread::execute(Task *task){
  
  if(task != NULL){
    task->execute();
  }

  return;
}

int Thread::lockMutex(){

  int ret = 0;

  ret = pthread_mutex_lock(this->mutex);

  return ret;
}
  
int Thread::unlockMutex(){

  int ret = 0;
  
  ret = pthread_mutex_unlock(this->mutex);

  return ret;
}
