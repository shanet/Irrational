# Irrational
#### Shane Tully (shane@shanetully.com)
#### shanetully.com 

A simple pi calculator built with MPFR (http://www.mpfr.org).

## Prerequisties

To compile and run you'll need libmpfr as well as the development headers. Everything else is standard C.

## Compiling

Just run the usual "make". It's a pretty simple compile.

There are two versions available. The default, target "machin", is multithreaded and faster than the second version. The second being a recurrance based calculation (target: "recurrance"). Either of them builds a binary file called "irrational".

## Usage

For convience, "make test" will calculate pi to ~1,000,000 digits. This should take less than a minute (especially on a multicore processor), or maybe two minutes on older hardware. The program can be directly run by with:

    ./irrational [--hide-pi] [precision]

Where "--hide-pi" supresses output of pi and precision is the number of correct digits to calculate (default is 1 million).

The program will look for a text file, "pi.txt", in the same directory as the binary file to compare the digits of pi it has calculated with known digits to determine precision. If the file does not exist, this step is simply skipped. A text file with the first 1 billion digits of pi can be downloaded at: http://stuff.mit.edu/afs/sipb/contrib/pi/

If "--hide-pi" is not requested, pi is output to standard output. The timing and precision output is output to standard error so pi can be redirected to a file if desired.

## Internals

Obviously the source code is available for study, but a more detailed discussion of how this works is available at: TODO

## Support
This program is a little side project and carries no warranty or support from its author. However, bugs and feature requests may be submitted to the GitHub repo linked to above.


## Legal
This program is open source software. It is free to distribute, modify, and use with the exception of it being made closed source or sold for commercial purpose swithout the consent of the author.