#!/usr/bin/perl -w

#    Uniword: keyword-based unicode character selector
#    Copyright (C) 2012-2025  Daniel A. Wagenaar <daw@caltech.edu>
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

# We use "NamesList.txt" rather than "UnicodeData.txt" because it contains
# useful information about areas smaller than blocks. The information about
# individual characters is identical.

use strict;

open IN, "<UCD/NamesList.txt" or die;
open ALIAS, ">tables/alias.txt" or die;
open GROUPS, ">tables/groups.txt" or die;
open BLOCKS, ">tables/blocks.txt" or die;
open CHARS, ">tables/chars.txt" or die;

my %alias;

my $groupstart = undef;
my $groupname = undef;
my $lastchar = undef;
my $ignore = 0;
while (<IN>) {
  chomp;
  if (/^\@/) {
    if (/^\@\@\@/) {
       next;
     } elsif (/^\@\@\s/) {
       # Got a block definition
       /^\@\@\s+([0-9A-F]+)\s+(.*?)\s+([0-9A-F]+)\s*$/ or die;
       my $start = hex($1);
       my $name = $2;
       my $end = hex($3);
       $name =~ /\((.*?)\)/ and $name = $1;
       next if $name eq "Unassigned";
       next if $name =~ /Private Use/;
       printf BLOCKS "%06x-%06x %s\n", $start, $end, $name;
     } elsif (/^\@\s/) {
       # Got a group definition
       if (defined $groupstart) {
	 printf GROUPS "%06x-%06x %s\n", $groupstart, $lastchar, $groupname;
       }
       /^\@\s+(.*)\s*$/ or die;
       $groupstart = undef;
       $groupname = $1;
     }
  } elsif (/^([0-9A-Z]+)/) {
    my $char = hex $1;
    /^([0-9A-Z]+)\s+(.*)/ or die;
    my $name = $2;
    $ignore = $name =~ /^<.*>$/;
    next if $ignore;
    $lastchar = hex($1);
    $groupstart = $lastchar if defined $groupname && !defined $groupstart;
    printf CHARS "%06x %s\n", $lastchar, $name;
  } elsif (/^\s*=\s+(.*)$/) {
    next if $ignore;
    my $name = $1;
    $name =~ s/\(.*\)//g;
    $name =~ s/^\s+//;
    $name =~ s/\s+$//;
    push @{$alias{$lastchar}}, $name;
  }
}

for (sort {$a <=> $b } keys %alias) {
  printf ALIAS "%06x %s\n", $_, join("; ", @{$alias{$_}});
}

