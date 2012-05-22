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
