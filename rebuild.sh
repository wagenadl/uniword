#!/bin/sh


# Create blocks.txt from UCD
./getBlocks.pl

# Create chars.txt from UCD
./getData.pl

# Create aliases and groups from UCD
./getIndex.pl

# Make the program
qmake
make
