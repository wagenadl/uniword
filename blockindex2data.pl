#!/usr/bin/perl -w

use strict;

open IN, "<blockindex.html";
open OUT, ">blocks.txt";

while (<IN>) {
  /"row[01]/ or next;
  <IN> =~ /">(.*?)<\/a>/;
  my $name = $1;
  <IN> =~ /U\+(.*?)</;
  my $start = hex($1);
  <IN> =~ /U\+(.*?)</;
  my $end = hex($1);

  printf OUT "%06x-%06x %s\n", $start, $end, $name;
}
