# Operating-Systems
In this projects I use fork,mmap,pipe.

Operating-Systems
In this project, i check sudoku by create three child and each child responsable of check rowcloumnmatrix 33.

The main process create three child and for each child create pipe,in addition one pipe for all. 
Three pipe to read the sudoku and other to return if sudoku is legal. 
When the father do fork the child start perform two thing  move the FD of read to STDIN and write to STDOUT,
the second close the FD will not be used. And after that the child execute the program by execv. 
Also father close the FD will not be used and wait the child write to pipe and read the answer.

explain of file execveAndPipes-this is the main procees(father).
execvColumn-child that check the legal of column. 
execveLines-child that check the legal of line. 
execveMatrix-child that check the legal of nine matrix(33). 
demo.txt- legal sudoku. 
wrong1 wrong2 wrong3 - wrong sudoku.

v1 -is make file for this program.

I create secnod program that check sudoku but other way. 
In mmapSecondCheck.c i share memory by create struct that contain solution and status. 
solution is the sudoku i read from file. 
status is the result of check sudoku and each child write Permanent position. 
The three child that check the sudoku same way.

v2 -is make file for this program.
