#!/usr/bin/perl -w

use strict;

open IN, "<UnicodeData.txt" or die;
open OUT, ">:encoding(UTF-8)", "morechars.txt" or die;

while (<IN>) {
  chomp;
  my ($code, $desc, @junk) = split(/;/, $_);
  next if $desc =~ /</;
  addshort(hex($code), $desc);
}

sub addshort {
  my $c = shift;
  return if $c<32;
  return if $c>=127 && $c<160;
  my $d = shift;
  printf OUT "%06x %c %s\n", $c, $c, $d;
}
