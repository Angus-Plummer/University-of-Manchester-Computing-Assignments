/*
Introduction to Programming for Physicists, Assessment 7: Inverting a matrix.

Program to read in  an arbitrary matrix and write out its inverse to a new file.

Program will abort if:  Failed to read or write files.
                        Matrix is not square.
                        Any matrix elements are not castable to a double.

Program will not detect if the determinant is 0 and thus no inverse exists.
*/

//includes
#include<fstream>
#include<string>
#include<vector>
#include<cstdio>
#include<sstream>
#include<iostream>
#include<stdexcept>

//namespace shortcut
using namespace std;

//main program
int main(){
	//declare null matrix to hold input matrix
	vector < vector <double> > matrixInput;

	//declare line variable to hold each row of data while reading in
	string aLine;

	//---------------Read in file and fill input matrix---------------//

	//open the file to read in
	ifstream inFile("test_data.txt");

	// check if the file opened successfully
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
			vector<double> aLineVect;

			//while stringstream has no error flags raised (usually means not at end of stream)
			while (ss.good()) {
				//declare temporary substring variable
				string subStr;
				//separate out substring from the stringstream by comma
				getline(ss, subStr, ',');
				
				//attempt to cast the substring to a double type variable, element
				double element;
				try{
					//attempt conversion
					element = stod(subStr);
				}
				catch (const invalid_argument&){
					//if argument was invalid then inform user
					cerr << "Matrix elements must be numbers\n";
					//wait for user input before closing
					getchar();
					// die
					exit(1);
				}
				//add substring as last element to aLineVect
				aLineVect.push_back(element);
			}
			//add the row to the input matrix
			matrixInput.push_back(aLineVect);
		}
		inFile.close(); //close the file
	}
	else {
		// something went wrong so print to user
		cout << "Error opening file!\n";
		//wait for user input before closing
		getchar();
		// die
		exit(1);
	}

	//-----Check matrix validity and initialise augmented matrix for gaussian elimination-----//

	//define column and row size variables to iterate to
	int rows = matrixInput.size();
	int columns = matrixInput[0].size();

	//check input matrix is square
	if (rows != columns){
		//tell user that input was not valid
		cout << "Input was not a square matrix\n";
		//wait for user input before closing
		getchar();
		// die
		exit(1);
	}
	
	//create identity matrix of size rows*rows to hold inverse
	vector < vector <double> > matrixInverse(rows);
	for (int i = 0; i < rows; i++){
		//set each row to have columns number of elements
		matrixInverse[i].resize(columns);
		//set diagonal elements to 1
		matrixInverse[i][i] = 1;
	}

	//---------------Start of Gauss Jordan Elimination---------------//
	
	//variable to hold factor difference of elements and pivots
	double d;

	//iterate over every column and perform pivot
	for (int j = 0; j < columns; j++){

		//-----partial pivot of jth column to reduce rounding error and prevent division by 0-----//
		
		//pivot row is initially jth row
		int pivotRow = j;

		//iterate over all rows j or greater (none lower than j so that finished diagonals are not swapped)
		for (int i = j; i < rows; i++){
			//if jth element in row is higher than current highest
			if (matrixInput[i][j] > matrixInput[pivotRow][j]){
				//update pivotRow to new row number
				pivotRow = i;
			}
		}
		//swap the original pivotRow with the new pivotRow in both matrices
		matrixInput[j].swap(matrixInput[pivotRow]);
		matrixInverse[j].swap(matrixInverse[pivotRow]);

		//-----Diagonalise the matrix-----//

		for (int i = 0; i < rows; i++){
			//do nothing for diagonal elements
			if (i != j){
				//d = factor by which current element is greater than the diagonal element in its column
				d = matrixInput[i][j] / matrixInput[j][j];
				//iterate over the row
				for (int k = 0; k < columns; k++){
					//for each element subtract d * the value of the corresponding column element in the row of pivot
					matrixInput[i][k] -= matrixInput[j][k] * d;
					//do same to working inverse matrix
					matrixInverse[i][k] -= matrixInverse[j][k] * d;
				}
			}
		}
	}

	//-----Reduce to identity matrix-----//

	//iterate over all elements of matrix
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++){
			//divide element in inverse matrix by the value of he diagonal on that row in the working input matrix
			matrixInverse[i][j] /= matrixInput[i][i];
		}
		//set diagonal in row of inputs matrix to 1
		matrixInput[i][i] = 1;
	}

	//---------------Write inverse matrix to file---------------//

	//open new file to write matrix transpose out to
	ofstream outFile("matrix_inverse.txt");

	//if output file opened correctly
	if (outFile.is_open()){
		//cycle through column by row to write to file
		for (int i = 0; i < rows; i++){
			for (int j = 0; j < columns; j++){
				//if not last element of row then add comma
				if (j != columns - 1){
					outFile << matrixInverse[i][j] << ",";
				}
				//if last then make new line
				else{
					outFile << matrixInverse[i][j] << "\n";
				}
			}
		}
	}

	//---------------End of Program---------------//

	//wait for user input before closing
	getchar();

	return 0;
}