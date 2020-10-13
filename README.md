
The MapReduce library
=====================

Foreword
--------

This document is intended for C++ programmer who wants to develop your MapReduce
program using this library.

Introduction
------------

This is a MapReduce library which can run on multi-thread environment.  
MapReduce is a programming model for large scale data processing  
which was developed by Google.
A user can define map and reduce function which is available   
to run on multi-core processor and implemented in C++.  
Supported platforms are OS-X(macOS) and Linux.  

Build
-----

  In order to build this library, you need to install the following compilers.  
  - gcc/g++
  - clang/clang++

  If you would like to debug your program based on this library by a debugger,  
  you also need to install that. e.g., gdb or lldb.  
  These debuggers depend on a compiler which you choose.  

  To build this library, simply type and run the following command, as usual.  
  Build this library with command as follows.

  make
  
  The "libmapreduce.a" and "libthreadpool.a" after created after execute this command,   
  Both files needs for building a mapreduce program, and  
  these has role as follows.  
  
  - libmapreduce.a  : has MapReduce class.
  - libthreadpool.a : has feature to run on multi-thread.

Test
----
  A test project for our MapReduce library is located in "test" directory.  
  For the convenience, I provide a word counting program as a test program.  
  To enther the directory and run the test program, use the following lines.  
  
  cd src/test/  
  make  
  ./wordcnt fileList  
  
  And each test project which is to build, run the make into test directory.  
  At this time, MapReduce library which is to build have already complete  
  The fileList has file names which stores input data for wordcnt program.  

How to embed MapReduce library in your program
----------------------------------------------
  You can build your program using this library. This section explains an instruction about this topic.  
  When you use the library, you must define sub-classes which inherits the following classes as follow.  

  Classes in "mapreduce.h"
  
  ---------------------------------------------------------------------------------------------------
  - Mapper        : The class for map phase.
  - Reducer       : The class for reduce phase.
  - Splitter      : The class for dividing input data.
  - InputData     : The class for intended data which this MapReduce library is processed.
  - MapInput      : The class for the mapper class is received data of class from mapreduce library.
  - Intermediate  : The class for saving data which after each mapper process.
  ---------------------------------------------------------------------------------------------------
  
  Note that you also need to implement a driver program for calling the MapReduce library.  
- How to obtain a final result (Optional)  
  If you want to write a result which is processed by MapReduce library into a file,   
  standard output of a console or the others,  
  you need to implement a class which inherits OutPutWriter class.  
  The input file of your program should be described a collection in a file.  

  If your program with this MapReduce library executes on parallel computation environment,  
  you need to specify a number of running thread in a constructor of MapReduce class.  

  In order to build your program with this MapReduce library, run the commands below.  
   
   $ command for the compiler your_program.cpp libmapreduce.a libthreadpool.a -o exec_file  
      
   - your_program.cpp : A driver program for building your program with this MapReduce library.
   - exec_file        : An output file.
   - -o               : The option to designate a name for an output file.
                        Both gcc and clang has the option.

   If you would like to execute your program on a Linux environment,  
   you need to add a compile option "-lpthread" when you compile your source codes.  

Author
------

  Shota Hisai

License
-------
	
  This library is released under the MIT License.  
  See LICENSE.

Reference
---------

  For to better understanding MapReduce, I have consult the following papers and implemented   
  the library based on them.

  - Jeffrey Dean, Sanjay Ghemawat
     MapReduce:Simplified Data Processing on Large Clusters

  - Colby Ranger, Ramanan Raghuraman, Arun Penmetsa, Gary Bradski, Christos Kozyrakis
     Evaluating MapReduce for Multi-core and Multiprocessor Systems
     
  Note that the first article explains a landmark work for the notion of MapReduce by the staff of Google.

Contact information
-------------------

  If you contact me, please send an e-mail as follows.  
  e-mail : shisai934Atgmail.com   
  I will be willing to dissucss with you.  
