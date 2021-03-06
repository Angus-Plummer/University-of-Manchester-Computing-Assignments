// Assignment_6.cpp : Sixth assignemnt of OOP in C++ course.
// written by Angus Plummer

// A simple matrix class

#include "stdafx.h"
#include<iostream>
#include<stdlib.h> // for c style exit
#include<cmath>
using namespace std;

class matrix{
	// Friend functions for input and output to streams
	friend ostream & operator<<(ostream &os, const matrix &mat);
	friend istream & operator>>(istream &is, matrix &mat);
private:
	// Member variables to hold data and dimensions
	double *mdata_;
	int rows_, columns_;
public:
	// Default constructor
	matrix() : mdata_(0), rows_(0), columns_(0) {}
	// Parameterized constructor. Initialises mdata_ with ixj matrix of zeros
	matrix(int i, int j) : mdata_(new double[i*j]{}), rows_(i), columns_(j) {}

	// Copy constructor. Creates deep copy of mat.
	matrix(matrix const &mat) : rows_(mat.rows_), columns_(mat.columns_) {
		mdata_ = new double[mat.rows_ * mat.columns_];
		// Copy over data from mat
		for (int i = 0; i < mat.rows_ * mat.columns_; i++) {
			mdata_[i] = mat.mdata_[i];
		}
	}

	// Move constructor. Takes Rvalue argument, assigns mat member variables to this and sets mat to be a null matrix
	matrix(matrix &&mat) : rows_(mat.rows_), columns_(mat.columns_), mdata_(mat.mdata_) {
		mat.rows_ = 0;
		mat.columns_ = 0;
		mat.mdata_ = nullptr;
	}

	// Destructor. Deletes mdata.
	~matrix() { delete[] mdata_; }

	// Access functions
	int getrows() const { return rows_; } // Return number of rows
	int getcols() const { return columns_; } // Return number of columns
	int index(int m, int n) const { // Return position in array of element (m,n)
		// if within bounds then return index otherwise print error and exit
		if (m>0 && m <= rows_ && n>0 && n <= columns_) return (n - 1) + (m - 1)*columns_;
		else {
			cout << "Error: out of range (" << m << ", " << n << ")" << endl;
			exit(1);
		}
	}
	// Returns (by reference) the (m,n) component of the matrix 
	double & operator()(int m, int n) const { return mdata_[index(m, n)]; }

	// Other functions 
	// Copy  Assignment operator
	matrix& operator=(matrix& mat);

	// Move Assignment operator
	matrix& operator=(matrix&& mat);

	// Addition, subtraction and multiplication
	matrix operator+(matrix& mat) const;

	matrix operator-(matrix& mat) const;

	matrix operator*(matrix& mat) const;

	// Return minor of matrix (remove row m and column n)
	matrix minor(int m, int n) const;

	// determinant
	double determinant() const;


};

// Copy assignment
matrix& matrix::operator=(matrix& mat) {
	// If self assigned then do nothing and return this
	if (&mat == this) {
		return *this;
	}
	// Delete mdata_ and copy the dimensions
	delete[] mdata_;
	rows_ = mat.rows_;
	columns_ = mat.columns_;
	// Create new mdata_ and deep copy from mat
	mdata_ = new double[rows_ * columns_];
	for (int i = 0; i < mat.rows_ * mat.columns_; i++) {
		mdata_[i] = mat.mdata_[i];
	}
	return *this;
}
// Move assignment
matrix& matrix::operator=(matrix&& mat) {
	// Swap all member data with mat
	swap(rows_, mat.rows_);
	swap(columns_, mat.columns_);
	swap(mdata_, mat.mdata_);
	return *this;
}
// Overloading addition
matrix matrix::operator+(matrix& mat) const {
	// If matrices are not same dimensions then print error message and quit
	if (rows_ != mat.rows_ || columns_ != mat.columns_) {
		cout << "Error in matrix addition: different matrix dimensions." << endl;
		exit(1);
	}
	// Otherwise make new matrix that is deep copy of this and add mat element by element
	matrix sum(*this);
	for (int i = 0; i < mat.rows_ * mat.columns_; i++) {
		sum.mdata_[i] += mat.mdata_[i];
	}
	return sum;
}
// Overloading subtraction operator
matrix matrix::operator-(matrix& mat) const{
	// If matrices are not same dimensions then print error message and quit
	if (rows_ != mat.rows_ || columns_ != mat.columns_) {
		cout << "Error in matrix subtraction: different matrix dimensions." << endl;
		exit(1);
	}
	// Otherwise make new matrix that is deep copy of this and subtract mat element by element
	matrix sub(*this);
	for (int i = 0; i < mat.rows_ * mat.columns_; i++) {
		sub.mdata_[i] -= mat.mdata_[i];
	}
	return sub;
}
// Overloading multiplication operator
matrix matrix::operator*(matrix& mat) const{
	// If matrix dimensions do not allow for multiplication then print error message and quit
	if (columns_ != mat.rows_) {
		cout << "Error in matrix multiplication: inoperable matrix dimensions." << endl;
		exit(1);
	}
	// Otherwise make new matrix of dimensions this.rows * mat.columns
	matrix mult(rows_, mat.columns_);
	// Iterate through every element of new matrix
	for (int i = 1; i <= mult.rows_; i++) {
		for (int j = 1; j <= mult.columns_; j++) {
			// AB = C : C_ij = SUM(A_im * B_mj) (sum over m)
			for (int m = 1; m <= columns_; m++) {
				mult(i, j) += (*this)(i, m) * mat(m, j);
			}
		}
	}
	return mult;
}

