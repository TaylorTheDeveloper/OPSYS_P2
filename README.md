OPSYS_P2
========

Operating Systems - Project 2: Kernel Level Airport Shuttle Simulator

Swedish Airport Shuttle Simulator: Implementing system calls through the module
Taylor Brockhoeft
Justin Block

Scheduling Algorithm:
	For our project, we decided to use the typewrite algorithm, or, a very slow c-scan.
	Basically, Our shuttle starts in the middle of the terminals and 
	progressivly moves in one direction. Once we are at the end of the terminals,
	we then move to the begining and start again. This continues until the shuttle stops.

For Starters, download this version of the linux kernel:
	wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.16.4.tar.xz

Now that you have that, let's go over what's in this package:

Under the 'src' folder, you'll find the following folders:

'syscalls' folder: (All of these files need to be updated within the kernel you just downloaded)
	syscalls.h     - Syscall prototypes added to linux file
		- Goes in "linux-3.16.4/include/linux"
	Makefile	   - Compiles Syscalls with kernel
		- Goes in "linux-3.16.4/" (main directory)
	syscall_64.tbl - This is the modified table file. 
		- Goes in "linux-3.16.4/arch/x86/syscalls"

'opsyscalls' folder: (These are the implementations for the linkage to the module for our syscalls)
	- Place this folder in the primary Linux Kernel Directory: "/linux-3.16.4/"
	syscall.h      - Syscall prototypes for our new system calls
	syscall.c      - Syscall asmlinkage's for the system calls
	Makefile	   - Makefile for new system calls

'Module' folder: (The Shuttle/Terminal simulation module)
	terminal.c     - the terminal module
		- Creates the proc file entry for the terminal in /proc/
	Makefile	   - Makefile for the Loadable Kernel Module (LKM)
	shuttle.c 	   - Contains Loading/Unloading shuttle code. Resets Shuttle and Terminal
	shuttle.h      - Header file for shuttle and terminal objects
	passenger.c    - Contains the Empty passengers and utils
	passenger.h    - Header file for the passenger

'userspace'
	proc.sh	       - Preview Proc Entry, updates every second
	start.c	       - Starts Shuttle
			- Compile With gcc -o start start.c
	stop.c	       - Stops Shuttle
			- Compile With gcc -o stop stop.c
	issue.c        - Issues a single request (Adult from terminal 1, to 5)
			- Compile With gcc -o issue issue.c
	issue_multiple.c - Issues N random requests with a delay D, and a deviation X (X >= 3)
			- Compile With gcc -o issue_multiple issue_multiple.c
			- All Params are optional. N defaults to 10, D defaults to 1, X defaults to 3
			- Usage: ./issue_multiple <N> <D> <X>
			- Ex: ./issue_multiple 500 5 3
				- Issues 500 random requests, with a delay of 5 milliseconds, and a deviation of 3
			- Deviation:
				- 3 will give equal preference to Adults, Adults with luggage, and Children
				- Anything higher will give preference to more children
				- If the deviation was 3:
					- 1/3 Adults with Luggage
					- 1/3 Adults w/o luggage
					- 1/3 Children
				- If the deviation was 10
					- 1/10 Adults with Luggage
					- 1/10 Adults w/o Luggage
					- 8/10 Children
			
		
	

