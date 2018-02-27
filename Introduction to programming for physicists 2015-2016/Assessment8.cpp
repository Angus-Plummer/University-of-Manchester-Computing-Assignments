/* Angus Plummer - University of Manchester - Introduction to programming for Physicists. Assessment 8: Optimisation 

Program to find optimum location for a postal hub in the uk, given town and city data from GBPlaces.csv

The program has been improved on the basic task outline by:
	
	Increasing the precision of the search for local minimum when it cant be improved further. This is done until a desired precision is reached.
	
	Weighting the distance to each town/city by its population.

	Taking user input for number of random starts to use with hill climbing
		- Only accepts positive integer values
		- Informs the user if they input a number larger than recommended and then asks for confirmation to continue
	
	Informing the user of which town is nearest to the found "global" minimum
	
	Improving the Hill Climbing algorithm by devising a 3x3 cell system where the fitnesses of adjacent test positions around the current hub location are not evaluated if they have already been evaluated at the previous hub location
		- This nearly halves the number of fitness evaluations required
*/

#define _USE_MATH_DEFINES

#include<math.h>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<ctime>
#include<cstdio>

//namespace shortcut
using namespace std;

//integer variable to hold total number of iterations performed in search;
//int totalEval;

struct Place{
	string name;
	double population;
	double latitude;
	double longitude;
};

//function to return great circle distance between two places given their longitude and latitude
double distanceBetween(Place place1, Place place2){
	//convert longitude and latitude of both places in radians
	double latitude1 = place1.latitude * M_PI / 180;
	double latitude2 = place2.latitude * M_PI / 180;
	double longitude1 = place1.longitude * M_PI / 180;
	double longitude2 = place2.longitude * M_PI / 180;

	//difference in the two longitudes and latitudes
	double dLat = latitude2 - latitude1;
	double dLon = longitude2 - longitude1;

	//calculate haversine for the two lat lon pairs
	double haversine = pow(sin(dLat / 2), 2) + cos(latitude1) * cos(latitude2) * pow(sin(dLon / 2), 2);
	//calculate great circle distance
	double distance = 6371000 * 2 * asin(sqrt(haversine));
	//return distance
	return distance;
}

//function to return the sum of weighted distances of a place to a vector of places
double fitness(vector<Place> places, Place base){
	//variable to hold total sum
	double sum = 0;
	//iterates over all places and adds the distance to * place population
	for (int i = 0; i < places.size(); i++){
		sum += distanceBetween(base, places[i]) * places[i].population * pow(10,-12);
	}
	//totalEval += 1;
	//return the total sum
	return sum;
}

//function to generate a random number between an upper and lower limit in divisions of range/n
double randomNumber(double upper, double lower, int n) {
	double r;
	r = lower + (rand() % (n + 1) * (1. / n) * (upper - lower));
	return r;
}

