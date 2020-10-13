#ifndef __INCLUDE__COND__
#define __INCLUDE__COND__
#include <pthread.h>
#include "state.h"

class Condition {
 private:
  pthread_cond_t *cond;
  pthread_mutex_t *mutex;
  State *state;
 public:
  Condition(pthread_cond_t *cond,pthread_mutex_t *mutex,State *state){
    this->cond = cond;
    this->mutex = mutex;
    this->state = state;
  }
  
  pthread_cond_t *getCond(){ return this->cond; }
  pthread_mutex_t *getMutex(){ return this->mutex; }
  State *getState(){ return this->state; }
};

#endif
