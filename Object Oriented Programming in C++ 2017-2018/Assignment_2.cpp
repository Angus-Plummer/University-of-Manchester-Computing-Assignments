// Assignment_2.cpp : Second assignment of OOP in C++ course
// written by Angus Plummer
//
// This code computes the mean, standard deviation and standard 
// error of the mean electronic charge. Data is read from file.

//CHECK WITH SUPERVISORS:
// The assignment outline ask that the code checks if the file is opened successfully
// if not the program should print error message and terminate
// I have instead let the user input the filename as that makes more sense than recompiling 
// for different files. Is this fine?
//
// No information on how the output of the program is actually desired (other than in coulomb)


#include "stdafx.h"
#include<iostream>
#include<iomanip>
#include<fstream> // Library for i/o of files as streams
#include<cmath>
#include<string>

using namespace std;

//------------------------------------- Useful Functions -------------------------------------//

// Function to calculate the mean of a data set.
// Takes in data by reference to the array so that do not have to copy in data (if say file is large)
double mean(const double *dat, int SIZE) {
	// mean = 1/N * SUM(x)
	double sum(0);
	for (int i = 0; i < SIZE; i++) {
		sum += dat[i];
	}
	return sum / SIZE;
}

// Function to calculate standard deviation of data set. Takes by reference for same reason as mean function.
double std_dev(const double *dat, int SIZE) {
	//std dev = sqrt[ 1/(N-1) * SUM (x - mean)^2 ]
	double mean_val = mean(dat, SIZE);
	double delta_square_sum(0); // to hold SUM (x - mean)^2
	for (int i = 0; i < SIZE; i++) {
		delta_square_sum += pow((dat[i] - mean_val), 2);
	}
	double variance = delta_square_sum / (SIZE - 1);
	return pow(variance, 0.5);
}

// Function to check whether string is castable to a double
bool is_number(const string &s) {
	// attempt to convert to double
	try {
		stod(s);
	}
	// if error then return false
	catch (exception e) {
		return false;
	}
	// if success the return true
	return true;
}

// Function to calculate the standard deviation of a data set




int main() {
	//------------------------------------- Welcome User to Program -------------------------------------//
	cout << endl <<"This program calculates the mean and error of the electric charge from experimental data." << endl
		<< "Data is read in from an external file specified by the user." << endl << endl;

	//------------------------------------- Open File -------------------------------------//
	string filename;
	ifstream infile;

	// Ask for user input of file name
	cout << "Please enter name of file you wish to open: ";
	cin >> filename;
	// Attempt to open file asked for by user
	string filedir("C:\\Users\\mbcx3ap3\\Desktop\\");
	infile.open(filedir.append(filename));
	// if input was not valid or file could not be opened, try to input again.
	while (cin.fail() || !infile.good() || cin.peek() != '\n') {
		// Allow user to quit so that the program doesnt get stuck if there are no files to read
		if (filename == "quit" && cin.peek() == '\n') {
			return(1);
		}
		// If there was a problem with the input format, inform user
		if (cin.fail() || cin.peek() != '\n') {
			cout << "Sorry that input was invalid." << endl;
		}
		// If no bad input format but file could not be read, inform user
		else {
			if (!infile.good()){
				cout << "There was a problem loading the requested file." << endl;
			}
		}
		// clear fail bits, ignore bad input and request a new input
		cout << "Please enter name of file you wish to open or type \"quit\" to terminate the program: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		infile.clear();
		cin >> filename;
		infile.open(filename);
	}
	//clear any remaining data on cin stream
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	//------------------------------------- Determine Number of Data Points -------------------------------------//

	string line; // String to temporarily hold each line

	// iterate over the input and count the number of lines
	int num_valid_lines(0);
	int num_lines(0);

	// Keep iterating though each line until no more lines to read
	while (getline(infile, line)) {
		num_lines += 1;
		// If the line can be converted into a double then increment line count by 1
		if (is_number(line)) {
			num_valid_lines += 1;
		}
		// If line cannot be converted then inform user of invalid entry
		else {
			cout << endl << "Error: File contains an invalid entry: \"" << line << "\" (line " << num_valid_lines + 1 << ")" << endl
				<< "This entry will be ignored." << endl;
		}
	}
	// Inform user of number of valid data points from file
	cout << endl << "Number of valid data points: " << num_valid_lines << endl;
	cout << "Number of ignored (invalid) lines: " << num_lines - num_valid_lines << endl << endl;

	//------------------------------------- Allocate Memory and Read in Data -------------------------------------//

	// Initialise array of doubles with length as determined above
	double *data = new double[num_valid_lines];

	// Clear eof bit if set and set stream back to beginning of file
	infile.clear();
	infile.seekg(0, infile.beg);

	// Iterate through file and only add lines to the data array if they can be cast to doubles
	int counter(0); // variable to keep track of position of current valid data point in file
	// Iterate through all lines
	while (getline(infile, line)){
		// If line can be cast to double then do so and add to data array. Increase position tracker by 1.
		if (is_number(line)) {
			data[counter] = stod(line);
			counter += 1;
		}
	}

	// Close input data file
	infile.close();

	//------------------------------------- Calculate and Print Statistical Quantities -------------------------------------//
	// Double variables to hold the quantities
	double mean_val;
	double std_dev_val;
	double std_err_val;

	// Use functions defined above to calculate the quantities
	mean_val = mean(data, num_valid_lines);
	std_dev_val = std_dev(data, num_valid_lines);
	std_err_val = std_dev_val / pow(num_valid_lines, 0.5);

	// Delete data array from memory
	delete[] data;

	// Format output
	cout.setf(ios::fixed, ios::floatfield);
	cout << setprecision(3);

	// Output results to user
	cout << "The mean of the data is " << mean_val << endl;
	cout << "The standard deviation of the data is " << std_dev_val << endl;
	cout << "The standard error of the data is " << std_err_val << endl << endl;

	cout << "The charge of the electron has been calculated to be:" << endl
		<< "e = (-" << mean_val << " +/- " << std_err_val << ")x10^(-19) C" << endl << endl;

	cout << "The program will now terminate." << endl;

	return 0;
}

