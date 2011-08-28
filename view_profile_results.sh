#!/bin/sh
opreport -g --session-dir=`pwd`/build/oprofile_session --demangle=smart --symbols `pwd`/build/raytracer
