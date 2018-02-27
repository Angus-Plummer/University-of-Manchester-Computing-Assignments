#Assessment5
#A visual representation of British Town and City data from GBPlaces.csv

import numpy as np;
import matplotlib.pyplot as plt;
from mpl_toolkits.axes_grid.anchored_artists import AnchoredText;

#file is initially unopened
readFiles = False;

try:
    #open file to read from
    readFile = open('GBPlaces.csv','r');
    #import map of GB
    GBMap = plt.imread('GBMap.png');
    #if got here then file has been read, update readFiles
    readFiles = True;
except:
    print('There was an error reading in the files. Please ensure they are in the same directory as Assessment5.py');
    
#if files were read then run rest of program
if readFiles:

    #create empty array to hold the towns and cities    
    GBplaces = [];

    #read the file line by line
    for line in readFile:
        #split the line by comma
        lineItems = line.split(',');
        #remove the carriage return from the end the line
        lineItems[4] = lineItems[4].rstrip();
        #add the line to the array
        GBplaces.append(lineItems);
    
    #close the file  
    readFile.close();  
    
    #deletes the first line of the file as it is not needed
    del GBplaces[0];

    #scale of the city sizes
    scale = 1/400;

    #create empty arrays for town/city data
    name = [];
    population = [];
    longitude = [];
    latitude = [];
    area = [];
    colour = [];
    

    #pass through GBplaces array place by place
    for place in GBplaces:
        #add name, population, longitude and latitude of place to respective array
        name.append(place[0]);
        population.append(place[2]);
        latitude.append(place[3]);
        longitude.append(place[4]);
        #scatter plot circle area: p/log(p) * scale factor
        area.append(float(place[2]) / np.log(float(place[2])) * scale);
        #if place is city then make circle red
        if place[1] == 'City':
            colour.append('r');
            #if place is town make circle blue
        else:
            colour.append('b');
            
    #define figure and axis variables        
    fig, ax = plt.subplots();        
            
    #plot map of GB, scaled and positioned to right position
    implot = ax.imshow(GBMap, extent = [-10.78,2.22,49.75,59]);

    #label axes and title
    plt.xlabel('Latitude', fontsize=12);
    plt.ylabel('Longitude', fontsize=12);
    plt.suptitle('British Towns and Cities', fontsize=14, fontweight='bold');
    
    #create global anchored text box artist
    global at;
    #initialise text box with instructions
    at = AnchoredText('Click on a town or city for more information', prop=dict(size=10), frameon=True, loc=2);
    at.patch.set_boxstyle("round,pad=0.,rounding_size=0.2");
    #add the text box to the axes
    ax.add_artist(at);    
    
    #define function for handling mouse click event on circles from scatter plot
    def onpick(event):
        global at;
        #remove the current text box when click event is triggered
        at.remove();
        #store index of clicked town circle
        ind = event.ind;
        #store the data about the town as separate strings
        place = np.take(name, ind)[0];
        pop = np.take(population, ind)[0];
        lon = np.take(longitude, ind)[0]; 
        lat = np.take(latitude, ind)[0];
        #if circle is red then it is a city
        if np.take(colour, ind)[0] == 'r':
            size = 'City';
        #otherwise it is a town
        else:
            size = 'Town';
        #create string to be in text box
        textdata = size + ' - ' + place + ', Population: ' + pop + ', Latitude: ' + lat + ', Longitude: ' + lon;
        #create new anchored text box and add to axes
        at = AnchoredText(textdata , prop=dict(size=10), frameon=True, loc=2);
        at.patch.set_boxstyle("round,pad=0.,rounding_size=0.2");
        ax.add_artist(at);
        #update the plot with the new text box
        plt.draw();
        
    #create scatter graph from the data in arrays
    col = ax.scatter(longitude, latitude, s=area, c=colour, alpha=0.5, picker = True);
    #connect onpick function to figure
    fig.canvas.mpl_connect('pick_event', onpick)    
    #show the plot
    plt.show();
