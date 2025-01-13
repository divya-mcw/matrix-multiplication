#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include "matmul.hpp"
using namespace std;

int** read_matrix_from_file(const char* filename, int* rows, int* cols){
    ifstream infile(filename);
    infile>>*rows>>*cols;
    int** matrix=new int*[*rows];
    for(int i=0;i<*rows;++i){
        matrix[i]=new int[*cols];
        for(int j=0;j<*cols;++j) {
            infile >> matrix[i][j];
        }
    }
    return matrix;
}

void write_result_to_csv(const string& test_case, const string& status, const double durations[5]){
    ofstream resultsFile("results.csv",ios::app);
    resultsFile<<test_case<<","<<status;
    for(int i=0;i<5;++i) {
        resultsFile<<","<<durations[i];
    }
    resultsFile<<endl;
}

int main(){
    const string test_cases[] ={
        "Unit_test/unit_1",
        "Unit_test/unit_2",
        "Unit_test/unit_3",
        "Unit_test/unit_4"
    };

    // Open the results CSV file and write the header
    ofstream resultsFile("results.csv");
    resultsFile<<"Test Case,Status,ijk t(ms),ikj t(ms),jik t(ms),jki t(ms),kij t(ms),kji t(ms)"<<endl;
    resultsFile.close();

    //loop through the test cases
    for(const auto& test_case:test_cases){
        cout<<"Running test case: "<<test_case<<endl;

        string A_file=test_case+"/A.txt";
        string B_file=test_case+"/B.txt";
        string C_file=test_case+"/C.txt";

        int rowsA,colsA,rowsB,colsB,rowsC,colsC;

        int** A=read_matrix_from_file(A_file.c_str(),&rowsA,&colsA);
        int** B=read_matrix_from_file(B_file.c_str(),&rowsB,&colsB);
        int** C=read_matrix_from_file(C_file.c_str(),&rowsC,&colsC);

        // Check if multiplication is valid
        if(colsA!=rowsB||rowsA!=rowsC||colsB!=colsC){
            cout<<"Matrix dimensions mismatch for test case "<< test_case<<". Skipping..."<< endl;

            // Free allocated memory for invalid matrices
            for(int i=0;i<rowsA;++i)delete[] A[i];
            for(int i=0;i<rowsB;++i)delete[] B[i];
            for(int i=0;i<rowsC;++i)delete[] C[i];
            delete[] A;
            delete[] B;
            delete[] C;
            continue;
        }
        //result matrix
        int** result = new int*[rowsA];
        for (int i = 0; i < rowsA; ++i){
            result[i] = new int[colsB]();
        }

        // yiming measuring
        double durations[5]={0};
        string status="Success";

        for(int choice=1;choice<=5;++choice){
            auto start_time=chrono::high_resolution_clock::now();
            matrix_multiplication(A,rowsA,colsA,B,rowsB,colsB,result,choice);
            auto end_time=chrono::high_resolution_clock::now();
            durations[choice-1]=chrono::duration<double,milli>(end_time-start_time).count();

            // cross checking with C
            bool is_success=true;
            for(int i=0;i<rowsA&&is_success;++i){
                for(int j=0;j<colsB;++j){
                    if(result[i][j]!=C[i][j]){
                        is_success=false;
                        status="Failed";
                        break;
                    }
                }
            }

            if(status=="Failed")break;  // Stop further testing on failure
        }

        write_result_to_csv(test_case, status, durations);

        for(int i=0;i<rowsA;++i)delete[] A[i];
        for(int i=0;i<rowsB;++i)delete[] B[i];
        for(int i=0;i<rowsC;++i)delete[] C[i];
        for(int i=0;i<rowsA;++i)delete[] result[i];
        delete[] A;
        delete[] B;
        delete[] C;
        delete[] result;
    }

    return 0;
}
