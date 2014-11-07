#pragma once

#include <linux/err.h>
#include <linux/kthread.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/time.h>

// Prototype Declarations
long start_shuttle( void );
long stop_shuttle( void );
long issue_request( char passenger_type, int initial_terminal, int destination_terminal );