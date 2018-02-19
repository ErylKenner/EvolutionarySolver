#include "Matrix.h"

//Size Constructor
Matrix::Matrix(const int rows, const int cols)
    : m_rows(rows)
    , m_cols(cols){

    if (rows == 0 || cols == 0){
        std::cerr << "Error: Matrix size constructor has 0 size" << std::endl;
        exit(1);
    }
    m_data = new double[rows * cols];
}

//Copy constructor
Matrix::Matrix(const Matrix& m)
    : m_rows(m.numRows())
    , m_cols(m.numCols()){
    
    if (m_rows <= 0 || m_cols <= 0){
        std::cerr << "Error: Matrix copy constructor has 0 size" << std::endl;
        exit(1);
    }
    m_data = new double[m_rows * m_cols];
    for(int row = 0; row < m_rows; ++row){
        for(int col = 0; col < m_cols; ++col){
            m_data[m_cols * row + col] = m(row, col);
        }
    }
}

//Sets all matrix elements to 'n'
void Matrix::initialize(const int n){
    for(int row = 0; row < m_rows; ++row){
        for(int col = 0; col < m_cols; ++col){
            m_data[m_cols * row + col] = n;
        }
    }
}

int Matrix::numRows() const{
    return m_rows;
}

int Matrix::numCols() const{
    return m_cols;
}

//Prints the matrix as a grid to the console
void Matrix::printData() const{
    for(int row = 0; row < m_rows; ++row){
        for(int col = 0; col < m_cols; ++col){
            printf("%10.5lf", m_data[m_cols * row + col]);
        }
        std::cout << std::endl;
    }
}

//Returns a copy of itself which is transposed
Matrix Matrix::transpose() const{
    Matrix temp(m_cols, m_rows);
    for(int row = 0; row < m_rows; ++row){
        for(int col = 0; col < m_cols; ++col){
            temp(col, row) = m_data[m_cols * row + col];
        }
    }
    return temp;
}

//Destructor
Matrix::~Matrix(){
    delete[] m_data;
}

//Override = operator to copy each element
void Matrix::operator= (const Matrix& m){
    if(m.numRows() != m_rows || m.numCols() != m_cols){
        std::cerr << "Error: Matrix sizes are not equivalent. Cannot perform assignment." << std::endl;
        exit(1);
    }
    for(int row = 0; row < m_rows; ++row){
        for(int col = 0; col < m_cols; ++col){
            m_data[m_cols * row + col] = m(row, col);
        }
    }
}

//Override - (unary) operator to negate each element
Matrix Matrix::operator- (){
    Matrix temp(m_rows, m_cols);
    for(int row = 0; row < m_rows; ++row){
        for(int col = 0; col < m_cols; ++col){
            temp(row, col) = -1*m_data[m_cols * row + col];
        }
    }
    return temp;
}

//Override + operator to add elementwise
Matrix Matrix::operator+ (const Matrix& a){
    if(a.numRows() != m_rows || a.numCols() != m_cols){
        std::cerr << "Error: Matrix sizes are not equivalent. Cannot perform addition." << std::endl;
        exit(1);
    }
    Matrix temp(m_rows, m_cols);
    for(int row = 0; row < m_rows; ++row){
        for(int col = 0; col < m_cols; ++col){
            temp(row, col) = a(row, col) + m_data[m_cols * row + col];
        }
    }
    return temp;
}

//Override - operator to subtract elementwise
Matrix Matrix::operator- (const Matrix& a){
    if(a.numRows() != m_rows || a.numCols() != m_cols){
        std::cerr << "Error: Matrix sizes are not equivalent. Cannot perform subtraction." << std::endl;
        exit(1);
    }
    Matrix temp(m_rows, m_cols);
    for(int row = 0; row < m_rows; ++row){
        for(int col = 0; col < m_cols; ++col){
            temp(row, col) = m_data[m_cols * row + col] - a(row, col);
        }
    }
    return temp;
}

//Override * operator to multiply elementwise by a constant
Matrix Matrix::operator* (const int a){
    Matrix temp(m_rows, m_cols);
    for(int row = 0; row < m_rows; ++row){
        for(int col = 0; col < m_cols; ++col){
            temp(row, col) = a * m_data[m_cols * row + col];
        }
    }
    return temp;
}

//Override * operator to perform standard matrix multiplication
Matrix Matrix::operator* (const Matrix& a){
    if(m_cols != a.numRows()){
        std::cerr << "Error: Matrix sizes are incompatible. Cannoto perform matrix multiplication." << std::endl;
        exit(1);
    }
    Matrix temp(m_rows, a.numCols());
    for(int row = 0; row < m_rows; ++row){
        for(int col = 0; col < a.numCols(); ++col){
            //perform cross product between current row and current column
            temp(row, col) = 0;
            for(int element = 0; element < m_cols; ++element){
                temp(row, col) += m_data[m_cols * row + element] * a(element, col);
            }
        }
    }
    return temp;
}

//Override () operator to allow access at a specific location
double& Matrix::operator() (const int row, const int col){
    if (row >= m_rows || col >= m_cols ){
        std::cerr << "Error: Matrix subscript out of bounds (too large)." << std::endl;
        exit(1);
    }
    if(row < 0 || col < 0){
        std::cerr << "Error: Matrix subscript out of bounds (too small)." << std::endl;
        exit(1);
    }
    return m_data[m_cols * row + col];
}

//Override () operator to allow access at a specific location
double Matrix::operator() (int row, int col) const{
    if (row >= m_rows || col >= m_cols){
        std::cerr << "Error: Matrix subscript out of bounds (too large)." << std::endl;
        exit(1);
    }
    if(row < 0 || col < 0){
        std::cerr << "Error: Matrix subscript out of bounds (too small)." << std::endl;
        exit(1);
    }
    return m_data[m_cols * row + col];
}

//Sets each element to a random double in the range [min, max] with specified resolution
void Matrix::initRand(const double min, const double max, const unsigned resolution){
    if(min > max){
        std::cerr << "Error: initRand(): Min is larger than max." << std::endl;
        exit(1);
    }
    if(resolution == 0){
        std::cerr << "Error: initRand(): Resolution is equal to 0" << std::endl;
        exit(1);
    }
    for(int row = 0; row < m_rows; ++row){
            for(int col = 0; col < m_cols; ++col){
                //bound is the range in which to generate an int
                int bound = (int)(resolution * (max - min));
                //The double value is converted from bound
                double randDouble = min + (double)(rand() % bound) / resolution;
                //Passed into the matrix
                m_data[m_cols * row + col] = randDouble;
            }
        }
}





