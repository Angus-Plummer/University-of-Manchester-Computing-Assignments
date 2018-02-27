// Assignment_3.cpp : Third assignemnt of OOP in C++ course.
// written by Angus Plummer
//
//Simple code to store and sort courses names using vectors and strings

#include "stdafx.h"
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>

using namespace std;

//------------------------------------- Comparison functions for sorting strings -------------------------------------//


// Function for comparing course numbers. arg1 < arg2 returns true
bool num_comparison(string str1, string str2) {
	// Create substring of course number part 
	// Starts at 6th char and is 5 chars long (PHYS ##### title)
	string num1 = str1.substr(5, 5);
	string num2 = str2.substr(5, 5);
	return (stod(num1) < stod(num2)); // Converts the substrings to numbers and returns comparison
}
// Function for comparing course titles. arg1 < arg2 returns true
bool title_comparison(string str1, string str2) {
	// Create substring of course name part 
	// Starts at 12th char and continues to end of string (PHYS ##### title)
	string title1 = str1.substr(11);
	string title2 = str2.substr(11);
	// Convert both strings to upper case so that comparison ignores case
	transform(title1.begin(), title1.end(), title1.begin(), ::toupper);
	transform(title2.begin(), title2.end(), title2.begin(), ::toupper);

	return (title1 < title2);
}

int main() {
	//------------------------------------- Welcome User to Program -------------------------------------//
	cout << endl << "This program reads in user input of course numbers and titles and prints them back to the user." << endl
		<< "Courses from a specific year can be viewed and the data can be sorted by course number or title." << endl
		<< "Course titles should be entered in the following format: ##### course-title" << endl << endl;

	//------------------------------------- Get list of courses from user -------------------------------------//
	const string degreeCode("PHYS"); // Indicates that a course is in Physics. Is at start of all course codes
	string line; // String to write input into
	int courseNum; // Course number
	string courseTitle; // Course title text
	stringstream tempStream; // Stringstream for easily joining the number and name
	vector<string> courseList; // Vector list of all course titles

	bool runInput(true);

	while (runInput) {
		// Reset stringstream fail bits
		tempStream.clear();
		cout << "Please enter a course (or x to finish): ";
		// Take whole input line and put into "line" string
		getline(cin, line);
		// If input is "x" then stop requesting more input
		if (line == "x") {
			runInput = false;
		}
		else {
			tempStream.str(line); // Put input string into the stringstream
			tempStream >> courseNum; // First element is course number
			// If first element could not be put in int courseNum then inform user and try again
			if (tempStream.fail() || courseNum < 10000 || courseNum > 49999) {
				cout << "Invalid input: Course number must be an integer between 10000 and 49999" << endl;
				continue;
			}
			// If next char is not a space or tab then tell user bad formatting and try again
			if (tempStream.peek() != ' ' && tempStream.peek() != '	') {
				cout << "Invalid input: Please input number and title on same line with a space between them." << endl
					<< "(##### course name)" << endl;
				continue;
			}
			// Skip whitespace and add remaining string from stream into courseName
			tempStream >> ws;
			getline(tempStream, courseTitle, '\n');

			// Reset string stream and put degree code, number, and course title together
			tempStream.str("");
			tempStream.clear();
			tempStream << degreeCode << " " << courseNum << " " << courseTitle;
			// Add this concatenated string to the list of courses
			courseList.push_back(tempStream.str());
		}
	}

	//------------------------------------- Print courses back to user and select specific year -------------------------------------//

	// Print full list
	cout << endl << "List of courses:" << endl << endl;
	for (auto iterator = courseList.begin(); iterator != courseList.end(); iterator++) {
		cout << *iterator << endl;
	}

	// Extract courses belonging to specific year
	cout << endl << "Courses for a specific year can be listed." << endl;
	char year;
	cout << "Please enter a year (or 0 to skip): ";
	cin >> year;
	// Keeps requesting input until user enters 0, 1, 2, 3, or 4.
	while (cin.fail() || cin.peek() != '\n' || !(year == '0' || year == '1' || year == '2' || year == '3' || year == '4')) {
		cout << "Invalid input: year must be 0, 1, 2, 3, or 4." << endl
			<< "Please enter a year (or 0 to skip): ";
		// Reset input stream and try again
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> year;
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear cin of any remaining data

	// If user didnt want to skip then print out all courses in that year
	if (year != '0'){
		cout << endl;
		for (auto iterator = courseList.begin(); iterator != courseList.end(); iterator++) {
			if ((*iterator).at(5) == year) { // Year digit is 6th digit (PHYS Y####)
				cout << *iterator << endl;
			}
		}
	}

	//------------------------------------- Sort data as requested and print again -------------------------------------//

	// Sort by course number or name
	cout << endl << "Courses can be sorted by course number or name." << endl;
	string sortType;
	cout << "Please enter \"num\" to sort by number or \"name\" to sort by name: " << endl;
	cin >> sortType;
	// Keeps requesting input until user enters "num" or "name"
	while (cin.fail() || cin.peek() != '\n' || !(sortType == "num" || sortType == "name")) {
		cout << "Invalid input." << endl
			<< "Please enter \"num\" to sort by number or \"name\" to sort by name: ";
		// Reset input stream and try again
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> sortType;
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear cin of any remaining data

	// If number sort requested then sort using the course number comparison function defined at top of program
	if (sortType == "num") {
		std::sort(courseList.begin(), courseList.end(), num_comparison);
	}
	// If number sort requested then sort using the course name comparison function defined at top of program
	if (sortType == "name") {
		std::sort(courseList.begin(), courseList.end(), title_comparison);
	}

	// Print out sorted list of courses
	for (auto iterator = courseList.begin(); iterator != courseList.end(); iterator++) {
		cout << *iterator << endl;
	}

    return 0;
}

