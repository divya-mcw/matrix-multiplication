#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include "matmul.hpp"
using namespace std;

bool find_dataType(string filename){
    ifstream infile(filename);
    string first;
    infile>>first; 
    infile.seekg(0,ios::beg);
    return (first.find('.')==string::npos);// to check first value is decimal 
}

//using template to handle both int and float data types
template <typename T>
T** read_matrix_from_file(string filename, int* rows, int* cols){
    ifstream infile(filename);
    if (!(infile>>*rows>>*cols)||*rows<=0||*cols<=0){
        throw invalid_argument("No matrix dimension found in "+filename);
    }
    T** matrix=new T*[*rows];
    for(int i=0;i<*rows;++i){
        matrix[i]=new T[*cols];
        for(int j=0;j<*cols;++j){
            if (!(infile>>matrix[i][j])){
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

template <typename T>
void deleteMatrix(T** matrix,int rows){
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

int main(){
    //the wanted file can be executed by using the below array
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
        "Unit_test/unit_12",
        "Unit_test/unit_13"
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
        bool is_Afloat=find_dataType(A_file);
        bool is_Bfloat=find_dataType(B_file);
        bool is_Cfloat=find_dataType(C_file);

        // initializing the matrix
        void* A = nullptr;
        void* B = nullptr;
        void* C = nullptr;

        try
        {
            A=is_Afloat?(void*)read_matrix_from_file<double>(A_file,&rowsA,&colsA):(void*)read_matrix_from_file<int>(A_file,&rowsA,&colsA);
            B=is_Bfloat?(void*)read_matrix_from_file<double>(B_file,&rowsB,&colsB):(void*)read_matrix_from_file<int>(B_file,&rowsB,&colsB);
            C=is_Cfloat?(void*)read_matrix_from_file<double>(C_file,&rowsC,&colsC):(void*)read_matrix_from_file<int>(C_file,&rowsC,&colsC);
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
            if (is_Afloat) {
                deleteMatrix<double>((double**)A,rowsA);
                deleteMatrix<double>((double**)B,rowsB);
                deleteMatrix<double>((double**)C,rowsC);
            } else {
                deleteMatrix<int>((int**)A,rowsA);
                deleteMatrix<int>((int**)B,rowsB);
                deleteMatrix<int>((int**)C,rowsC);
            }
            continue;
        }

        //result matrix
        void* result=is_Afloat?(void*)new double*[rowsA]:(void*)new int*[rowsA];
        for(int i=0;i<rowsA;++i) {
            if (is_Afloat) ((double**)result)[i]=new double[colsB]();
            else ((int**)result)[i]=new int[colsB]();
        }

        // time measuring in different loops 
        string status="Success";
        for(int choice=1;choice<=6;++choice){
            auto start_time=chrono::high_resolution_clock::now();
            if (is_Afloat) matrix_multiplication((double**)A,rowsA,colsA,(double**)B,rowsB,colsB,(double**)result,choice);
            else matrix_multiplication((int**)A,rowsA,colsA,(int**)B,rowsB,colsB,(int**)result,choice);
            auto end_time=chrono::high_resolution_clock::now();
            durations[choice-1]=chrono::duration<double,milli>(end_time-start_time).count();

            // cross checking with C
            bool is_success=true;
            for(int i=0;i<rowsA&&is_success;++i){
                for(int j=0;j<colsB;++j){
                    if (is_Afloat) {
                        if (((double**)result)[i][j]-((double**)C)[i][j]>=numeric_limits<double>::epsilon()){
                            is_success=false;
                            status="Failed";
                            break;
                        }
                    }
                    else{
                        if(((int**)result)[i][j]!=((int**)C)[i][j]){
                            is_success=false;
                            status="Failed";
                            break;
                        }
                    }
                }
            }
            if(status=="Failed")break;  // Stop further testing on wrong output
        }
        write_result_to_csv(test_case, status, durations);

        //delete all the matrix finally
        if (is_Afloat) {
            deleteMatrix<double>((double**)A,rowsA);
            deleteMatrix<double>((double**)B,rowsB);
            deleteMatrix<double>((double**)C,rowsC);
        } 
        else {
            deleteMatrix<int>((int**)A,rowsA);
            deleteMatrix<int>((int**)B,rowsB);
            deleteMatrix<int>((int**)C,rowsC);
        }
    }
}
