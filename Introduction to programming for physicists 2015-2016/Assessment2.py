#Program to calculate kinetic energy from a given mass and velocity
# define kinetic energy function. Returns float of kinetic energy or prints error and returns -1 if there is an error
def kineticEnergy(mass, velocity):

    try:
        floatMass = float(mass);    #attempts to convert mass and velocity into floats
        floatVelocity = float (velocity);
    except:              #if this fails, return -1 and request user inputs number values
        print("Mass and velocity must be numbers");
        return -1; 
    
    if floatMass < 0:   #throws an exception if the mass is positive, returns -1 and tells user problem
        print("Mass must be positive");
        return -1; 

    elif floatVelocity > 300000000: #if velocity is greater than speed of light, return -1 and tell user problem
        print("Velocity must be lower than the speed of light");
        return -1;

    else:
        return 0.5 * floatMass * floatVelocity * floatVelocity;  #returns float value of kinetic energy
    
m = 1;
v = 1;
ke = kineticEnergy(m, v);

if ke == -1:  #if there was an error in ke then tell the user other wise print statement about ke
    return("There was an error in the input for kineticEnergy");
else:
    print('A mass of %6.2f kg travelling with velocity %8.2e m/s has kinetic energy = %8.2e J' % ( m,v,ke ));   
