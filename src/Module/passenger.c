#include "passenger.h"

/*
	Emptys the list of passengers so a new list can be put
	in.
*/
	
void emptyPassengers( struct list_head * list ) {

	struct Passenger * passenger = NULL, * tempPassengera = NULL; 

	// Goes through and removes the passengers
	list_for_each_entry_safe ( passenger, tempPassengera, list, list ) 
	{ 
		// Removes from the list
		list_del(&(passenger->list));
		// Allows other processes to take over
		kfree( passenger );
	}
}

/*
	Returns the type of passenger the user is based on the passed in character
	this way we dont have to check all the alternative cases
*/
enum PassengerType getPassengerType(char passenger_type) {

	// Based on character found we return the enum for them
	switch(passenger_type) {

		case 'C': 
			return CHILD;
		case 'A':
			return ADULT_NO_LUGGAGE;
		case 'L':
			return ADULT_LUGGAGE;
		default:
			return OTHER;

	}
}
