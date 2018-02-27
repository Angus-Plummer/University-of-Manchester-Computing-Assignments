#program to sort file with listed cities into ascending order of population

try:
    #attempt to open file
    readFile = open('GBplaces.csv','r');
except:
    #an error occured when opening
    print('An error occured when trying to open the file');
    
#create empty array to hold the towns and cities    
GBplaces = [];

#read the file line by line
for line in readFile:
    #split each line by comma
    lineItems = line.split(',');
    #remove the carriage return from the end of each line
    lineItems[4] = lineItems[4].rstrip();
    #add the line to the array
    GBplaces.append(lineItems);
    
#deletes the first line of the file as it is not needed
del GBplaces[0];

#function to return popluation as float to be compared and sorted
def getKey(item):
    #converts third element on line into float for comparison
    return float(item[2]);

#create new array sorted by population
GBplacesByPop = sorted(GBplaces, key=getKey);

#open new file to be written to
writeFile = open('output.csv','w');    
    
#add key to top of file
writeFile.write('% place,type,population,latitude,longitude\n');

#cycle through the sorted array
for place in GBplacesByPop:
    #cycle through the elements of each town
    for i in range(0,len(place)):
        #for last element of each town
        if i == len(place) -1:
            #write element and write carriage return
            writeFile.write(place[i] + '\n');
        else:
            #if any other element, write element and comma
            writeFile.write(place[i] + ',');
    
#close the files    
readFile.close();   
writeFile.close(); 
