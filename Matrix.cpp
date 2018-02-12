#include "Matrix.h"

//Constructor
Matrix::Matrix(unsigned rows, unsigned cols)
    : rows_ (rows)
    , cols_ (cols){
    if (rows == 0 || cols == 0){
        throw std::out_of_range("Matrix constructor has 0 size");
    }
    data_ = new double[rows * cols];
}

//Copy constructor
Matrix::Matrix(const Matrix& m)
    : rows_(m.numRows())
    , cols_(m.numCols()){
    
    data_ = new double[rows_ * cols_];
    for(int row = 0; row < rows_; ++row){
        for(int col = 0; col < cols_; ++col){
            data_[cols_*row + col] = m(row, col);
        }
    }
}

void Matrix::initialize(const int n){
    for(int row = 0; row < rows_; ++row){
        for(int col = 0; col < cols_; ++col){
            data_[cols_*row + col] = n;
        }
    }
}

int Matrix::numRows() const{
    return rows_;
}

int Matrix::numCols() const{
    return cols_;
}

//Prints the matrix as a grid
void Matrix::printData() const{
    for(int row = 0; row < rows_; ++row){
        for(int col = 0; col < cols_; ++col){
            printf("%10.5lf", data_[cols_*row + col]);
        }
        std::cout << std::endl;
    }
}

//Returns a copy which is transposed
Matrix Matrix::transpose() const{
    Matrix temp(cols_, rows_);
    for(int row = 0; row < rows_; ++row){
        for(int col = 0; col < cols_; ++col){
            temp(col, row) = data_[cols_*row + col];
        }
    }
    return temp;
}

//Destructor
Matrix::~Matrix(){
    delete[] data_;
}

//Override = operator
Matrix& Matrix::operator= (const Matrix m){
    if(m.numRows() != rows_ || m.numCols() != cols_){
        throw std::out_of_range("Matrix sizes are not equivalent. Cannot perform assignment");
    }
    for(int row = 0; row < rows_; ++row){
        for(int col = 0; col < cols_; ++col){
            data_[cols_*row + col] = m(row, col);
        }
    }
}

//Override - (unary) operator
Matrix Matrix::operator- (){
    Matrix temp(rows_, cols_);
    for(int row = 0; row < rows_; ++row){
        for(int col = 0; col < cols_; ++col){
            temp(row, col) = -1*data_[cols_*row + col];
        }
    }
    return temp;
}

//Override + operator
Matrix Matrix::operator+ (const Matrix a){
    if(a.numRows() != rows_ || a.numCols() != cols_){
        throw std::out_of_range("Matrix sizes are not equivalent. Cannot perform addition");
    }
    Matrix temp(rows_, cols_);
    for(int row = 0; row < rows_; ++row){
        for(int col = 0; col < cols_; ++col){
            temp(row, col) = a(row, col) + data_[cols_*row + col];
        }
    }
    return temp;
}

//Override - operator
Matrix Matrix::operator- (const Matrix a){
    if(a.numRows() != rows_ || a.numCols() != cols_){
        throw std::out_of_range("Matrix sizes are not equivalent. Cannot perform subtraction");
    }
    Matrix temp(rows_, cols_);
    for(int row = 0; row < rows_; ++row){
        for(int col = 0; col < cols_; ++col){
            temp(row, col) = data_[cols_*row + col] - a(row, col);
        }
    }
    return temp;
}

//Override * (constant) operator
Matrix Matrix::operator* (int a){
    Matrix temp(rows_, cols_);
    for(int row = 0; row < rows_; ++row){
        for(int col = 0; col < cols_; ++col){
            temp(row, col) = a * data_[cols_*row + col];
        }
    }
    return temp;
}

//Override * operator
Matrix Matrix::operator* (const Matrix a){
    if(cols_ != a.numRows()){
        throw std::out_of_range("Matrix sizes are incompatible. Cannot perform multiplication");
    }
    Matrix temp(rows_, a.numCols());
    for(int row = 0; row < rows_; ++row){
        for(int col = 0; col < a.numCols(); ++col){
            temp(row, col) = 0;
            for(int element = 0; element < cols_; ++element){
                temp(row, col) += data_[cols_*row + element] * a(element, col);
            }
        }
    }
    return temp;
}

//Override () operator
double& Matrix::operator() (unsigned row, unsigned col){
    if (row >= rows_ || col >= cols_ || row < 0 || col < 0){
        throw std::out_of_range("Matrix subscript out of bounds");
    }
    return data_[cols_*row + col];
}

//Override () operator
double Matrix::operator() (unsigned row, unsigned col) const{
    if (row >= rows_ || col >= cols_ || row < 0 || col < 0){
        throw std::out_of_range("Matrix subscript out of bounds");
    }
    return data_[cols_*row + col];
}

void Matrix::initRand(const double min, const double max, const int resolution){
    for(int row = 0; row < rows_; ++row){
            for(int col = 0; col < cols_; ++col){
                int bound = (int)(max - min);
                bound *= resolution;

                double randDouble = (double)(rand() % bound);

                randDouble = randDouble / resolution + min;

                data_[cols_*row + col] = randDouble;
            }
        }
}

//Returns the (flattened) index of the max value
int Matrix::maxVal() const{
    double maxVal = data_[0];
    int index = 0;
    for(int row = 0; row < rows_; ++row){
        for(int col = 0; col < cols_; ++col){
            if(data_[cols_*row + col] > maxVal){
                maxVal = data_[cols_*row + col];
                index = cols_*row + col;
            }
        }
    }
    return index;
}





