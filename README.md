# Uniword 0.2.0 - Keyword-based unicode character selector

## Installation

Binaries for the latest release may be found at 
[GitHub](https://github.com/wagenadl/uniword/releases/latest).

To compile from source on Linux or Windows, you need Qt and CMake. 

## Usage

To find a unicode character by its name or by any of its aliases,
type words or parts of words into the text entry field at the top of
the window. For instance, you could type “arrow left” to get a list of
left pointing arrows. Only characters that match all of your search
terms are displayed.

For each of your search terms, if there is an exact match in the
database to that term, partial matches are ignored. Thus, in the above
example, the fact that the word “left” occurs in the database,
prevents your search from matching many characters that have the word
“leftward” in their names. To get more results, you could search for
“arrow lef” instead.

To exclude a keyword, prefix the search term with an exclamation
point. For instance, “arrow lef !righ” shows only those left-pointing
arrows that do not also point to the right.


To search in the list of block and area names, prefix the search
term with a period. For instance, “.mathem” will search for
mathematics-related characters. (The term “.math” finds only a very
small number of characters.)

No more than 100 characters are displayed for a given search. If
the list ends in “...”, you know that you should be more specific.

You can use the mouse to select one or more characters in the
output panel. If you select precisely one, the selection will be
automatically copied to the clipboard so that you can easily paste it
into other programs.

Besides ordinary characters, the Unicode database also contains a
set of “combining marks”. If your search term matches any of these,
the mark is represented in combination of a small gray square, but the
combining mark is copied to the clipboard without that
square. Similarly, the database contains some “spacing modifier
letters”. These are represented following the same small square,
which, again, is not copied to the clipboard.

Some characters are invisible, such as “Em space” (U2003). These are
represented with underlining. Again, the underlining is not copied to
the clipboard.

Names of characters and groups are derived from the file
“NamesList.txt” that is part of the Unicode Character Database which
may be found at
[unicode.org/Public/UNIDATA](https://unicode.org/Public/UNIDATA).
A copy of this file is also included with the source package for
Uniword, under the terms of the 
[Unicode License](https://www.unicode.org/license.txt). 
You may find it helpful
to browse this file to get a sense for the kind of words that the
Unicode Consortium uses to describe characters.

The menu option “View/Font...” can be used to select the font used
for displaying characters. It is worth noting that not all fonts have
equally good coverage of the full Unicode range. Some operatings fill
the gaps by “borrowing” glyphs from other fonts, which can be quite
confusing. Uniword has no way to detect such anomalies in general, but
the font selection box can be used to filter for fonts that advertise
an ability to handle certain alphabets.

