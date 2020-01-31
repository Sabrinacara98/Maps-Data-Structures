In this code, I place a couple of mathematical operators in a grid and perform specified operations on the placed opeartors. 
The size of the grid as well as size and position of the mathematical operators must be read from a text file (e.g., “grid.txt”)
and the operations that will be performed should be read from another text file (e.g., “instructions.txt”)  
The details are as follows: 

Inıtializing The Grid (grid.txt) 

1. The grid consists of N rows and M columns which are specified in the first line of the grid.txt file, 
as examplified in Figure 1. 
Note that row/column numbers start from 1 and not from 0. 

2. The operators are added to the grid by specifying its type (+, -, x, /),
its center point (row and column in the grid), and its size (a number in the interval [1..9]).  

3. When you place an operator, you have to check whether the operator fits into the grid or not. 
There is a BORDER ERROR when at least one cell of the operator is placed outside of the grid. 

4. When an error  (BORDER and/or CONFLICT error) occured in placing the operators, 
you skip the placement and continue with the next placement (if any) specified in the grid.txt file.

Instructions on The Placed Operators (instructions.txt) 

1.  Move  Right (MVR) instruction moves the center of the operator by a specified amount to the right 
(i.e., it increments column part of the center location). 
Its format is as follows: MVR row column move_by where row and column specifies a cell on the grid on which a part of the operator resides. 
The cell (row, column) MUST NOT be the center of the operator. You must first find the center, and move the operator by the amount move_by 
 
 