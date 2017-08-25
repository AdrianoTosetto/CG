#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <vector>

class Matrix
{
public:
	Matrix(int _rows, int _cols): rows(_rows), cols(_cols) {
		std::vector<int> m(_rows *_cols);
		rawMatrix = m;
	}
	void setValue(int row, int col, int value) {
		rawMatrix.at(row*cols + col) = value;
	}
	double getValue(int row, int col) {
		return rawMatrix.at(row*cols + col);
	}

	int &operator()(int i, int j) {
	    return rawMatrix[i*cols + j];
	}

	int operator()(int i, int j) const {
	    return rawMatrix[i*cols + j];
	}
	Matrix& operator*=(const Matrix& b) {
		
		for (int i = 0; i < this->rows; ++i) 
			for (int j = 0; j < b.cols; ++j) {
				setValue(i, j, 0);
				for (int k = 0; k < this->cols; ++k) {
					setValue(i, j, &rawMatrix[i][j] += &rawMatrix[i][k] * b.rawMatrix[k][j]);
				}
			}
	}
	Matrix operator*(Matrix a, const Matrix& b) {
 	   return a *= b;
	}
	~Matrix();

private:
	int rows;
	int cols;
	std::vector<int> rawMatrix;
};

int main(int argc, char const *argv[])
{
	Matrix m1(3,3);
	Matrix m2(3,3);

	for(int i = 0; i < 3;i++) {
		for(int j = 0; j < 3;j++) {
			m1.setValue(i,j,i+j);
		}
	}

	for(int i = 0; i < 3;i++) {
		for(int j = 0; j < 3;j++) {
			m2.setValue(i,j,i+j);
		}
	}

	return 0;
}

#endif