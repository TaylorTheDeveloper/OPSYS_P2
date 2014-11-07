#include "shuttle.h"

//Typewriter Algorithm
int pickNextTerminal(int curr){
	int dest = 1;
	switch(curr){
		case 1: dest = 2;
		break;
		case 2: dest = 3;
		break;
		case 3: dest = 4;
		break;
		case 4: dest = 5;
		break;
		case 5: dest = 1;
		break;
	}
	return dest;
}

void resetShuttle( struct Shuttle * shuttle ) {
	shuttle->status = OFFLINE;//Shuttle Offline
	shuttle->numPassengers = 0;// With Zero Passengers
	shuttle->currentTerminal = 3;//At Terminal 3, the 3rd terminal
	shuttle->destinationTerminal = 3;//Destination is set to 2nd because Offline
	shuttle->children = 0;
	shuttle->adult = 0;
	shuttle->luggage = 0;
}

void resetTerminal( struct Terminal * terminal){
	//Clear Counters
	terminal->passenger_Counts_Children = 0;
	terminal->passenger_Delivered_Children = 0;
	terminal->passenger_Counts_Adult = 0;
	terminal->passenger_Delivered_Adult = 0;
	terminal->passenger_Counts_Luggage = 0;
	terminal->passenger_Delivered_Luggage = 0;
}

/*
* 
*/
int sumDelivered(struct Terminal * terminal){
	return terminal->passenger_Delivered_Luggage + terminal->passenger_Delivered_Adult + terminal->passenger_Delivered_Children;
}


/*
	Drops the passengers off and keeps track of the passengers dropped off

	Parameters -

		struct Shuttle * shuttle - the shuttle
		struct Terminal * terminal - the terminal info
*/
int dropPassengersOff(struct Shuttle * shuttle, struct Terminal terminal[]) {

	struct Passenger  * passenger = NULL;
	struct Passenger * tempPassengers = NULL;
	int droppedOff = 0;
	
	// Checks if each passenger should get off (doubly-linked list)
	list_for_each_entry_safe(passenger, tempPassengers, &shuttle->passengers, list) 
	{ 
		// If we are at the same terminal as the destination then
		// we remove the passenger
		if (shuttle->currentTerminal == passenger->destinationTerminal) {

			// Decrease the passenger count based on the passenger type
			// and increases the amount of passengers that have been delivered
			switch(passenger->passengerType){
				case CHILD:
					shuttle->children--;
					terminal[passenger->initialTerminal-1].passenger_Delivered_Children++;
					break;
				case ADULT_NO_LUGGAGE:
					shuttle->adult--;
					terminal[passenger->initialTerminal-1].passenger_Delivered_Adult++;
					break;
				case ADULT_LUGGAGE:
					shuttle->luggage--;
					terminal[passenger->initialTerminal-1].passenger_Delivered_Luggage++;
					break;
				default:
					return 0;
			}

			// Change the number of passengers based on the type
			shuttle->numPassengers -= getPassengerValue(passenger->passengerType);

			// Delete the passenger from the list on the shuttle
			list_del(&passenger->list);
			kfree(passenger);

			++droppedOff;
		}
			
	}

	// Return the number of passengers dropped off so we know if
	// extra time is needed as a result of unloading too many passengers
	return droppedOff;
}

void resetDeliveredCount(struct Terminal * terminal){
	//Clear Counters
	terminal->passenger_Delivered_Children = 0;
	terminal->passenger_Delivered_Adult = 0;
	terminal->passenger_Delivered_Luggage = 0;
}

/*
	Inserts passengers onto the shuttle
*/
int insertPassengers(struct Shuttle * shuttle, struct Terminal terminal[]) {

	int insertedPassenger = 0;
	// Used for list itteration
	struct Passenger * passenger = NULL;
	struct Passenger * tempPassengers = NULL;

	// Loop through all people waiting at the terminal and decide if they should get on or not
	// Goes from start which has already been sorted
	list_for_each_entry_safe (passenger, tempPassengers, &terminals[shuttle->currentTerminal-1].passengers, list) 
	{ 

		// If we can't add anymore passengers we can stop now
		if (shuttle->numPassengers >= (MAX_PASS)){
			break;
		}
		// Only pick up this passenger if they can fit (we have the capacity and they won't make us go over 25)
		else if (getPassengerValue(passenger->passengerType) + (shuttle->numPassengers) <= (MAX_PASS)){

			//Delete the passenger from the list and add them to the shuttle passenger list
			list_del(&passenger->list);
			list_add(&passenger->list, &shuttle->passengers);

			// Increase number of passengers (weighted value)
			shuttle->numPassengers += getPassengerValue(passenger->passengerType);

			// Decrease the amount of people waiting at the terminal by 1
			switch(passenger->passengerType){
				case CHILD:
					terminal[shuttle->currentTerminal-1].passenger_Counts_Children--;
					shuttle->children++;
					break;
				case ADULT_NO_LUGGAGE:
					terminal[shuttle->currentTerminal-1].passenger_Counts_Adult--;
					shuttle->adult++;
					break;
				case ADULT_LUGGAGE:
					terminal[shuttle->currentTerminal-1].passenger_Counts_Luggage--;
					shuttle->luggage++;
					break;
				default:
					return 0;
			}

			insertedPassenger++;
		}

	}

	return insertedPassenger;
}

