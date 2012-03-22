#!/usr/bin/perl -w

use strict;

open IN, "<List_of_Unicode_characters.html" or die;
open OUT, ">:encoding(UTF-8)", "morechars.txt" or die;

while (<IN>) {
  chomp;
  if (/<td>U\+(.*?)<\/td>/) {
    addlong(hex($1));
  } elsif (/<td title="U\+(.*?): (.*?)">/) {
    addshort(hex($1), $2);
  }
}

sub addshort {
  my $c = shift;
  return if $c<32;
  return if $c>=127 && $c<160;
  my $d = shift;
  printf OUT "%06x %c %s\n", $c, $c, $d;
}

sub addlong {
  my $c = shift;
  my $done = 0;
  while (<IN>) {
    /<tr>/ and last;
    /<td>(.*)<\/td>/ or next;
    next if $done;
    my $d = $1;
    $d =~ s/<.*?>//g;
    next if length($d)<5;
    next if $d =~ /&.*?;/;
    $d =~ s/\[.*?\]//g;
    addshort($c, $d);
    $done = 1;
  }
}
