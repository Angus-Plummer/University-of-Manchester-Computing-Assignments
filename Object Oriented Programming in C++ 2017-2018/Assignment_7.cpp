// Assignment_7.cpp : Seventh assignemnt of OOP in C++ course.
// written by Angus Plummer
//
// A class for n dimensional vectors
// A class for 4-vectors that inherits from the n dimensional vector class
// A particle class

// Questions for demonstrators:
// What is really wanted in main()? Do we need to repeat tests to show inherited functions still work?

#include "stdafx.h"
#include<cmath>
#include<iostream>

using namespace std;

class Vector {
	
protected:
	// Member data
	int dimensions_;
	double *vData_;
public:
	// Default ctor. 0D null vector
	Vector() : dimensions_(0), vData_(nullptr) {}
	// Parametrised ctor (intialises as n dimensional array of zeros)
	Vector(int dim) : dimensions_(dim), vData_(new double[dim]{}) {}
	// Copy ctor (deep)
	Vector(Vector const &vec) : dimensions_(vec.dimensions_) {
		vData_ = new double[dimensions_];
		// Deep copy over data from vec
		for (int i = 0; i < dimensions_; i++) {
			vData_[i] = vec.vData_[i];
		}
	}
	// Move ctor. Takes Rvalue vector, moves over data to new vector and sets vec to be 0D null vector
	Vector(Vector &&vec) : dimensions_(vec.dimensions_), vData_(vec.vData_) {
		vec.dimensions_ = 0;
		vec.vData_ = nullptr;
	}
	// Dtor. Deletes array of vector components
	~Vector() { delete[] vData_; }

	// Returns number of dimensions
	int getdimensions() const { return dimensions_; } 
	// Returns magnitude of matrix
	double magnitude() const {
		double result = 0; // temp variable to hold sum
		// iterate over components and use N-D pythagoras theorem to get magnitude
		for (int i = 0; i < dimensions_; i++) {
			result += vData_[i] * vData_[i];
		}
		return pow(result, 0.5);
	}
	// Returns nth element of vector
	double & operator[](int n) const {
		// If within bounds then return
		if (n >= 0 && n < dimensions_) { return vData_[n]; }
		// Else print error message and exit
		else {
			cout << "Error: (" << n << ") out of range.\n";
			exit(1);
		}
	}
	// Copy  Assignment operator
	Vector& operator=(Vector const &vec);

	// Move Assignment operator
	Vector& operator=(Vector&& vec);

	// Dot product
	double dot(Vector const &vec) const;

	// friend function for output to stream overloading
	friend ostream& operator<<(ostream &os, const Vector &vec);
};

// Copy assigment operator
Vector& Vector::operator=(Vector const &vec){
	// If self assigned then do nothing and return *this
	if (&vec == this) {
		return *this;
	}
	// Delete vector components and copy dimensions
	delete[] vData_;
	dimensions_ = vec.dimensions_;
	// Create new components with new dimensions and deep copy over
	vData_ = new double[dimensions_];
	for (int i = 0; i < dimensions_; i++) {
		vData_[i] = vec.vData_[i];
	}
	return *this;
}

// Move assignment operator
Vector& Vector::operator=(Vector&& vec) {
	// If not self assigned then swap data
	if (this != &vec) {
		// Swap all member data
		swap(dimensions_, vec.dimensions_);
		swap(vData_, vec.vData_);
	}
	return *this;
}

// Dot product
double Vector::dot(Vector const &vec) const {
	// If vectors have same dimension then calc and return dot product
	if (dimensions_ == vec.dimensions_) {
		double result = 0; // temp variable for holding sum
		for (int i = 0; i < dimensions_; i++) {
			result += vData_[i] * vec[i];
		}
		return result;
	}
	// otherwise print error message and quit
	else {
		cout << "Error: Different matrix dimensions (" << dimensions_ << " =/= " << vec.dimensions_ << ")\n";
		exit(1);
	}
}

// Overload insertion to output stream for matrices
ostream& operator<<(ostream &os, Vector const &vec) {
	// If vector has non zero dimensions..
	if (vec.dimensions_ > 0) {
		cout << '(';
		// Print out each row with commas between elements
		for (int i = 0; i < vec.dimensions_ -1; i++) {
			cout << vec[i] << ",";
		}
		// last element has bracket close after instead of comma
		cout << vec[vec.dimensions_ - 1] << ")";
	}
	// If vector has zero dimension then print "null vector"
	else {
		cout << "null vector" << endl;
	}
	return os;
}

