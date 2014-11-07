#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "serviceWrappers.h" // To make the syscalls

// Arg 1 = numRequests - defaults to 10
// Arg 2 = delay - defaults to 1
// Arg 3 = variation - defaults to 3.
// 	Must be greater than three, if not, will default to 3
//		Any Value above 3 will give a preference to children

char passengerNumToChar( int num ){
	// Based on character found we return the enum for them
	switch(num) {
		case 1: 
			return 'C';
		case 2:
			return 'A';
		case 3:
			return 'L';
		default:
			return 'C';//Return children cause easy if nothing else
	}
}

/*
	Getting output/testing - ./a.out && cat /var/log/syslog > output.txt
*/
int main( int argc, char * argv[] ) {
	int i;
	int numRequests = 10;
	int delay = 1;
	int variation = 3;
	/* initialize random seed: */
  	srand (time(NULL));

  	if(argc >= 2){
  		numRequests = atoi(argv[1]);
  		if(argc >= 3){
  			delay = atoi(argv[2]);
  			if(argc >= 4){
  				variation = atoi(argv[3]);
  				if(variation < 3){
  					variation = 3;
  				}
  			}
  		}
  	}
  	else{
  		numRequests = 10;
  	}

 	for(i = 0; i < numRequests; i++){  		
	  	int passType = rand() % variation + 1;//Anything greater than 3 will increase the # of children
	  	int initial = rand() % 5 + 1;
	  	int destination = rand() % 5 + 1;

	  	// while(initial == destination){//Ensures no passenegers are idiots.
	  	// 	destination = rand()%5 +1;
	  	// }
		
		//usleep( delay*1000 );
	  	issue_request(passengerNumToChar(passType), initial, destination);
  	}
	return 0;
}
