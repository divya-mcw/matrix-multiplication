#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include "matmul.hpp"
using namespace std;

double** read_matrix_from_file(string filename, int* rows, int* cols){
    ifstream infile(filename);
        if (!(infile>>*rows>>*cols)||*rows<=0||*cols<=0){
            throw invalid_argument("No matrix dimension found in "+filename);
        }
        double** matrix=new double*[*rows];
        for(int i=0;i<*rows;++i){
            matrix[i]=new double[*cols];
            for(int j=0;j<*cols;++j) {
                if (!(infile >> matrix[i][j])) {
                    throw runtime_error("Insufficient matix data in "+filename);
                }
            }
        }
        return matrix;
}

void write_result_to_csv(string test_case,string status,double durations[6]){
    ofstream resultsFile("results.csv",ios::app);
    resultsFile<<test_case<<","<<status;
    for(int i=0;i<6;++i) {
        resultsFile<<","<<durations[i];
    }
    resultsFile<<endl;
}

void deleteMatrix(double** matrix,int rows){
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

int main(){
    const string test_cases[] ={
        "Unit_test/unit_1",
        "Unit_test/unit_2",
        "Unit_test/unit_3",
        "Unit_test/unit_4",
        "Unit_test/unit_5",
        "Unit_test/unit_6",
        "Unit_test/unit_7",
        "Unit_test/unit_8",
        "Unit_test/unit_9",
        "Unit_test/unit_10",
        "Unit_test/unit_11",
        "Unit_test/unit_12"
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
        double durations[6]={0};  //setting empty array for measuring time
        // initializing the matrix
        double** A = nullptr;
        double** B = nullptr;
        double** C = nullptr;

        try
        {
            A=read_matrix_from_file(A_file,&rowsA,&colsA);
            B=read_matrix_from_file(B_file,&rowsB,&colsB);
            C=read_matrix_from_file(C_file,&rowsC,&colsC);
        }
        catch(const exception& e)
        {   string error=e.what();
        cout<<"matrix dimension or data error"<< endl;
            write_result_to_csv(test_case,error,durations);
            continue;
        }

        // Check if multiplication is valid and the C.txt matrix is valid
        if(colsA!=rowsB || rowsA!=rowsC||colsB!=colsC){
            cout<<"Matrix dimensions mismatch for test case "<< endl;
            string error="input dimension mismatch";
            write_result_to_csv(test_case,error,durations);

            if(rowsA!=rowsC||colsB!=colsC){
                cout<<"output matrix dimension mismatch"<<endl;
                string error="output dimension mismatch";
                write_result_to_csv(test_case,error,durations);
            }

            // Free allocated memory for invalid matrices
            deleteMatrix(A,rowsA);
            deleteMatrix(B,rowsB);
            deleteMatrix(C,rowsC);
            continue;
        }

        //result matrix
        double** result = new double*[rowsA];
        for (int i=0;i<rowsA;++i){
            result[i]=new double[colsB]();
        }

        // time measuring in different loops 
        string status="Success";
        for(int choice=1;choice<=6;++choice){
            auto start_time=chrono::high_resolution_clock::now();
            matrix_multiplication(A,rowsA,colsA,B,rowsB,colsB,result,choice);
            auto end_time=chrono::high_resolution_clock::now();
            durations[choice-1]=chrono::duration<double,milli>(end_time-start_time).count();

            // cross checking with C
            bool is_success=true;
            for(int i=0;i<rowsA&&is_success;++i){
                for(int j=0;j<colsB;++j){
                    // if(result[i][j]!=C[i][j]){
                    if(result[i][j]-C[i][j]>=numeric_limits<double>::epsilon()){
                        is_success=false;
                        status="Failed";
                        break;
                    }
                }
            }

            if(status=="Failed")break;  // Stop further testing on wrong output
        }
        write_result_to_csv(test_case, status, durations);

        //delete all the matrix finally
        deleteMatrix(A,rowsA);
        deleteMatrix(B,rowsB);
        deleteMatrix(C,rowsC);
        deleteMatrix(result,rowsA);
    }
}
