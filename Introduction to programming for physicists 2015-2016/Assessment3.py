#Assessment3
#promgram that asks for two of p, V an T and returns the unkown

molar_gas_constant = 8.314;

print('Welcome to the fabulous Ideal Gas Law calculator.');

def validInput(test_input):
    #only accepts valid input and then returns when it is valid
    #assumes input is not a possitive number
    input_is_number = False;
    input_is_positive = False;
    while input_is_number == False or input_is_positive == False:
        #will run through this loop until input is valid
        try:
            #attempts to turn input to float, if successful update input_is_number
            float_input = float(test_input);
            input_is_number = True;
            #checks input is positive and updates input_is_positive if it is
            if float_input > 0:
                input_is_positive= True
            else:
                #if ont positive ask user to enter a positive number
                print('entered value must be a number greater than 0');
                test_input = input('Please enter again: ');
        except:
            #if failed to convert to float ask fora valid input
            print('entered value must be a number greater than 0');
            test_input = input('Please enter again: ');
    #returns the final input
    return float_input;

#------------------------------------------------------------------------

#set running to true so that loop continues until it is altered inside loop 
running = True;

while running:

    #asks user which variable they wish to enter first
    input_1 = input('Are you going to enter a [t]emperature, a [p]ressure or a [v]olume? ');  

    while input_1.lower() not in ['t', 'p', 'v']:
        #will loop until the user enters a valid variable
        input_1 = input('Sorry, please enter a [t]emperature, a [p]ressure or a [v]olume? type either ,''t'',''p'' or ''v'' ');

    if input_1.lower() == 't':
        #if the variable they want to enter is temperature ask for the value
        input_1_value = input('OK, please tell me the temperature in Kelvin: ');
        #will force the user to enter correct value
        temp = validInput(input_1_value);
        #asks user which other variable they wish to enter
        input_2 = input('Are you going to enter a [p]ressure or a [v]olume? ');

        while (input_2.lower() not in ['p', 'v']):
            #loops until pressure or volume is chosen
            input_2 = input('Sorry, please enter a [p]ressure or a [v]olume? type ''p'' or ''v'' ');

        if input_2.lower() == 'p':
            #if input is pressure ask for value
            input_2_value = input('OK, please tell me the pressure in Pascals: ');
            #check value is valid
            pressure = validInput(input_2_value);
            #calculate and print volume
            volume = temp * molar_gas_constant / pressure;
            print('Volume = ', volume);

        if input_2.lower() == 'v':
            #if input is volume ask for value
            input_2_value = input('OK, please tell me the volume in m^3: ');
            #check value is valid
            volume = validInput(input_2_value);
            #calculate and print pressure
            pressure = temp * molar_gas_constant / volume;
            print('Pressure = ', pressure);


    elif input_1.lower() == 'p':
        #if the variable they want to enter is pressure ask for the value
        input_1_value = input('OK, please tell me the perssure in Pascals: ');
        #will force the user to enter correct value
        pressure = validInput(input_1_value);
        #asks user which other variable they wish to enter
        input_2 = input('Are you going to enter a [t]emperature or [v]olume? ');

        while (input_2.lower() not in ['t', 'v']):
            #loops until temperature or volume is chosen
            input_2 = input('Sorry, please enter a [t]emperature or [v]olume? type ''t'' or ''v'' ');

        if input_2.lower() == 't':
            #if input is temperature ask for value
            input_2_value = input('OK, please tell me the temperature in Kelvin: ');
            #check value is valid
            temp = validInput(input_2_value);
            #calculate and print volume
            volume = temp * molar_gas_constant / pressure;
            print('Volume = ', volume);

        if input_2.lower() == 'v':
            #if input is volume ask for value
            input_2_value = input('OK, please tell me the volume in m^3: ');
            #check value is valid
            volume = validInput(input_2_value);
            #calculate and print temperature
            temp = pressure * volume / molar_gas_constant;
            print('Temperature = ', temp);




    elif input_1.lower() == 'v':
        #if the variable they want to enter is volume ask for the value
        input_1_value = input('OK, please tell me the volume in m^3: ');
        #will force the user to enter correct value
        volume = validInput(input_1_value);
        #asks user which other variable they wish to enter
        input_2 = input('Are you going to enter a [t]emperature or [p]ressure? ');

        while (input_2.lower() not in ['t', 'p']):
            #loops until temperature or pressure is chosen
            input_2 = input('Sorry, please enter a [t]emperature or [p]ressure? type ''t'' or ''p'' ');

        if input_2.lower() == 't':
            #if input is temperature ask for value
            input_2_value = input('OK, please tell me the temperature in Kelvin: ');
            #check value is valid
            temp = validInput(input_2_value);
            #calculate and print pressure
            pressure = temp * molar_gas_constant / volume;
            print('Pressure = ', pressure);

        if input_2.lower() == 'p':
            #if input is pressure ask for value
            input_2_value = input('OK, please tell me the pressure in Pascals: ');
            #check value is valid
            pressure = validInput(input_2_value);
            #calculate and print Temperature
            temp = pressure * volume / molar_gas_constant;
            print('Temperature = ', temp);
            
       #ask user if they wish to run program again     
    again = input('If you would like to run the program again please press ''y'': ');

    if again.lower() != 'y':
        #if they dont type y then end the program
        running = False
        print('Thankyou for using the fabulous Ideal Gas Law calculator.')


    

    

