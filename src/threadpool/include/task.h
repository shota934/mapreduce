#ifndef __INCLUDE__TASK__
#define __INCLUDE__TASK__

class Task {

 public:
  Task(){ }
  virtual ~Task(){ }
  virtual void execute() = 0;
};

#endif
