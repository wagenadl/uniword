#!/bin/sh

rm -f uniword
rm -rf src/.rcc src/.obj src/.moc
rm -f src/Makefile

debuild -us -uc
