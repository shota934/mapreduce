CC=g++
OPT=-c -std=c++11
LIBDIR=-L ../lib  -L lib/
LIBS=-lfile -lparsercsv -lmapreduce -lthreadpool -lpthread
INCLUDE=-I include/ -I ../include/  -I fileutil/include/  -I ../src/threadpool/include
OBJ=wordcnt.o util.o main.o
SUB_PRJ_SRC=fileutil

TARGET=wordcnt

wordcnt : ${OBJ}
	make -C ${SUB_PRJ_SRC}
	${CC} ${OBJ} -o ${TARGET} ${LIBDIR} ${LIBS} ${INCLUDE}

wordcnt.o : wordcnt.cpp
	${CC} ${OPT} wordcnt.cpp ${INCLUDE}

main.o  : main.cpp 
	${CC} ${OPT} main.cpp ${INCLUDE}

util.o : util.cpp
	${CC} ${OPT} util.cpp ${INCLUDE}

clean:
	rm -f *.o
	make clean -C ${SUB_PRJ_SRC}
	rm ${TARGET}
	rm -r -f lib

test :
	./${TARGET} fileList
