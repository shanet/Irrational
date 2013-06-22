# Irrational
#### Shane Tully (shane@shanetully.com)
#### shanetully.com 

A simple pi calculator built with MPFR (http://www.mpfr.org).

## Prerequisties

To compile and run you'll need libmpfr as well as the development headers. Everything else is standard C.

## Compiling

* Install the MPFR development package. On Ubuntu this is libmpfr-dev.
* Just run `make`.

There are two versions available. The default, target "machin", is multithreaded and faster than the second version. The second being a recurrance based calculation (target: "recurrance"). Either of them builds a binary file called "irrational".

## Usage

For convenience, "make test" will calculate pi to ~1,000,000 digits. This should take less than a minute (especially on a multicore processor), or maybe two minutes on older hardware. The program can be directly run by with:

    ./irrational [--hide-pi] [precision]

Where "--hide-pi" supresses output of pi and precision is the number of correct digits to calculate (default is 1 million).

The program will look for a text file, "pi.txt", in the same directory as the binary file to compare the digits of pi it has calculated with known digits to determine precision. If the file does not exist, this step is simply skipped. A text file with the first 1 billion digits of pi can be downloaded at: http://stuff.mit.edu/afs/sipb/contrib/pi/

If "--hide-pi" is not requested, pi is output to standard output. The timing and precision output is output to standard error so pi can be redirected to a file if desired.

## Internals

Obviously the source code is available for study, but a more detailed discussion of how this works is available at: http://shanetully.com/2012/11/calculating-pi-to-10000000-digits-with-mpfr-and-threads/

## Support
This program is a little side project and carries no warranty or support from its author. However, bugs and feature requests may be submitted to this GitHub repo.


## License
Copyright (C) 2012-2013 Shane Tully

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

