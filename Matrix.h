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
    Matrix(const unsigned int rows, const unsigned int cols);
    Matrix(const Matrix& m);
    ~Matrix();
    
    double& operator() (const unsigned int row, const unsigned int col);
    double  operator() (const unsigned int row, const unsigned int col) const;
    void operator= (const Matrix& m);
    
    std::vector<double> toVector() const;
    
    unsigned int numRows() const;
    unsigned int numCols() const;
    void printData() const;
    
    Matrix operator- () const; 
    Matrix operator+ (const Matrix& a) const;
    Matrix operator- (const Matrix& a) const;
    Matrix operator* (const int a) const;
    Matrix operator* (const Matrix& a) const;
    
    Matrix transpose() const;
    void initialize(const int n);
    void initRand(const double min, const double max, 
                  const unsigned int resolution=20);
    
    Matrix addRowsCols(unsigned int extraRows, unsigned int extraCols, 
                       const double value);
    
private:
    unsigned int m_rows, m_cols;
    double* m_data;
};

#endif
