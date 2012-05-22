#!/usr/bin/perl -w

#    Uniword: keyword-based unicode character selector
#    Copyright (C) 2012  Daniel A. Wagenaar <daw@caltech.edu>
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

use strict;

my %blocks;
my %blockends;
loadblocks();

open IN, "<UCD/Index.txt";

my %chars;
my %aliases;
my %groups;

while (<IN>) {
  chomp;
  /(.*)?\s+([0-9A-F]+)$/ or next;
  my $name = $1;
  my $char = hex($2);
  unless ($name =~ /,.*,/) {
    if ($name =~ /(.*), (.*)/) {
      $name = "$2 $1";
    }
  }
  next if $char<32;
  next if $char>=127 && $char<160;
  if ($name eq uc($name)) {
    # This is a simple name
    $chars{$char} = $name;
  } elsif ($name eq lc($name)) {
    # This is an alias
    push @{$aliases{$char}}, $name;
  } else {
    # This is a block or an area
    $name .= "; Latin-1 Supplement" if $char eq 0x80;
    $groups{$char} = $name;
  }
}

my $maxchar = 0;
for (keys %chars) {
  $maxchar = $_ if $_>$maxchar;
}

my @groupstart;
push @groupstart, $_ for sort { $a <=> $b } (keys %groups);
my @groupend;
push @groupend, $_ - 1 for (@groupstart);
shift @groupend;
push @groupend, $maxchar;
my %groupends;
$groupends{$groupstart[$_]} = $groupend[$_] for (0..$#groupstart);

for (keys %blocks) {
  if (exists $groups{$_}) {
    if ($groupends{$_} eq $blockends{$_}) {
      delete $groups{$_};
      delete $groupends{$_};
    } else {
      $groupends{$_} = $blockends{$_} if $groupends{$_}>$blockends{$_};
    }
  } else {
    # printf STDERR "Block w/o group: %06x-%06x (%s)\n", \
    #   $_, $blockends{$_}, $blocks{$_};
  }
}


open GROUPS, ">tables/groups.txt";
for (sort { $a <=> $b } (keys %groups)) {
  printf GROUPS "%06x-%06x %s\n", $_, $groupends{$_}, $groups{$_};
}
close GROUPS;

#open CHARS, ">altchars.txt";
#for (sort { $a <=> $b } (keys %chars)) {
#  printf CHARS "%06x %s\n", $_, $chars{$_};
#}
#close CHARS;

open ALIAS, ">tables/alias.txt";
for (sort { $a <=> $b } (keys %aliases)) {
  printf ALIAS "%06x %s\n", $_, join("; ", @{$aliases{$_}});
}
close ALIAS;

######################################################################

sub loadblocks {
  open BLOCKS, "<tables/blocks.txt";
  while (<BLOCKS>) {
    /(......)-(......) (.*)/;
    my $start = hex($1);
    my $end = hex($2);
    my $name = $3;
    $blocks{$start} = $name;
    $blockends{$start} = $end;
  }
  close BLOCKS;
}
