#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <ctime>
#include <complex>
#include <cblas.h>
using namespace std; 

const int EPS = 0.001;

float randFloat(float a, float b) {
    float n = rand();
    n = (n/RAND_MAX)*(b-a+1)+a;
    return n;
}

void randComplexMatrix(int n, complex<float> *A, float min, float max) {
    int in;
    for (int i=0; i<n; i++) {
        in = i*n;
        for (int j=0; j<n; j++) {
            A[in+j] = {randFloat(min,max),randFloat(min,max)};
        }
    }
}

void matrixZeroing(int n, complex<float> *A) {
    int i0;
    for (int i=0; i<n; i++) {
        i0 = i*n;
        for (int j=0; j<n; j++) {
            A[i0+j] = {0,0};
        }
    }
}

void simpleMultiplication(int n, complex<float> *A, complex<float> *B, complex<float> *C) {
    int in, index;
    for (int i=0; i<n; i++) {
        in = i*n;
        for (int j=0; j<n; j++) {
            index = in+j;
            for (int k=0; k<n; k++) {
                C[index] += A[in+k]*B[k*n+j];
            }
        }
    }
}

void selfWrittenMultiplication(int n, complex<float> *A, complex<float> *B, complex<float> *C) {
    const int block_size = 128;
    #pragma omp parallel for schedule(dynamic)

    for (int ii=0; ii<n; ii+=block_size) {
        for (int jj=0; jj<n; jj+=block_size) {
            complex<float>* __restrict c_ptr = &C[ii*n+jj];
            for (int kk=0; kk<n; kk+=block_size) {
                const complex<float>* __restrict a_ptr = &A[ii*n+kk];
                const complex<float>* __restrict b_ptr = &B[kk*n+jj];
                for (int i=0; i<block_size; i+=4) {
                    for (int j=0; j<block_size; ++j) {
                        complex<float> a0 = a_ptr[i*n+j];
                        complex<float> a1 = a_ptr[(i+1)*n+j];
                        complex<float> a2 = a_ptr[(i+2)*n+j];
                        complex<float> a3 = a_ptr[(i+3)*n+j];

                        complex<float>* c0 = &c_ptr[i*n];
                        complex<float>* c1 = &c_ptr[(i+1)*n];
                        complex<float>* c2 = &c_ptr[(i+2)*n];
                        complex<float>* c3 = &c_ptr[(i+3)*n];

                        for (int k=0; k<block_size; ++k) {
                            complex<float> b_val = b_ptr[j*n+k];
                            c0[k] += a0*b_val;
                            c1[k] += a1*b_val;
                            c2[k] += a2*b_val;
                            c3[k] += a3*b_val;
                        }
                    }
                }
            }
        }
    }
}
/*
void printMatrix(int n, complex<float> *A) {
    int i0;
    for (int i=0; i<n; i++) {
        i0 = i*n;
        for (int j=0; j<n; j++) {
            cout << A[i0+j] << " ";
        }
        cout << endl;
    }
}
*/
bool equalityTesting(int n, complex<float> *A, complex<float> *B) {
    int in;
    for (int i=0; i<n; i++) {
        in = i*n;
        for (int j=0; j<n; j++) {
            if (abs(A[in+j].real()-B[in+j].real())>EPS || abs(A[in+j].imag()-B[in+j].imag())>EPS) return false;
        }
    }
    return true;
}


main() {
    cout << "\nProgram started" << endl;
    int size = 4096;
    double start, end, time;
    complex<float> *A, *B, *C0, *C1, *C2;
    complex<float> alpha={1,1}, beta={0,0};

    A = new complex<float>[size*size];
    B = new complex<float>[size*size];
    C0 = new complex<float>[size*size];
    C1 = new complex<float>[size*size];
    C2 = new complex<float>[size*size];

    cout << "\nGeneration two " << size << "x" << size << " matrices... " << endl;
    randComplexMatrix(size,A,-64,64);
    randComplexMatrix(size,B,-64,64);
    cout << "Comleted" << endl;

    cout << "\nPreparation of final matrices... " << endl;
    matrixZeroing(size,C0);
    matrixZeroing(size,C1);
    matrixZeroing(size,C2);
    cout << "Comleted" << endl;
/*
    cout << "\nSimple " << size << "x" << size << " matrix multiplication... " << endl;
    start = clock();
    simpleMultiplication(size,A,B,C0);
    end = clock();
    time = (end-start)/CLOCKS_PER_SEC;
    cout << "Comleted in " << time << " seconds. " << pow(size,3)/(time*500000) << " MFlops" << endl;
*/

    cout << "\nBLAS " << size << "x" << size << " matrix multiplication... " << endl;
    start = clock();
    cblas_cgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,size,size,size,&alpha,A,size,B,size,&beta,C1,size);
    end = clock();
    time = (end-start)/CLOCKS_PER_SEC;
    cout << "Comleted in " << time << " seconds. " << pow(size,3)/time/500000 << " MFlops" << endl;


    cout << "\nSelf-written " << size << "x" << size << " matrix multiplication... " << endl;
    start = clock();
    selfWrittenMultiplication(size,A,B,C2);
    end = clock();
    time = (end-start)/CLOCKS_PER_SEC;
    cout << "Comleted in " << time << " seconds. " << pow(size,3)/(time*500000) << " MFlops" << endl;

    cout << "\nEquality testing... ";
    if (equalityTesting(size,C1,C2) == 1) cout << "True" << endl;
    else cout << "False" << endl;
    cout<<"\n\nPress any key!";
    getch();
}