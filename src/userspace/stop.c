#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "serviceWrappers.h" // To make the syscalls

char passengerNumToChar( int num );

/*
	Getting output/testing - ./a.out && cat /var/log/syslog > output.txt
*/
int main( int argc, char * argv[] ) {
	stop_shuttle();
	return 0;
}
