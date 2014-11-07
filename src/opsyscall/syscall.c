// PrintK and Kern Macros
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/export.h>
#include <linux/linkage.h>

// Stubs for kernel module implementation
long ( * STUB_start_shuttle )( void ) = NULL;
long ( * STUB_stop_shuttle )( void ) = NULL;
long ( * STUB_issue_request )( char passenger_type, int initial_terminal, int destination_terminal ) = NULL;
// Make out syscalls availble for module access
EXPORT_SYMBOL( STUB_start_shuttle );
EXPORT_SYMBOL( STUB_stop_shuttle );
EXPORT_SYMBOL( STUB_issue_request );

//test Call
asmlinkage long sys_opsyscall(const char *test){

	printk(KERN_ALERT "Called with string: %s\n", test);
	return 0;//0 for sucess

}

/* System call wrappers.  If the stub is not NULL, it will be run, 
otherwise returns -ENOSYS */

//Start Shuttle
asmlinkage long sys_start_shuttle(){
	if ( STUB_start_shuttle ){
		return STUB_start_shuttle();//0 for sucess
		printk(KERN_ALERT " START SHUTTLE \n\n");
	}
	printk( KERN_WARNING "Attempting to Use \"start_shuttle\" failed\n" );
	return -ENOSYS;
}
//Stop The Shuttle!
asmlinkage long sys_stop_shuttle(){
	if( STUB_stop_shuttle ){
		return STUB_stop_shuttle();//0 for sucess
		printk(KERN_ALERT " STOP SHUTTLE \n\n");
	}
	printk( KERN_WARNING "Attempting to Use \"stop_shuttle\" failed\n" );
	return -ENOSYS;

}
//issue a new passenger request
asmlinkage long sys_issue_request(const char passenger_type, const int initial_terminal, const int destination_terminal){
	if ( STUB_issue_request ){
		printk(KERN_ALERT " REQUEST ISSUIED \n\n");
		return STUB_issue_request( passenger_type, initial_terminal, destination_terminal );//0 for sucess
	}
	printk( KERN_WARNING "Attempting to Use \"issue_request\" failed\n");
	return 0;

}