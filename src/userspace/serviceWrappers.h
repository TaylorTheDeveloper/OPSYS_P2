#include <sys/syscall.h>
#include <unistd.h>

//x86_64 bit syscalls
#define START 318
#define STOP 319
#define REQUEST 320

//Wrapers for each system call
inline long start_shuttle(void){ return syscall(START); }
inline long stop_shuttle(void){ return syscall(STOP); }
inline long issue_request(const char passenger_type, const int initial_terminal, const int destination_terminal){ 
	return syscall(REQUEST,passenger_type,initial_terminal,destination_terminal); }