#pragma once

#include <linux/list.h>
#include <linux/list_sort.h>
#include "passenger.h"

#define NUM_TERMINALS 5
#define MAX_PASS 50 // Maximum Capacity is 25 units of flesh, times two for float issues
#define CHILDFLESH 1
#define ADULTFLESH 2
#define LUGGAGEFLESH 4

struct Terminal {
	//List of passangers at a given terminal
	struct list_head passengers;
	//Counts for each terminal
	int passenger_Counts_Children;
	int passenger_Delivered_Children;
	int passenger_Counts_Adult;
	int passenger_Delivered_Adult;
	int passenger_Counts_Luggage;
	int passenger_Delivered_Luggage;
};
/*shuttle data*/
enum Status {OFFLINE, DEACTIVATING, MOVING,PARKED};//0,1,2,3 respectfully
static const char * statusString[4] = { "OFFLINE", "DEACTIVATING", "MOVING", "PARKED" };//String Versions

struct Shuttle {
	enum Status   status;//Offline, Deactivating, Moving, Parked
	int   numPassengers; //Max 25
	int   currentTerminal;//0,1,2,3,4
	int   destinationTerminal;//Null
	struct list_head passengers;
	int adult;
	int children;
	int luggage;
};

struct Terminal terminals[NUM_TERMINALS];
struct Shuttle shuttle;

/*
*	Resets the shuttle params
*/
void resetShuttle( struct Shuttle * shuttle );
/*
*	These Return passengers from shuttles
*/
// int shuttleChildren( struct Shuttle * shuttle );
// int shuttleAdult( struct Shuttle * shuttle );
// int shuttleLuggage( struct Shuttle * shuttle );
/*
*	Resets the terminal params
*/
void resetTerminal( struct Terminal * terminal );
/*
*   Calculates the sum of delivered passengers from one terminal
*/
int sumDelivered( struct Terminal * terminal ); 

/*
	Drops the passengers off and keeps track of the passengers dropped off

	Parameters -

		struct Shuttle* shuttle - the shuttle
		struct Terminal * terminal - the terminal info
*/
int dropPassengersOff(struct Shuttle* shuttle, struct Terminal terminal[]);

/*
	Resets amount of delivered passengers
*/
void resetDeliveredCount(struct Terminal * terminal);