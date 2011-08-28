#!/bin/sh

#NOTE: must be run as superuser
echo 0 > /proc/sys/kernel/nmi_watchdog
opcontrol --no-vmlinux --session-dir=/home/dan/PROJECTS/ewsraytracer/build/oprofile_session 
opcontrol --init
opcontrol --start-daemon