// Minor of matrix (matrix with row m and column n removed)
matrix matrix::minor(int m, int n) const {
	// if out of bounds print error and exit
	if (m <= 0 || m > rows_ || n <= 0 || n > columns_){
		cout << "Error: out of range (" << m << "/" << rows_ << ", " << n << "/" << columns_ << ")" << endl;
		exit(1);
	}
	// Make new matrix with both dimensions reduced by 1
	matrix minor_mat(rows_ - 1, columns_ - 1);
	int minor_row, minor_col; // Varaibles to hold row and col of new mat to write into
	// Iterate over each row
	for (int i = 1; i <= rows_; i++) {
		// Row number is unchanged if before the removed row and reduced by 1 if after
		minor_row = i;
		if (minor_row > m) {
			minor_row -= 1;
		}
		// Iterate over each column
		for (int j = 1; j <= columns_; j++) {
			// Column number unchanged if before the removed column and reduced by 1 if after
			minor_col = j;
			if (minor_col > n) {
				minor_col -= 1;
			}
			// If element is not in removed row or column then copy assign data using corrected indices
			if (i != m && j != n) {
				minor_mat(minor_row, minor_col) = (*this)(i, j);
			}
		}
	}
	return minor_mat;
}

// Returns determinant of matrix
double matrix::determinant() const {
	// If matrix is not square print error message and quit
	if (rows_ != columns_) {
		cout << "Error in finding determinant: matrix is not square" << endl;
		exit(1);
	}
	// If matrix is 2x2 then find by det(a) = a_11*a_22 - a_12*a_21
	if (rows_ == 2) {
		return (*this)(1, 1) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 1);
	}
	// Divide problem recursively using the expansion of minors
	double det = 0;
	// det(A) = SUM(over i or j) (-1)^(i+j) * A_ij * det(M_ij) where M_ij is minor of A with i,j removed 
	for (int j = 1; j <= columns_; j++) {
		// sum over j and use i=1
		det += pow(-1., 1. + j) * (*this)(1, j) * minor(1, j).determinant();
	}
	return det;
}


// Overload insertion to output stream for matrices
ostream & operator<<(ostream &os, const matrix &mat){
	// If matrix has non zero dimensions..
	if (mat.rows_ > 0 && mat.columns_ > 0) {
		// Print out each row with spaces between elements
		for (int i = 1; i <= mat.rows_; i++) {
			for (int j = 1; j <= mat.columns_; j++) {
				cout << mat(i, j) << ' ';
			}
			// New line at end of row
			cout << endl;
		}
	}
	// If matrix has a zero dimension then print null matrix
	else {
		cout << "null matrix" << endl;
	}
	return os;
}

// Overload insertion from input stream. Takes elements row by row, 1 element at a time
istream & operator>>(istream &is, matrix &mat){
	for (int i = 1; i <= mat.rows_; i++) {
		for (int j = 1; j <= mat.columns_; j++) {
			double value;
			cout << "Enter value for M(" << i << ", " << j << "):";
			is >> value;
			while (is.fail() || is.peek() != '\n') {
				cout << "Invalid input. Matrix elements must be of type double." << endl
					<< "Enter value for M(" << i << ", " << j << "):";
				// Reset input stream and try again
				is.clear();
				is.ignore(numeric_limits<streamsize>::max(), '\n');
				is >> value;
			}
			mat(i, j) = value;
		}
	}
	return is;
}

