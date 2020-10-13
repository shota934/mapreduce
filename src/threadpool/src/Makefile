CXX=g++
OPT=-c -W
CC_LIB = ar rvs
INCLUDES=../include 
OBJ = thread.o threadpool.o
TAR = libthreadpool.a

thread_pool.a : ${OBJ}
	${CC_LIB} ${TAR} ${OBJ}

threadpool.o : threadpool.cpp 
	${CXX} ${OPT} threadpool.cpp -I ${INCLUDES}

thread.o : thread.cpp
	${CXX} ${OPT} thread.cpp -I ${INCLUDES}

clean :
	rm *.o
	rm *.a

