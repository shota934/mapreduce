CXX=g++
OPT=-c -W
OBJ=mapreduce.o
INCLUDES = ../include
TARGET=libmapreduce.a
SUB_LIB=libthreadpool.a
LIBPATH=../lib
AR=ar 
RCS=rcs

SUB_PRJ_SRC=threadpool/src/
SUB_DIR_INCLUDE=threadpool/include/
SUB_PRJ=threadpool

mapreduce : ${OBJ}
	${AR} ${RCS} ${TARGET} ${OBJ}
	mkdir -p ${LIBPATH}
	cp ${TARGET} ${LIBPATH}
	cp ${SUB_PRJ_SRC}${SUB_LIB} ${LIBPATH}

mapreduce.o : mapreduce.cpp
	make -C ${SUB_PRJ_SRC}
	${CXX} ${OPT} mapreduce.cpp -I ${INCLUDES} -I ${SUB_DIR_INCLUDE}

clean : 
	rm -f *.o
	rm -f *.a
	make clean -C ${SUB_PRJ_SRC}
	rm -r -f ${LIBPATH}