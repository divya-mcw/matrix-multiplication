This project demonstrates various matrix multiplication algorithms by testing different loop orders and records their performance. It is implemented in C++ and designed to handle multiple test cases.

To run this project, you need:
    A C++ compiler (e.g., g++)
    Bash shell (for run.sh)
    Git (for version control and GitHub integration)

Each unit_test directory has:
    A.txt - Matrix A
    B.txt - Matrix B
    C.txt - Expected result matrix

The program will:
    Read matrices A, B, and C from the Unit_test/ directory.
    Perform matrix multiplication using six different loop orders.
    Compare the result with the expected matrix C.
    Record the results and performance metrics in results.csv.
Open the results.csv file for details on test outcomes.

The results.csv contain the overall result like status(success or failure) and time taken in ms for the ijk,ikj,kij,kji,jik and jki loop