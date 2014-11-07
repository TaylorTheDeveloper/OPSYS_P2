#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "serviceWrappers.h" // To make the syscalls

char passengerNumToChar( int num );

/*
	Getting output/testing - ./a.out && cat /var/log/syslog > output.txt
*/
int main( int argc, char * argv[] ) {

	//issue_request('A', 1, 5);
	stop_shuttle();
	//system( "clear" );
	printf( "Shuttle stopped - 1\n");

	return 0;
}
