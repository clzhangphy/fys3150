// Using Blas and armadillo for matrix-matrix multiplications

#include <cstdlib>
#include <ios>
#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;


/*Because fortran files don't have any header files,
 *we need to declare the functions ourself.*/
extern "C"
{
    void dgemm_(char*, char*, int*, int*, int*, double*,
            double*, int*, double*, int*, double*, double*, int*);
}


int main(int argc, char** argv)
{
    //Dimensions
    int n = atoi(argv[1]);
    int m = n;
    int p = m;

    /*Create random matrices
     * (note that older versions of armadillo uses "rand" instead of "randu") */
    srand(time(NULL));
    mat A(n, p);
    A.randu();
    // Pretty print, and pretty save, are as easy as the two following lines.
    cout << A << endl;
    A.save("A.mat", raw_ascii);
    mat A_trans = trans(A);
    mat B(p, m);
    B.randu();
    mat C(n, m);
    //    cout << B << endl;
    //    B.save("B.mat", raw_ascii);

    /////////////////////////////////////////////
    //////
    /////       ARMADILLO  
    ////                   TEST  
    ///                          BLOCK       
    //
    cout << "Starting armadillo multiplication\n";
    //Simple wall_clock timer is a part of armadillo.
    wall_clock timer;
    timer.tic();
    C = A*B;
    double num_sec = timer.toc();
    cout << "-- Finished in " << num_sec << " seconds.\n\n";
    //Storing results.
    mat C_armadillo = C;

    /////////////////////////////////////////////
    //////
    /////         
    ////        BLAS
    ///                        
    //
    C = zeros<mat> (n, m);
    cout << "Starting blas multiplication.\n";
    timer.tic();
    {
        char trans = 'N';
        double alpha = 1.0;
        double beta = 0.0;
        int _numRowA = A.n_rows;
        int _numColA = A.n_cols;
        int _numRowB = B.n_rows;
        int _numColB = B.n_cols;
        int _numRowC = C.n_rows;
        int _numColC = C.n_cols;
        int lda = (A.n_rows >= A.n_cols) ? A.n_rows : A.n_cols;
        int ldb = (B.n_rows >= B.n_cols) ? B.n_rows : B.n_cols;
        int ldc = (C.n_rows >= C.n_cols) ? C.n_rows : C.n_cols;
        dgemm_(&trans, &trans, &_numRowA, &_numColB, &_numColA, &alpha,
                A.memptr(), &lda, B.memptr(), &ldb, &beta, C.memptr(), &ldc);
    }
    num_sec = timer.toc();
    cout << "-- Finished in " << num_sec << " seconds.\n";
     //Comparing
    cout << "Comparing results... ";
    double diff = max(max(abs(C - C_armadillo)));
    cout << " Largest difference between two elements: " << diff << endl;

    cout << "\n\nIf the difference was zero, and the two methods used approx. "
	 << "the same wall clock time,\nI would guess armadillo keeps its promise "
	 << "and interfaces blas!\n";
    
    return 0;   

}
