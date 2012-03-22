#!/usr/bin/perl -w

use strict;

my %blocks;
my %blockends;
loadblocks();

open IN, "<charindex.html";

my %chars;
my %aliases;
my %groups;

while (<IN>) {
  chomp;
  /<td>(.*?)<\/td><td><a href=\"PDF\/U(.*?)\.pdf\">(.*?)<\/a><\/td>/ or next;
  my $name = $1;
  my $ref = hex($2);
  my $char = hex($3);
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
      #printf STDERR "Group w/o block: %06x-%06x (%06x; %s)\n", $_, $groupends{$_}, $blockends{$_}, $groups{$_};
      $groupends{$_} = $blockends{$_} if $groupends{$_}>$blockends{$_};
    }
  } else {
    printf STDERR "Block w/o group: %06x-%06x (%s)\n", $_, $blockends{$_}, $blocks{$_};
  }
}


open GROUPS, ">groups.txt";
for (sort { $a <=> $b } (keys %groups)) {
  printf GROUPS "%06x-%06x %s\n", $_, $groupends{$_}, $groups{$_};
}
close GROUPS;

open CHARS, ">:encoding(UTF-8)", "chars.txt";
for (sort { $a <=> $b } (keys %chars)) {
  printf CHARS "%06x %c %s\n", $_, $_, $chars{$_};
}
close CHARS;

open ALIAS, ">:encoding(UTF-8)", "alias.txt";
for (sort { $a <=> $b } (keys %aliases)) {
  printf ALIAS "%06x %c %s\n", $_, $_, join("; ", @{$aliases{$_}});
}
close ALIAS;

######################################################################

sub loadblocks {
  open BLOCKS, "<blocks.txt";
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
