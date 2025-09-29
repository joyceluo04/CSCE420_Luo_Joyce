# csce420-fall24
How does my blocksworld.cpp work?
Command Line Instructions: 
Compile: type in make and it should compile blocksworld.cpp with a g++ compiler
Run: ./blocksworld problemXXX.bwp [h0,h1]
    -replace XXX with the problem number (A03, A04 etc.)
    -[h0,h1] is the second argument used to simulate either BFS or my heuristic
        -type in h0 for BFS, type in h1 to use the heuristic
Constraints/Limitations:
1. Currently the initialization of stacks is hard coded to only work for either 3 stack or 5 stack problems. My program will suffice for the current set of problems A03-B20, however it does not support the calculation of any other stack sizes.
2. My queue is sorted using a comparator and may slow down as queue sizes get bigger/problems get harder
3. The parent of each successor is stored in a parents vector which will increase in size as problems get harder
