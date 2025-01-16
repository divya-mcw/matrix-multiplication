#include <iostream>
#include "matmul.hpp"
using namespace std;

template <typename T>
void matrix_multiplication(T** A,int rowsA,int colsA,T** B,int rowsB,int colsB,T** result,int choice){
    // result matrix with zero
    for(int i=0;i<rowsA;i++){
        for(int j=0;j<colsB;j++){
            result[i][j]=0;
        }
    }

    switch (choice){
        case 1: 
            // ijk
            for(int i=0;i<rowsA;i++){
                for(int j=0;j<colsB;j++){
                    for(int k=0;k<colsA;k++){
                        result[i][j]+=A[i][k]*B[k][j];
                    }
                }
            }
            break;
        case 2: 
            // ikj
            for(int i=0;i<rowsA;i++){
                for(int k=0;k<colsA;k++){
                    for(int j=0;j<colsB;j++){
                        result[i][j]+=A[i][k]*B[k][j];
                    }
                }
            }
            break;
        case 3: 
            // jik
            for(int j=0;j<colsB;j++){
                for(int i=0;i<rowsA;i++){
                    for(int k=0;k<colsA;k++){
                        result[i][j]+=A[i][k]*B[k][j];
                    }
                }
            }
            break;
        case 4: 
            // jki
            for(int j=0;j<colsB;j++){
                for(int k=0;k<colsA;k++){
                    for(int i=0;i<rowsA;i++){
                        result[i][j]+=A[i][k]*B[k][j];
                    }
                }
            }
            break;
        case 5:
            // kij
            for(int k=0;k<colsA;k++){
                for(int i=0;i<rowsA;i++){
                    for(int j=0;j<colsB;j++){
                        result[i][j]+=A[i][k]*B[k][j];
                    }
                }
            }
            break;
        case 6: 
            // kji
            for(int k=0;k<colsA;k++){
                for(int j=0;j<colsB;j++){
                    for(int i=0;i<rowsA;i++){
                        result[i][j]+=A[i][k]*B[k][j];
                    }
                }
            }
            break;
        default:
            cerr<<"Invalid choice for multiplication"<<endl;
            exit(EXIT_FAILURE);
    }
}
template void matrix_multiplication<double>(double** A, int rowsA, int colsA, double** B, int rowsB, int colsB, double** result, int choice);
template void matrix_multiplication<int>(int** A, int rowsA, int colsA, int** B, int rowsB, int colsB, int** result, int choice);
