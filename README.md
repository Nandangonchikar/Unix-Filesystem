# Unix-Filesystem


Operating System(CS 5348) Project 2 Part 1

 Group 4

1. Chih-An Chang  - CXC210017
2. Nandan Gonchikar Shankarappa - NXG200043
3. Sayantan Kundu - SXK210048

This project is written in c++ and has the following functions:


The project has following files:
1. Source code [mod-v6.cc]
2. Header file [mod-v6.h]
3. Readme.md 


The project can be compiled and executed as mentioned below:

1. Compile the source code using a c++ compiler. g++ mod-v6.cc

2. Run the code. ./a.out

3. It will ask for a command.The valid commands are open, initfs, q (for quit) and print.

4. If a command other than the above is entered, it will throw as invalid command.



open:
 open Filename - opens the file Filename and returns an file identifier. If a file is not present it will create a new one.
 Example: open a.txt
 
Initfs:
 Initfs Blocknumber Inodenumber - Filesystem initialised with Blocknumber number of blocks, Inodenumber of inodes.
 
  
 Example: initfs 300 10
 
 Quit:
 q - Close the file and close the file system
 
 Example: q
 
 Print:
 print Blocknumber - print the contents of the Blocknumber to command window.
 
 Example: print 30
 




