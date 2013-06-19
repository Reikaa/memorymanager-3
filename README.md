Memory Manager
=============

This is a pretty basic memory manager, which checks for memory leaks in a c++ programm. 
To use it, it must be inluded in the c++ project and the it must be compiled with de debug flag, no includes are needed.

## Memory Statistics
It will then colled statistics during the execution of the program and print statistics at the end. 
It will discover wrong deletes, leaked memory, the amount of leaked memory and total used memory. <br />
Example:
<pre>
----------------------------------------
Memory Statistics
----------------------------------------

#  operator     size address     memory 
----------------------------------------
1  new[]         100 0x100103b80    100
2  new             4 0x100103c30    104
3  new[]         400 0x100103c80    504
4  new             8 0x100103e50    512
5  new[]         800 0x100103ea0   1312
6  new[]        2408 0x100803200   3720
7  delete          8 0x100103b80   3620 *** wrong delete ***
8  delete          8 0x100103e50   3612
9  delete[]        8 0x100103ea0   2812
10 delete[]        8 0x100103c80   2412

Memory Leaks
----------------------------------------
2  new             4 0x100103c30      4
6  new[]        2408 0x100803200   2412

Memory Usage
----------------------------------------
No. of leaks:        2
Max used memory:     3720
Leaked memory:       2412

</pre>

## Line Numbers
When a leak is detected, it would be helpfull to get the line number, where the memory was allocated. 
To get the line numbers, the header `MemMngr.h` must be included in the according file. 
It will then print the file name and line number, where the memory which was not deallocated, was allocated.<br />
Example:
<pre>
----------------------------------------
Memory Statistics
----------------------------------------

#  operator     size address     memory 
----------------------------------------
1  new[]         100 0x100103b80    100
2  new             4 0x100103c30    104
3  new[]         400 0x100103c80    504
4  new             8 0x100103e50    512
5  new[]         800 0x100103ea0   1312
6  new[]        2408 0x100803200   3720
7  delete          8 0x100103b80   3620 *** wrong delete *** /Users/robin/Development/Xcode/MemoryManager/MemoryManager/main.cpp:32
8  delete          8 0x100103e50   3612
9  delete[]        8 0x100103ea0   2812
10 delete[]        8 0x100103c80   2412

Memory Leaks
----------------------------------------
2  new             4 0x100103c30      4 /Users/robin/Development/Xcode/MemoryManager/MemoryManager/main.cpp:24
6  new[]        2408 0x100803200   2412 /Users/robin/Development/Xcode/MemoryManager/MemoryManager/main.cpp:29

Memory Usage
----------------------------------------
No. of leaks:        2
Max used memory:     3720
Leaked memory:       2412
</pre>



