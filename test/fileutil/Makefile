CC=g++
OPT=-c
CC_LIB = ar rvs

OBJ=file.o parsercsv.o
TAR_PARSER=libparsercsv.a
TAR_FILE=libfile.a
TARGET=*.a
INCLUDE = include

lib  : ${OBJ}
	${CC_LIB} ${TAR_PARSER} file.o
	${CC_LIB} ${TAR_FILE} parsercsv.o
	mkdir -p ../lib
	cp *.a ../lib

parsercsv.o : parsercsv.cpp
	${CC} ${OPT} parsercsv.cpp -I ${INCLUDE}

file.o : file.cpp
	${CC} ${OPT} file.cpp -I ${INCLUDE}


clean :
	rm *.a
	rm *.o

