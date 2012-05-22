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

all: uniword

TABLES=tables/alias.txt tables/blocks.txt tables/chars.txt tables/groups.txt

uniword: $(TABLES)
	( cd src; qmake )
	make -C src

tables/alias.txt: tools/getIndex.pl tables/blocks.txt UCD/Index.txt
	tools/getIndex.pl

tables/blocks.txt: tools/getBlocks.pl UCD/Blocks.txt
	tools/getBlocks.pl

tables/chars.txt: tools/getData.pl UCD/UnicodeData.txt
	tools/getData.pl

tables/groups.txt: tools/getIndex.pl tables/blocks.txt UCD/Index.txt
	tools/getIndex.pl

.PHONY: all
