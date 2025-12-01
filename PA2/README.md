# csce420-fall24
Author: Joyce Luo

Implementation Details:
This implementation of dpll is written in C++ and implemented iteratively instead of recursively in a while loop.
The associated variables and index values are stored in an unordered map. After retrieving the index from the unordered map,
the variable's truth value can be found in a truthValue vector.
The backtracking order/variable assignment order is kept in the backtrackOrder vector.
Any helper functions are commented in the dpll.cpp file.

DPLL structure:
The first variable in the unordered map is assigned with a truth value of 1 to initialize and jumpstart the dpll while loop.
First, a for loop checks each clause in the KB for satisfaction or conflicts.
Then, is a conflict is found, we backtrack using the backtrackOrder vector.
If no conflicts are found, we first try to assign a variable using the unit clause heuristic.
If a unit clause can't be found, we then find the first unassigned variable and randomly assign it true.
After these steps, the while loop continues for another iteration until a solution is found :D

To compile, the user should type: make
To run dpll alone, the user should type: ./dpll mapcolor.cnf OR ./dpll 6queens.cnf
To run dpll with a forced truth variable, the user should type: ./dpll mapcolor.cnf WAR OR ./dpll 6queens.cnf Q41
