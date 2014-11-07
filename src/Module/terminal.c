#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/time.h> 
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/list.h> // for list
#include <linux/init.h>
#include <linux/kthread.h>

#include "shuttle.c" 
#include "passenger.c" 

#define SECOND 10
#define DEFAULT_WAIT 10
#define EXTRA_WAIT 3
#define EXTRA_THRESH 4
#define TRAVEL_TIME 30

/*
  Make sure to be in sudo mode first (sudo -i) recommended

  To run
  rmmod terminal; make; insmod terminal.ko
  or
  sudo rmmod terminal; make; sudo insmod terminal.ko

  Then 
  cat /proc/terminal
*/

MODULE_LICENSE("GPL");


//Mutexs
struct mutex mutex_terminal;
struct mutex mutex_shuttle;

// For thread
struct task_struct *threadTask;

// Structs
struct Terminal terminals[5];
struct Shuttle shuttle;

bool is_exiting = false;

// Terminals proccess file
static struct proc_dir_entry* proc_file;


bool DEBUGMOD = true;//Debugging Flag
// Stubs for kernel module shuttle services
extern long ( * STUB_start_shuttle )( void );
extern long ( * STUB_stop_shuttle )( void );
extern long ( * STUB_issue_request )( char passenger_type, int initial_terminal, int destination_terminal );
long start_shuttle(void);
long stop_shuttle(void);
long issue_request( char , int , int );

/*Updates Terminal*/
static int terminal_show(struct seq_file *m, void *v) {
  int i;

  mutex_lock( &mutex_shuttle );
  mutex_lock( &mutex_terminal );
  
  seq_printf(m,"Welcome to Sweden Shuttle. Where everyone is equal. Excapt the Dainish. $#*& the Dainish.\n");
  //Print Status, seats used
  seq_printf( m, "\nStatus:\t\t%s\nSeats:\t\t%d used %d available\n",
        statusString[shuttle.status], //Status
          ((shuttle.luggage * 4 +shuttle.adult * 2 + shuttle.children)/2), //seats used
          ((MAX_PASS/2)-(shuttle.luggage * 4 +shuttle.adult * 2 +shuttle.children)/2) ); // seats available

  // Print out passengers on terminal shuttle with type
    seq_printf( m, "Passengers:\t%d (%d adult with luggage, %d adult without luggage, %d children)",
        shuttle.luggage+shuttle.adult+shuttle.children,
        shuttle.luggage,
        shuttle.adult,
        shuttle.children );

    seq_printf( m, "\nLocation:\t%d\nDestination:\t%d\n",
      shuttle.currentTerminal,
      shuttle.destinationTerminal );

    // Print out delivered passengers and type
    seq_printf( m, "Delivered:\t%d (%d adult with luggage, %d adult without luggage, %d children)\n-----\n",
        sumDelivered(&terminals[0]) + sumDelivered(&terminals[1]) + sumDelivered(&terminals[2]) + 
        sumDelivered(&terminals[3]) + sumDelivered(&terminals[4]),
        terminals[0].passenger_Delivered_Luggage + terminals[1].passenger_Delivered_Luggage +
        terminals[2].passenger_Delivered_Luggage + terminals[3].passenger_Delivered_Luggage +
        terminals[4].passenger_Delivered_Luggage  ,
        terminals[0].passenger_Delivered_Adult + terminals[1].passenger_Delivered_Adult +
        terminals[2].passenger_Delivered_Adult + terminals[3].passenger_Delivered_Adult +
        terminals[4].passenger_Delivered_Adult  ,
        terminals[0].passenger_Delivered_Children + terminals[1].passenger_Delivered_Children +
        terminals[2].passenger_Delivered_Children + terminals[3].passenger_Delivered_Children +
        terminals[4].passenger_Delivered_Children  );

    // Print out terminal info
    for(i = 0; i < 5; ++i ) {
          seq_printf( m, "Terminal %d: %d adult with luggage, %d adult without luggage, %d children in queue. %d passengers delivered so far.\n", 
            i+1,
            terminals[i].passenger_Counts_Luggage,
            terminals[i].passenger_Counts_Adult,
            terminals[i].passenger_Counts_Children,
            sumDelivered(&terminals[i]));
    }

  mutex_unlock( &mutex_terminal );
  mutex_unlock( &mutex_shuttle );

  return 0;//Done Printing
}

static int terminal_open(struct inode *inode, struct  file *file) {
  return single_open(file, terminal_show, NULL);
}

static const struct file_operations terminal_fops = {
  .owner = THIS_MODULE,
  .open = terminal_open,
  .read = seq_read,
  .release = single_release,
};


