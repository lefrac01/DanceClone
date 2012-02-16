all: 
#	cd Generic; $(MAKE) -f Makefile
#	cd Generic/WiiDash; $(MAKE) -f Makefile
	cd games/DanceClone; $(MAKE) -f Makefile

clean: 
#	cd Generic; $(MAKE) -f Makefile clean
#	cd Generic/WiiDash; $(MAKE) -f Makefile clean
	cd games/DanceClone; $(MAKE) -f Makefile clean

install: 
#	cd Generic; $(MAKE) -f Makefile install
#	cd Generic/WiiDash; $(MAKE) -f Makefile install
	cd games/DanceClone; $(MAKE) -f Makefile install

