binary-logger
=============

Proof of concept for logging facility that unloads formatting from embedded device unto host  
See <https://travis-ci.org/matthiaskraaz/binary-logger> for output of a current test run.

What you find here:
- src contains the sources of a small C library for the embedded device to write log messages in a binary format without formatting
- scripts contains Ruby scripts for the PC to format the binary output from the embedded device into nicely formatted human-readable log messages
- test contains the sources of a demo program that uses the C library to write a few binary log messages
- Makefile builds and runs the demo