int main(){

	//---------------read in file---------------//

	//declare empty vector to hold all the cities/towns of type Place
	vector < Place>  places;

	//open the file to read in
	ifstream inFile("GBPlaces.csv");

	//declare string, line, to hold each town/city data
	string aLine;

	//variable to change after first line so that first line is ignored
	bool isFirstLine = 1;

	// check if it opened successfully
	if (inFile.is_open()) {
		// file is open so read it in

		// while not at end of file
		while (!inFile.eof()) {
			//set aLine to current line of file
			getline(inFile, aLine);
			//create a stringstream of line to read through char at a time
			stringstream ss(aLine);
			//declare new vector to hold elements as strings
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
			//if not first line then create a place with read in components
			if (isFirstLine == 0){
				//add the data to a newly declared place and then add the new place to vector of places
				Place newPlace;
				newPlace.name = aLineVect[0];
				newPlace.population = stod(aLineVect[2]);
				newPlace.latitude = stod(aLineVect[3]);
				newPlace.longitude = stod(aLineVect[4]);
				places.push_back(newPlace);
			}
			//if it is the first line then do nothing and update isFirstLine to false
			else{
				isFirstLine = 0;
			}
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

	//---------------start of optimization---------------//
	//set boolean variable running to be 1 when doing optimisation to allow user to retry without reopening program
	bool running = 1;
	while (running){

		//declare variables to hold current lat and lon and hub global min lat and lon
		double x, y, glx, gly = 0;
		//variables to hold direction of hill climb direction
		int dx = 0;
		int dy = 0;
		//variables to hold current step size, starting step size and minimum step size to determine precision of local minimum
		double step;
		double maxStep = 0.25;
		double minStep = 0.0001;

		//set total function evaluation count to 0
		//totalEval = 0;

		//3x3 boolean array to store whether the adjacent positions around test hub location have been previously tested. 1 if untested. 0 if tested and hence greater than current minimum
		bool untestedCells[3][3];

		//variables to hold evaluated fitness values for hill climbing cells
		double value, newValue, oldValue = 0;
		//variable for the global minimum fitness value. Initally set to a value higher than any possible
		double globalMin = pow(10, 20);

		//number of random starts for hill climbing
		int iterations = 0;
		//number of iterations over which will ask for user confirmation
		int maxIterations = 50;

		//ask user how many iterations they wish the hill climbing to use
		cout << "How many random starts do you want the hill climbing optimisation to use?: ";
		//iterations must be positive
		while (iterations <= 0){
			//while input not type integer...
			while (!(cin >> iterations)){
				//clear input and tell user output was invalid
				cin.clear();
				//ignore all characters until after new line
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//ask for valid input
				cout << "\nInvalid input.  Try again: ";
			}
			//if not positive
			if (iterations <= 0){
				//clear input
				cin.clear();
				//ask for valid input
				cout << "\nMust have a positive number of iterations.  Try again: ";
			}
			//requested number of iterations is above recommended number
			if (iterations > maxIterations){
				//clear input
				cin.clear();
				//inform user of potential issue and ask if they wish to continue
				cout << "\n" << iterations << " is a large number of iterations. Computation may take a while.\nDo you still wish to continue? Input y for yes: ";
				//declare new string to hold confirmation and await user input
				string confirmation;
				cin >> confirmation;
				//if user did not input "y"
				if (!(confirmation == "y")){
					//clear input
					cin.clear();
					//set iterations to -1 so loop begins again
					iterations = -1;
					//let user know the loop has reset
					cout << "\nHow many random starts do you want the hill climbing optimisation to use?: ";
				}
			}
		}

		//inform user that the optimisation process is starting
		cout << "\nStarting search for optimum hub location with " << iterations << " iterations.\n\nSearching";
		//place called hub to hold the current hub position
		Place hub;

		//initialise random number function with current time as seed
		srand(time(NULL));

		//main loop for hill climbing repeats
		for (int k = 0; k < iterations; k++){
			//set step to the starting step size at beginning of each hill climb
			step = maxStep;

			// first pick a random starting point for x and y between lat -4,1 and lon 51,55 each in 100 divisions
			x = randomNumber(-4, 1, 100);
			y = randomNumber(51, 55, 100);

			//set hub position to random location
			hub.latitude = x;
			hub.longitude = y;

			//set value to fitness of hub starting position
			value = fitness(places, hub);

			// reset untestedCells to say all untested by iterating over and setting to 1
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++){
					untestedCells[i][j] = 1;
				}
			}
			//set centre cell to 0 as does not need to be evaluated
			untestedCells[1][1] = 0;

			//while searching for a better position: searching = 1
			bool searching = 1;

			while (searching){
				//set old value of position to oldvalue, so value can be used for new fitness values
				oldValue = value;

				//iterate over 3x3 grid around current hub position, exluding current position, to find direction of greatest descent
				for (int i = -1; i <= 1; i++){
					for (int j = -1; j <= 1; j++){
						//for unevaluated position test new hub position
						if (untestedCells[i + 1][j + 1]){
							//set hub position to new position on grid surrounding old position
							hub.latitude = x + step * i;
							hub.longitude = y + step * j;
							//note fitness value for new position
							newValue = fitness(places, hub);
							//if lower than newValue lower then save into value
							if (newValue <= value) {
								//update direction of greatest negative gradient to i and j
								dx = i;
								dy = j;
								value = newValue;
							}
						}
					}
				}

				// update to new hub position
				x += step * dx;
				y += step * dy;

				//------update untestedCells with which cells around the new hub position need to be evaluated-----

				//reset untestedCells so that all say tested
				for (int i = 0; i < 3; i++){
					for (int j = 0; j < 3; j++){
						untestedCells[i][j] = 0;
					}
				}
				//if not in middle column:
				if (dx != 0){
					//iterate over column and set values to 1 as they haven't been tested
					for (int j = 0; j < 3; j++){
						untestedCells[dx + 1][j] = 1;
					}
				}
				//do same as above for rows
				if (dy != 0){
					for (int i = 0; i < 3; i++){
						untestedCells[i][dy + 1] = 1;
					}
				}

				//if there was no improvement on the local minimum value
				if (value >= oldValue){
					//if the stepsize is less than the minimum stepsize then stop searching
					if (step < minStep){
						searching = 0;
					}
					//else decrease by factor of 5
					step /= 5;
					//and reset untested cells to say all untested by iterating over and setting to 1
					for (int i = 0; i < 3; i++){
						for (int j = 0; j < 3; j++){
							untestedCells[i][j] = 1;
						}
					}
					//set centre cell to 0;
					untestedCells[1][1] = 0;

				}
			}
			//finished a hill climb so compare result to previous local minima
			//if lower than current "global" minimum
			if (value < globalMin){
				//set new local min as new global minimum
				globalMin = value;
				//set lon and lat hub locations for global minimum to new values
				glx = x;
				gly = y;
			}

			//print out . to tell user still searching
			cout << ".";
		}

		//finished all hill climbs so set hub location to location of global minimum
		hub.latitude = glx;
		hub.longitude = gly;

		//find closest town to global minimum
		//initially set closest town to first town to allow comparisons
		string hubTown = places[0].name;
		double minDistance = distanceBetween(hub, places[0]);
		//iterate over all places
		for (int i = 1; i < places.size(); i++){
			//store distance from hub to place
			double distance = distanceBetween(hub, places[i]);
			//if this distance is lower than current minimum
			if (distance < minDistance){
				//store the closest towns name
				hubTown = places[i].name;
				//set new minimum distance to current
				minDistance = distance;
			}
		}


		//print out result to user
		cout << "\nFinished\n\nOptimum Hub Location:\n\nLatitude: " << glx << ", Longitude: " << gly << ". Nearest Town: " << hubTown << "\n";

		//print out total function evaluations
		//cout << "\ntotal function evaluations: " << totalEval << "\n";

		//ask user if the wish to run the program again
		cout << "\nIf you wish to run the program again, enter 'y': ";
		//variable to store input
		string repeat;
		//take input
		cin >> repeat;
		//if input is 'y' then restart optimisation
		if (!(repeat == "y")){
			running = 0;
		}
	}
	return 0;
}