#!/usr/bin/perl -w

use strict;

open IN, "<UCD/Blocks.txt";
open OUT, ">tables/blocks.txt";

while (<IN>) {
  /^#/ and next;
  /^([0-9A-F]+)\.\.([0-9A-F]+); (.*)/ or next;
  my $start = hex($1);
  my $end = hex($2);
  my $name = $3;

  printf OUT "%06x-%06x %s\n", $start, $end, $name;
}
