# Several-page-replacement-algorithms

## Problem Statement:
### It is required to simulate some of paging replacement algorithms. The required algorithms to be implemented are as follows:
• Optimal<br>
• First In First Out (FIFO)<br>
• Least Recently Used (LRU)<br>
• Clock
## Input/Output:
Your input will be from standard input. Your output will be from standard output. 
The input will contain the number of pages allocated to the process, the simulated algorithm (OPTIMAL, FIFO, LRU, or CLOCK), and then a sequence of page references like below:<br>
3<br>
FIFO<br>
5<br>
12<br>
5<br>
2<br>
4<br>
2<br>
5<br>
……<br>
-1<br>
#### Note:  The last line in the input is -1 (and is ignored)<br>
### For each run, you should print the following:<br>
1. A trace recording page faults for each memory reference in the sequence.<br>
2. Counter recording total page faults.<br>
#### We will have the following results (notice the two-digit page numbers):
![](output.PNG)
