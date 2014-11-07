#!/bin/bash
make clean; make; rmmod terminal.ko; insmod terminal.ko; cat /proc/terminal