// 4-Vector class. Inherits from general vector class
class Vector4 : public Vector {
public:
	// Default ctor. Makes 4D vector of zeros
	Vector4(): Vector(4) {}
	// Parameterised constructor 1: takes time component and a 3 vector
	Vector4(double ct, Vector r) : Vector(4) {
		// If argument vector is 3D then copy in data
		if (r.getdimensions() == 3) {
			vData_[0] = ct;
			vData_[1] = r[0];
			vData_[2] = r[1];
			vData_[3] = r[2];
		}
		// If not 3D then print error and exit
		else {
			cout << "Error: Constructing 4 vector from " << r.getdimensions() << " dimensional vector.\n";
			exit(1);
		}
	}
	// Parameterised constructor 2: takes all 4 components as args
	Vector4(double ct, double x, double y, double z) : Vector(4) {
		vData_[0] = ct;
		vData_[1] = x;
		vData_[2] = y;
		vData_[3] = z;
	}
	// Dot product. (ct)^2 - r^2
	double dot(Vector4 const &vec) const {
		double result = 0; // to hold result as summed up
		result += vData_[0] * vec[0]; // (ct)^2
		for (int i = 1; i < 4; i++) {
			result -= vData_[i] * vec[i]; //(r_i)^2
		}
		return result;
	}
	// Lorentz boost
	Vector4 boost(Vector const &beta) const {
		// boost vector must be 3D and have mag less than 1
		if (beta.getdimensions() == 3 && beta.magnitude() < 1.) {
			// If magnitude of boost is 0 then return copy of (this) vector (avoids division by zero)
			if (beta.magnitude() == 0) {
				Vector4 boosted(*this); // copy of (this)
				return boosted;
			}
			// Components of 4-vector to be boosted
			double ct = vData_[0];
			Vector r(3); // space components as 3 vector to do dot products
			r[0] = vData_[1];
			r[1] = vData_[2];
			r[2] = vData_[3];
			// Resulting boosted 4-vector
			Vector4 boosted;
			// gamma = 1 / sqrt(1 - B^2)
			double gamma = pow(1 - pow(beta.magnitude(), 2.), -0.5);
			// ct' = gamma * (ct - B.r)
			boosted[0] = gamma * (ct - beta.dot(r));
			// r' = r + ( (gamma-1)(r.B/B^2) - gamma*ct ) * B
			for (int i = 0; i < 3; i++) {
				boosted[i + 1] = r[i] + ((gamma - 1.) * r.dot(beta) * pow(beta.magnitude(), -2.) - gamma * ct) * beta[i];
			}
			return boosted;
		}
		else {
			// magnitude >= 1 then print error and quit
			if (beta.magnitude() >= 1.){
				cout << "Error: Lorentz boost with v >= c.\n";
				exit(1);
			}
			// If B isnt 3D then print error and quit
			else {
				cout << "Error: Lorentz boost with " << beta.getdimensions() << " dimensional vector\n";
				exit(1);
			}
		}
	}
};

// Particle class.
class Particle {
private:
	// Member variables
	Vector4 position_;
	double mass_;
	Vector beta_;
public:
	// Parameterised ctor takes position, mass (in energy units MeV/c^2), and velocity
	Particle(Vector4 pos, double m, Vector b) : position_(pos) {
		// mass cannot be negative
		if (m >= 0) {
			mass_ = m;
		}
		// if negative then print wrror message and quit
		else {
			cout << "Error: Particle with negative mass (" << m << ")\n";
			exit(1);
		}
		// velocity cannot exceed c (beta=1)
		if (b.magnitude() <= 1) {
			beta_ = b;
		}
		// if v>c print error message and quit
		else {
			cout << "Error: Particle velocity greater than c";
			exit(1);
		}
		// massless particles must have v=c, massive particles cannot have v=c
		// if wrong then print error message and quit
		if ((b.magnitude() == 1 && m != 0) || (m == 0 && b.magnitude() != 1)) {
			cout << "Error: m=0 <=> v=c not held\n";
			exit(1);
		}
	}
	// return the gamma factor
	double gamma() {
		// gamma = 1 / sqrt(1-B^2)
		return pow(1 - pow(beta_.magnitude(), 2.), -0.5);
	}
	// returns the energy E = gamma * mc^2
	double energy() {
		return gamma() * mass_;
	}
	// returns the momentum p = gamma * mv in energy units (MeV/c)
	double momentum() {
		return gamma() * mass_ * beta_.magnitude();
	}

};


int main(){
	
	Vector beta(3);
	beta[0] = 0.8;
	
	Vector4 v4;
	v4[0] = 0;
	v4[1] = 0;
	v4[2] = 0;
	v4[3] = 0;

	Vector4 vboost(v4.boost(beta));
	cout << "beta = " << beta << endl;
	cout << "v4 = " << v4 << endl;
	cout << "boosted: " << vboost << endl;

	Particle proton(v4, 938.27231, beta);
	cout << "E = " << proton.energy() << " MeV." << endl
		<< "p = " << proton.momentum() << " MeV/c." << endl;

	return 0;
}
