# Matrix Multiplication (Multi-Threading)

## Overview

The input to the program is two matrixes A(x*y) and B(y*z) that are read from corresponding text files. The output is a matrix C(x*z) that is written to an output text file.

A parallelized version of matrix multiplication can be done using one of these three methods:

1. A thread computes the output C matrix i.e. without multi-threading. (A thread per matrix).
2. A thread computes each row in the output C matrix. (A thread per row).
3. A thread computes each element in the output C matrix. (A thread per element).

## Requirements

* Implement the multi-threaded matrix multiplication using all three methods described above.
* Compare the three implementations according to the following:
    1. Number of threads created.
    2. Execution time taken.

Your programs should do the following:

* Your program is executed as: ./matMultp Mat1 Mat2 MatOut, where Mat1 and Mat2 are the names of the text files to read the first and second matrices, respectively, and MatOut is the prefix of the text files names to write the output matrices (each file represents one method). If the user does not enter this information, the default is a.txt and b.txt, for input matrixes A and B, and c for output matrices (of all three methods) C. The following example should clarify inputs/outputs files.

    Arguments

        Example: 
            ./matMultp a b c
        Input files: 
            a.txt 
            b.txt
        Output files: 
            c_per_matrix.txt
            c_per_row.txt
            c_per_element.txt

    No Arguments:

        Example: 
            ./matMultp
        Input files: 
            a.txt 
            b.txt
        Output files: 
            c_per_matrix.txt
            c_per_row.txt
            c_per_element.txt

    Custom Arguments:

        Example: 
            ./matMultp x y z
        Input files: 
            x.txt 
            y.txt
        Output files: 
            z_per_matrix.txt
            z_per_row.txt
            z_per_element.txt

* Read the number of rows and columns of the input matrices. They are written in the first line of the file as ”row=x col=y”. The following is an example of the format on an input file.

        row=3 col=5
        1 2 3 4 5
        6 7 8 9 10
        11 12 13 14 15

* Read the input matrices from their corresponding files. Each row is on a separate line, columns are separated by spaces.
* Use threads to calculate the matrix that results from multiplying the input two matrixes.
* Output the resulting matrices in three files (each file represents one method). The following is an example of the format of the three output files.

    c_per_matrix.txt

        Method: A thread per matrix
        row=2 col=2
        1 2
        3 4

    c_per_row.txt

        Method: A thread per row
        row=2 col=2
        1 2
        3 4

    c_per_element.txt

        Method: A thread per element
        row=2 col=2
        1 2
        3 4

    Of course the values of the output matrices should all be the same for all three methods.

* **Output to the number of threads created and the time taken on the standout (the console) for all the three methods (three different outputs).**
* Assume I will not test with matrix size larger than 20 x 20.
* Your program need to handle any errors and terminate gracefully.

## Synchronization

* Your program **should not** use any of the pthread synchronization functions, except for [pthread_join](https://man7.org/linux/man-pages/man3/pthread_join.3.html), i.e. you're not allowed to use mutual exclusion, or semaphores, or etc.
* You should never use [pthread_join](https://man7.org/linux/man-pages/man3/pthread_join.3.html) directly after the [pthread_create](https://man7.org/linux/man-pages/man3/pthread_create.3.html), you should use pthread_join only after the main thread has created all the worker (children) threads, otherwise your code will work in a sequential manner but just with extra overhead.
* The use of any synchronization functions, or the improper use of pthread_join, will make the submission totally unacceptable and will result into a zero grade.

## Memory Management

Your code should handle memory management and sending arguments to threads functions as following:

* In the case of (A thread per row) you can do either of the following:
  * Leave matrices A, B, and C in the global segment and only pass the row number to the thread function by value.
  * Create A, B, and C in the dynamic heap, then create a struct that has A*, B*, C* (pointers to the arrays) and the row number, then send the struct to the thread function by reference (allocate the struct in the dynamic heap).
* In the case of (A thread per element) you can do either of the following:
  * Leave matrices A, B, and C in the global segment, then create a struct that has the row number, and the column number, send this struct to the thread function by reference (allocate the struct in the dynamic heap).
  * Create A, B, and C in the dynamic heap, then create a struct that has A*, B*, C* (pointers to the arrays), the row number, and the column number , then send the struct to the thread function by reference (allocate the struct in the dynamic heap).
* In all cases, you have to free any allocated memory in the dynamic heap at the end of the worker (child) thread, no memory leak should be allowed.

## Code Organization
- We get size of 2 matrices and check col1 == row2
- Execute 3 functions sequentially and print their threads and time in console.
    - In first function for matrix multiplication we use 1 thread and calculate in resMatrix 1 then print it in file.
    - In second function for raw multiplication we make row1 threads array and each thread calculate row in resMatrix 2 then we join these threads and print it in file.
    - In third function for element multiplication we make row1 * col2 threads matrix and each thread calculate an element in resMatrix 3 then we join these threads and print in file.

## Main Functions
- In main functions we read input from user and check valid input and execute 3 functions and calculate executed time for each function.
- In matrix thread function we calculate resMatrix 1 and print it to file.
- In row thread function we create thread for each row and put arguments (row index, col1, col2) in pointer to argument struct and allocate it in memory and each thread execute raw thread fill function to calculate its row in resMatrix 2 then we join all these threads and print to file.
- In row thread fill function we calculate row[row index] of resMatrix2 then we deallocate/free pointer to struct.
- In element thread function we create threads for each element and put arguments (row index, col index, col1, col2) in pointer to argument struct and allocate it in memory and each thread execute element thread fill function to calculate its element in resMatrix 3 then we join all these threads and print to file.
- In element thread fill function we calculate element [row index] [col index] of resMatrix3 then we deallocate/free pointer to struct.

## How to compile and run code
1) Make 2 text files for each matrix 1 and matrix 2
2) Open terminal at lab folder
3) make command to compile project
4) ./matMultp file1_name file2_name outFile_name
5) 3 text files with name entered will be created each has function name used
6) The input text files must be in lab folder and exist with same name entered
7) No arguments will have default name a , b , c for files respectively else the first 3 arguments only will be taken.

## Comparison between the three methods
- First method:
    Fastest method as it has no overhead to create threads
- Second method:
    Slower than first method as it has overhead to create row1 threads
- Third method:
    Slowest method as it has overhead to create row1*col2 threads\
As execution time of matrix multiplication is not very large so overhead to create threads is equivalent to matrix multiplication
