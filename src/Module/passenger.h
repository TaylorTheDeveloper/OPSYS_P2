#pragma once

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>

// PassengerTypes

enum PassengerType { CHILD, ADULT_NO_LUGGAGE, ADULT_LUGGAGE, OTHER };

// Returns the type of passenger found
enum PassengerType getPassengerType(char passenger_type);

// Struct for each passenger
struct Passenger{
		long int waitTime;
		enum PassengerType passengerType;
		int initialTerminal;
		int destinationTerminal;
		int cap;//flesh value
		struct list_head list;
};


// Functions
void emptyPassengers(struct list_head * list);
/*
	Gets the value of the passenger based on the type,

	All values are doubled since we cannot handle floats/doubles
*/
int getPassengerValue(enum PassengerType passengerType){
	switch(passengerType){
		case CHILD:
			return 1;
		case ADULT_NO_LUGGAGE:
			return 2;
		case ADULT_LUGGAGE:
			return 4;
		default:
			return 0;
	}
}