// Main program

int main(){
	// First part of assignment: constructing and deep copying matrices

	// Demonstrate default constructor
	matrix a1;
	cout << "Default constructor ->" << endl;
	cout << a1 << endl;

	// Parameterized constructor
	const int m(2), n(2);
	matrix a2(m, n);
	// Set values for a2 here
	a2(1, 1) = 1;
	a2(1, 2) = 0;
	a2(2, 1) = -1;
	a2(2, 2) = 2;

	// Print matrix a2
	cout << "Parameterised constructor and a2(i,j) = x ->" << endl;
	cout << "a2:" << endl;
	cout << a2 << endl;

	// Deep copy by assignment: define new matrix a3 then copy from a2 to a3
	cout << "Copy by assigment:" << endl;
	matrix a3(m, n);
	cout << "a3:" << endl;
	cout << a3 << endl;
	a3 = a2;
	cout << "a3=a2 ->" << endl << "a3:" << endl;
	cout << a3 << endl;
	// Modify contents of original matrix and show assigned matrix is unchanged here
	a2(1, 1) = 10;
	cout << "a2(1,1) = 10 ->" << endl;
	cout << "a2:" << endl;
	cout << a2 << endl;
	cout << "a3:" << endl;
	cout << a3 << endl;


	// Deep copy using copy constructor 
	cout << "a4(a2) ->" << endl;
	matrix a4(a2);
	cout << "a4:" << endl;
	cout << a4 << endl;
	// Modify contents of original matrix and show copied matrix is unchanged here
	cout << "a2(1,1) = 50 ->" << endl;
	a2(1, 1) = 50;
	cout << "a2:" << endl;
	cout << a2 << endl;
	cout << "a4:" << endl;
	cout << a4 << endl;

	// Move copy construction demonstration
	cout << "a5(move(a2)) ->" << endl;
	matrix a5(move(a2));
	cout << "a2:" << endl;
	cout << a2 << endl;
	cout << "a5:" << endl;
	cout << a5 << endl;

	// Move assignment demonstration
	matrix a6;
	a6 = move(a3);
	cout << "a6 = move(a3) ->" << endl;
	cout << "a3:" << endl;
	cout << a3 << endl;
	cout << "a6:" << endl;
	cout << a6 << endl;

	//
	// Second part of assignment: matrix operations
	//

	const int j(3), k(3);
	matrix m1(j, k);
	// Set values for m1 here
	m1(1, 1) = 7;
	m1(1, 2) = 9;
	m1(1, 3) = 2;
	m1(2, 1) = 3;
	m1(2, 2) = 0;
	m1(2, 3) = 1;
	m1(3, 1) = 6;
	m1(3, 2) = 8;
	m1(3, 3) = 5;

	matrix m2(j, k);
	// Set values for m2 here
	m2(1, 1) = 1;
	m2(1, 2) = 0;
	m2(1, 3) = 1;
	m2(2, 1) = 0;
	m2(2, 2) = 1;
	m2(2, 3) = 0;
	m2(3, 1) = 1;
	m2(3, 2) = 0;
	m2(3, 3) = 1;

	cout << "m1:" << endl;
	cout << m1 << endl;
	cout << "m2:" << endl;
	cout << m2 << endl;

	// Addition of 2 matrices
	cout << "m1 + m2 :" << endl;
	cout << m1 + m2 << endl;

	// Subtraction of 2 matrices
	cout << "m1 - m2 :" << endl;
	cout << m1 - m2 << endl;

	// Multiplication of 2 matrices
	cout << "m1 * m2 :" << endl;
	cout << m1 * m2 << endl;

	// Determinant
	cout << "determinant of m1 = " << m1.determinant() << endl;

	// Input from stream
	matrix m3(2, 3); // initialise 2x3 matrix of zeros
	// tell user to input
	cout << "m3:\n" << m3 << endl;
	cout << "Input your own values into m3:\n";
	// take user input and print back
	cin >> m3;
	cout << "m3:\n" << m3 << endl;
	
	return 0;
}