/*
  Only needed to move passengers to terminals see slide 4

  Called when shuttle begins running, when the shuttle is set
  to stop the shuttle drops off all the remaining passengers
  (not picking anymore up) and then closes the thread when
  done.
*/
int thread_run(void *data){
  int tempPassengers = 0;
  int nextTerminal;
  int waitTime;
  int i = 0;
  //reset shuttle and terminals should be handled within start shuttle
  
    mutex_lock( &mutex_shuttle );
    mutex_lock( &mutex_terminal );
          // Set the shuttle and terminal to default  
        resetShuttle(&shuttle);
        for(i=0; i < NUM_TERMINALS; i++){
        //   resetTerminal(&terminals[i]);
            // Resets the counter for the amount of people delivered
            resetDeliveredCount(&terminals[i]);
         }
    mutex_unlock( &mutex_shuttle );
    mutex_unlock( &mutex_terminal );

  while(!is_exiting || (shuttle.status == DEACTIVATING && shuttle.numPassengers != 0)){// While not offline, keep running

    mutex_lock( &mutex_shuttle );
    mutex_lock( &mutex_terminal );

    nextTerminal = pickNextTerminal(shuttle.currentTerminal);

    //Reset
    tempPassengers = 0;
    waitTime = 0;

    if(shuttle.status != DEACTIVATING){
      shuttle.status = PARKED;
    }

    // Drops the passengers off at the terminal
    tempPassengers += dropPassengersOff(&shuttle, terminals);
    
    //If We're not deactiviating, load more passengers
    if(shuttle.status != DEACTIVATING){
      tempPassengers += insertPassengers(&shuttle, terminals);
    }
      //shuttle.destinationTerminal = nextTerminal(shuttle.currentTerminal);//Write This, have it handle the equals case
    mutex_unlock( &mutex_shuttle );
    mutex_unlock( &mutex_terminal );

    // Wait an extra amount of time if we have unloaded/loaded more than 4 passengers
    waitTime = tempPassengers > EXTRA_THRESH ?  tempPassengers - EXTRA_THRESH : 0;   
    msleep( SECOND * ( DEFAULT_WAIT + ( waitTime * EXTRA_WAIT ) ) );

    // DO NOT lock a sleep - found this out the hard way :(
    //msleep(1000);//Unload/Load Time //OLD

    mutex_lock( &mutex_shuttle );
    if(shuttle.status != DEACTIVATING){
      shuttle.status = MOVING;
    }
    mutex_unlock( &mutex_shuttle );
    
    // It takes abs( X - Y ) * 30 seconds to get from one terminal to another
    msleep( abs( shuttle.currentTerminal - shuttle.destinationTerminal ) * ( TRAVEL_TIME * SECOND ) );
    //msleep(3000);//Move Time //OLD

    mutex_lock( &mutex_shuttle );
    shuttle.currentTerminal = shuttle.destinationTerminal;
    mutex_unlock( &mutex_shuttle );

        //Next Terminal
    mutex_lock( &mutex_shuttle );
    shuttle.destinationTerminal = nextTerminal;
    mutex_unlock( &mutex_shuttle );

}

mutex_lock( &mutex_shuttle );
shuttle.status = OFFLINE;
mutex_unlock( &mutex_shuttle );

  return 0;
}


/**
 * Terminal Init
 *    Sets up a proc file, sets syscall stubs correctly
 */
static int __init terminal_init(void) {

  int i;
  proc_file = proc_create("terminal", 0, NULL, &terminal_fops); //Initlize proc file
  
  // If it wasn't created end with error
  if(proc_file == NULL){
    return -ENOMEM;
  }

  //initilize mutexs
  mutex_init(&mutex_terminal);
  mutex_init(&mutex_shuttle);

  if(IS_ERR(threadTask) !=  0){
    printk(KERN_ERR "Shuttle service did not properly create the thread");
    return -1;
  }

  // Initialize the linked lists
  INIT_LIST_HEAD(&shuttle.passengers);

  for(i = 0; i < 5; i++){
    INIT_LIST_HEAD(&terminals[i].passengers);
  }

  shuttle.status = OFFLINE;

  resetShuttle(&shuttle);
  for(i=0; i<NUM_TERMINALS; i++){
    resetTerminal(&terminals[i]);
  }
  //Clear PassengerList
  
  // Redirect stub syscalls to our implementation
  STUB_start_shuttle = &start_shuttle;
  STUB_stop_shuttle = &stop_shuttle;
  STUB_issue_request = &issue_request;

  return 0;
}

/*
  Stops the thread
*/
void exit_thread(void){
  bool isLocked = true;

  mutex_lock(&mutex_shuttle);

  // Stop thread
  // Only do when shuttle is not online
  if(shuttle.status != OFFLINE){
    isLocked = false;
    is_exiting = true;

    mutex_unlock(&mutex_shuttle);

    // Exit thread now
    kthread_stop(threadTask);
  }

  // If shuttle is still locked, unlock it
  if(isLocked == true){
    mutex_unlock(&mutex_shuttle);
  }
}

/**
 * Terminal Exit
 *    Removes the proc file, tears down stubs
 */
