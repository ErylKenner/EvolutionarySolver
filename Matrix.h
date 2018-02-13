#ifndef MATRIX
#define MATRIX

#include <stdexcept>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

class Matrix {
public:
    Matrix(const unsigned rows, const unsigned cols);   // Size constructor
    Matrix(const Matrix& m);                 // Copy constructor
    ~Matrix();                              // Destructor
    
    double& operator() (const unsigned row, const unsigned col);        // Access operator
    double  operator() (const unsigned row, const unsigned col) const;  // Access operator
    Matrix& operator= (const Matrix& m);                    // Assignment operator
    
    
    int numRows() const;                    // Returns rows_
    int numCols() const;                    // Returns cols_
    void printData() const;                 // Prints in a grid
    
    Matrix operator- ();                    // Negative
    Matrix operator+ (const Matrix& a);      // Addition
    Matrix operator- (const Matrix& a);      // Subtraction
    Matrix operator* (const int a);         // Constant multiplication
    Matrix operator* (const Matrix& a);      // Matrix multiplication
    
    Matrix transpose() const;               // Transpose
    void initialize(const int n);
    void initRand(const double min, const double max, const unsigned resolution=20);
    
private:
    const unsigned m_rows, m_cols;
    double* m_data;
};

#endif
