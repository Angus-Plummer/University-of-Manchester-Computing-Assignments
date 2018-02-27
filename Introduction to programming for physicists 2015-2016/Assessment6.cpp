//Introduction to Programming for Physicists, Assessment 6: Reading, writing and transposing a matrix
//program to read in  an arbitrary matrix and write out its transpose to a new file

//includes
#include<fstream>
#include<string>
#include<vector>
#include<cstdio>
#include<sstream>
#include<iostream>

//namespace shortcut
using namespace std;

//main program
int main(){
	//declare null matrix to hold traspose matrix later
	vector < vector <string> > matrixTrans;

	//open the file to read in
	ifstream inFile("test_data.txt");

	//declare line variable to hold each row of data while read in
	string aLine;

	// check if it opened successfully
	if (inFile.is_open()) {
		// file is open
		// read it in
		while (!inFile.eof()) {
			// not at end of file
			//get line of file (row of matrix)
			getline(inFile, aLine);
			//create a stringstream of line to read through char at a time
			stringstream ss(aLine);
			//declare new vector to hold row elements
			vector<string> aLineVect;

			//while stringstream has no error flags raised
			while (ss.good()) {
				//declare temporary substring variable
				string subStr;
				//separate out substring from the stringstream by comma
				getline(ss, subStr, ',');
				//add substring as last element to aLineVect array
				aLineVect.push_back(subStr);
			}
			//add the row as the first column of the new transpose matrix
			matrixTrans.push_back(aLineVect);
		}
		inFile.close(); //close the file
	}
	else {
		// something went wrong so print to user
		cout << "Error opening file!\n";
		//wait for user input before continuing
		getchar();
		// die
		exit(1);
	}

	//define column and row size variables to iterate to
	int columns = matrixTrans.size();
	int rows = matrixTrans[0].size();

	//open new file to write matrix transpose out to
	ofstream outFile("matrix_transpose.txt");

	//if output file opened correctly
	if (outFile.is_open()){
		//cycle through column by row to write to file
		for (int j = 0; j < rows; j++){
			for (int i = 0; i < columns; i++){
				//if not last element of row then add comma
				if (i != columns - 1){
					outFile << matrixTrans[i][j] << ",";
				}
				//if last then make new line
				else{
					outFile << matrixTrans[i][j] << "\n";
				}
			}
		}
	}

	//wait for user input before continuing
	getchar();

	return 0;
}