static void __exit terminal_exit(void) {
  int i;

  STUB_start_shuttle = NULL;
  STUB_stop_shuttle = NULL;
  STUB_issue_request = NULL;
  remove_proc_entry("terminal", NULL);

  // Stops the thread
  exit_thread();

  mutex_lock(&mutex_shuttle);
  mutex_lock(&mutex_terminal);
  resetShuttle( &shuttle );
  for(i=0; i<NUM_TERMINALS; i++){
    resetTerminal(&terminals[i]);
  }
  mutex_unlock(&mutex_shuttle);
  mutex_unlock(&mutex_terminal);

  // Don't need mutexs anymore
  mutex_destroy(&mutex_shuttle);
  mutex_destroy(&mutex_terminal);
}
/*
Starts the shutttle. If Successful, return 0;
*/
long start_shuttle(){
  long result = 0;
  
    mutex_lock( &mutex_shuttle );
    //If offline, then start!
      if ( shuttle.status == OFFLINE ) {
        shuttle.status = PARKED;
        is_exiting = false;
        printk( KERN_DEBUG "Shuttle Starting.\n" );

        threadTask = kthread_run( thread_run, NULL, "ShuttleThread" );

        if ( IS_ERR( threadTask ) != 0 ) {          
          printk( KERN_ERR "Failed to create shuttle thread.\n" );
          result = -1;// Failure!
        }
        else{
          result = 0; //Success!
        }

      } 
      else { 
        result = 1; //Shuttle already started
      }
    mutex_unlock( &mutex_shuttle );

  return result;
}
/*
Stops the shutttle. If Successful, return 0;
*/
long stop_shuttle(){

  long result = 0;

  printk( KERN_DEBUG "Shuttle Stopped.\n" );

  mutex_lock( &mutex_shuttle );
    // Check if shuttle is Offline or Deativiating
  if ( shuttle.status == OFFLINE || shuttle.status == DEACTIVATING ) {
     printk( KERN_DEBUG "Shuttle is already offline/deactiviating when calling stop shuttle.\n" );
     result = 1; //exit
  } //Otherwise, shuttle must be either parked or moving
  else{
    shuttle.status = DEACTIVATING;
  }
    
  mutex_unlock( &mutex_shuttle );

  if(shuttle.status != OFFLINE || shuttle.status != DEACTIVATING ){
    exit_thread();
  }

  return result;
}
/*
Issues a passenger request in the form of <pass_type> <inital> <destination>
*/
long issue_request(char passenger_type, int initial_terminal, int destination_terminal){
  
  // Gets the type of passenger the user is
  enum PassengerType passengerType;

  struct Passenger * new_passenger;


  // Good old ISO C90 requires we set after declaring
  passengerType = getPassengerType(passenger_type);

    // If were issuing a request for the passenger to go to the terminal their already at we do nothing
  if (initial_terminal == destination_terminal){
    mutex_lock(&mutex_terminal);
      switch(passengerType){
        case CHILD:
          terminals[initial_terminal-1].passenger_Delivered_Children++;

        case ADULT_NO_LUGGAGE:
          terminals[initial_terminal-1].passenger_Delivered_Adult++;

        case ADULT_LUGGAGE:
          terminals[initial_terminal-1].passenger_Delivered_Luggage++;

        default:
          return 0;
      }
    mutex_unlock(&mutex_terminal);
    return 0;
  }


  // Creating a new null passenger - passenger container
  new_passenger = NULL;

  // If the passenger is at an actual terminal, their trying to go to an actual terminal and 
  // they are a valid passenger then we store the information
  if (initial_terminal > 0 && initial_terminal <= 5 && destination_terminal > 0 && destination_terminal <= 5 && passengerType != OTHER) {

    // Setup new passenger (kmalloc so dynamically allocated)
    new_passenger = kmalloc(sizeof(struct Passenger), GFP_KERNEL);

    // Now we set their initial, destination and type
    new_passenger->initialTerminal = initial_terminal;
    new_passenger->destinationTerminal = destination_terminal;
    new_passenger->passengerType = passengerType;

    // Gets the head of the list (wont change so we don't need to be inside the lock)
    INIT_LIST_HEAD(&new_passenger->list);

    // Lock the terminal so we can add the passenger
    mutex_lock(&mutex_terminal);

    // Add passenger to terminal
    list_add(&new_passenger->list, &terminals[initial_terminal-1].passengers);

    /*
      passenger_Counts_Luggage
      passenger_Counts_Adults
      passenger_Counts_Children
    */
    // Increase the amount of passengers at the terminal we just inserted to
    switch(passengerType){
      case CHILD:
        terminals[initial_terminal - 1].passenger_Counts_Children += 1;
        break;
      case ADULT_LUGGAGE:
        terminals[initial_terminal - 1].passenger_Counts_Luggage += 1;
        break;
      case ADULT_NO_LUGGAGE:
        terminals[initial_terminal - 1].passenger_Counts_Adult += 1;
        break;
      default:
        printk(KERN_DEBUG "Invalid passenger type entered");
        break;
    }

    // Unlock so other passengers can be added
    mutex_unlock(&mutex_terminal);
    
  }
  // Otherwise the passenger type being inserted is invalid
  else{
    printk(KERN_DEBUG "Shuttle Service used bad issue request. Using passenger_type %c, initial_terminal %d, destination_terminal %d\n", passenger_type, 
      initial_terminal, destination_terminal);
    return 1; // Returning 1 as error code

  }

  return 0; // If we reach this point all is well

}

module_init(terminal_init);
module_exit(terminal_exit);