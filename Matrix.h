#ifndef MATRIX
#define MATRIX

#include <stdexcept>
#include <iostream>

class Matrix {
public:
    Matrix(unsigned rows, unsigned cols);   // Size constructor
    Matrix(const Matrix& m);                // Copy constructor
    ~Matrix();                              // Destructor
    
    double& operator() (unsigned row, unsigned col);        // Access operator
    double  operator() (unsigned row, unsigned col) const;  // Access operator
    Matrix& operator= (const Matrix& m);                    // Assignment operator
    
    
    int numRows() const;                    // Returns rows_
    int numCols() const;                    // Returns cols_
    void printData() const;                 // Prints in a grid
    
    Matrix operator- ();                    // Negative
    Matrix operator+ (const Matrix a);      // Addition
    Matrix operator- (const Matrix a);      // Subtraction
    Matrix operator* (const int a);               // Constant multiplication
    Matrix operator* (const Matrix a);      // Matrix multiplication
    
    Matrix transpose() const;               // Transpose
    void initialize(const int n);
    
private:
    unsigned rows_, cols_;
    double* data_;
};

#endif
