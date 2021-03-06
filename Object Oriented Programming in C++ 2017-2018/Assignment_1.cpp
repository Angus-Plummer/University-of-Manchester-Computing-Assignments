// Assignment_1.cpp : First assignment of OOP in C++ course
// written by Angus Plummer
//
// This code calculates and returns the transition energy of an electron transition in the Bohr model of Hydrogen-like atoms.
// The user may input an atomic number, initial and final quantum numbers, and an output format of either Joules or electron-volts.
// Errors will be returned if the input is not correct and the user will be asked to input again until the input is acceptable.

// CHECK WITH SUPERVISORS BEFORE MARKING:
// Is using cmath fine? Niels said it's best to use C++ specific and not C
// I currently remove excess data from the stream after a correct input is accepted. Is this fine?
// The mark for having a separate function for calculating energy level. Does it mean allow the user to request energy of specific level or just how i have implemented?

#include "stdafx.h" // Precompiled header
#include<iostream>  // Main input and output libraries
#include<iomanip> // Input and output formatting
#include<cmath> // Math library for easier use of powers
#include<limits> // Library for determining max and min values numerical types. Allows better input error handling.


// Use namespace std for convenience
using namespace std;

// Function for calculating energy of level in eV
double bohr_energy(int z, int n){
	double energy = -13.6 * pow(z,2) * pow(n,-2); // E = -13.6 * Z^2 * n^(-2)
	return energy;
}

// Main function
int main(){
	// Declare variables 
	const double eV_to_J{ 1.602e-19 }; // Conversion rate between electron volts and joules
	double energy; // High precision variable to hold the energy
	int atomic_num; // Holds atomic number
	int n_initial; // Holds initial quantum number
	int n_final; // Holds final quantum number
	char output_flag; // Character for determing units for output of energy
	char repeat_flag; // Character for user determining whether to do another calculation
	bool running(true); // Boolean for continuation of program operation

	// Print text to welcome user and inform how the program works so that they will know what inputs are correct.
	cout << endl << "Welcome to the Bohr model transition energy calculation tool." << endl << endl
		<< "This program calculates the transition energy for a given atomic number (Z)," << endl 
		<< "initial quantum number (n_i), and final quantum number (n_f)." << endl
		<< "The output can be chosen to be in joules (J) or electron-volts (eV)." << endl << endl
		<< "Physically meaningful application of the Bohr model requires that Z, n_i," << endl 
		<< "and n_f be integer values satistfying (Z > 0), (n_i > 1), and (n_i > n_f > 0)." << endl << endl;

	//begin main program operation
	while (running) {

		// Ask user to input atomic number
		cout << "Please input the atomic number (Z > 0): ";
		cin >> atomic_num;

		// Check atomic_num input is a positive integer. Loop will run until input is valid.
		while (cin.fail() || cin.peek() != '\n' || atomic_num <= 0) {
			// Inform user of the error and inform them of the correct inputs
			cout << endl << "Sorry, your input was invalid. Atomic number (Z) must be an integer" << endl << "satisfying (Z > 0)." << endl << endl
				<< "Please input the atomic number (Z > 0): ";
			// Clear input and attempt again
			cin.clear(); // Clear cin of fail bit
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Remove input stream of any size and removes new line markers
			cin >> atomic_num; // User attempts new input
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // remove any excess data in stream

		// Ask user to input the initial quantum number
		cout << "Please input the initial quantum number (n_i > 1): ";
		cin >> n_initial;

		// Check n_intial input is positive integer. Loop will run until input is valid.
		while (cin.fail() || cin.peek() != '\n' || n_initial <= 1) {
			// Inform user of the error and inform them of the correct inputs
			cout << endl << "Sorry, your input was invalid. The initial quantum number (n_i) must be" << endl 
				<< "an integer satisfying (n_i > 1)." << endl << endl
				<< "Please input the initial quantum number (n_i > 1): ";
			// Clear input and attempt again
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> n_initial;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// Ask user to input final quantum number
		cout << "Please input the final quantum number (" << n_initial << " > n_f > 0): ";
		cin >> n_final;
		
		// Check n_final input is int with n_i > n_f > 0. Loop will run until input is valid.
		while (cin.fail() || cin.peek() != '\n' || n_final >= n_initial || n_final <= 0) {
			// Inform user of the error and inform them of the correct inputs
			cout << endl << "Sorry, your input was invalid. The final quantum number (n_f) must be" << endl
				<< "an integer satisfying (n_i = " << n_initial << " > n_f > 0)." << endl << endl
				<< "Please input the final quantum number (" << n_initial << " > n_f > 0): ";
			// Clear input and attempt again
			cin.clear(); 
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
			cin >> n_final;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// Ask user to input whether the output is wanted in joules or in electron-volts
		cout << "Please input \"j\" if would like the transition energy in joules (J)" << endl 
			<< "and \"e\" if you would like the transition energy in electron-volts (eV): ";
		cin >> output_flag;

		// Check output_flag input is either j or e. Loop will run until input is valid.
		while (cin.fail() || cin.peek() != '\n' || ((output_flag != 'e') && (output_flag != 'j'))) {
			// Inform user of the error and inform them of the correct inputs
			cout << endl << "Sorry, your input was invalid." << endl << endl
				<< "Please input \"j\" if would like the transition energy in joules (J)" << endl 
				<< "and \"e\" if you would like the transition energy in electron-volts (eV): ";
			// Clear input and attempt again
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
			cin >> output_flag;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// Calculate the energy of transition in electron-volts
		energy = bohr_energy(atomic_num, n_initial) - bohr_energy(atomic_num, n_final);

		// Output the energy in the units requested to three significant figures
		if (output_flag == 'j') {
			energy /= eV_to_J; // convert to juels
			cout << endl << "Transition energy = " << setprecision(3) << energy << " J. (Using Z = " << atomic_num << ", n_i = " << n_initial << ", n_f = " << n_final << ")" << endl << endl;
		}
		else {
			cout << endl << "Transition energy = " << setprecision(3) << energy << " eV. (Using Z = " << atomic_num << ", n_i = " << n_initial << ", n_f = " << n_final << ")" << endl << endl;
		}

		// Ask user if they wish to repeat the program with different variables
		cout << "If you would like to repeat the calculation using a different choice of" << endl 
			<< "variables, please input \"y\". If you would like to terminate the program," << endl 
			<< "please input \"n\": ";
		cin >> repeat_flag;

		//check repeat_flag input is either y or n. Loop will run until input is valid.
		while (cin.fail() || cin.peek() != '\n' || ((repeat_flag != 'y') && (repeat_flag != 'n'))) {
			// Inform user of the error and inform them of the correct inputs
			cout << endl << "Sorry, your input was invalid." << endl << endl
				<< "If you would like to repeat the calculation using a different choice of" << endl 
				<< "variables, please input \"y\". If you would like to terminate the program," << endl 
				<< "please input \"n\": ";
			// Clear input and attempt again
			cin.clear(); 
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
			cin >> repeat_flag; 
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// If user input 'n' then stop running loop
		if (repeat_flag == 'n') {
			running = false;
		}

		cout << endl; // New line for clarity if user wishes to repeat
	}

    return 0;
}

