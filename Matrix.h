#ifndef MATRIX_H
#define MATRIX_H

#include <stdexcept>
#include <vector>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;

class Matrix {
public:
    Matrix(const unsigned int rows, const unsigned int cols); // Size constructor
    Matrix(const Matrix& m);                // Copy constructor
    ~Matrix();                              // Destructor
    
    double& operator() (const unsigned int row, const unsigned int col);          // Access operator
    double  operator() (const unsigned int row, const unsigned int col) const;    // Access operator
    void operator= (const Matrix& m);                           // Assignment operator
    
    std::vector<double> toVector() const;
    
    unsigned int numRows() const;                    // Returns rows_
    unsigned int numCols() const;                    // Returns cols_
    void printData() const;                 // Prints in a grid
    
    Matrix operator- () const;                    // Negative
    Matrix operator+ (const Matrix& a) const;     // Addition
    Matrix operator- (const Matrix& a) const;     // Subtraction
    Matrix operator* (const int a) const;         // Constant multiplication
    Matrix operator* (const Matrix& a) const;     // Matrix multiplication
    
    Matrix transpose() const;               // Transpose
    void initialize(const int n);
    void initRand(const double min, const double max, const unsigned int resolution=20);
    
private:
    const unsigned int m_rows, m_cols;
    double* m_data;
};

#endif
