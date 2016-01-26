CXXOPTS := -O0
FOPTS := -Ofast -ipo

.PHONY: all clean
all : LASProc LASProcF geo

clean:
	$(MAKE) -C ../GeoProc clean
	rm -vf LASProc *~ LASProcF *.o *.mod
	
LASProc: LASProc.cpp StdAfx.h Makefile geo
	g++ -o $@ $(CXXOPTS) -I../GeoProc LASProc.cpp ../GeoProc/libGeo.a
	
LASProcF: LASProcF.f90 Makefile geo
	ifort -o $@ $(FOPTS) -I../GeoProc $< ../GeoProc/libGeoF.a
	@echo $@ created

geo:
	$(MAKE) -C ../GeoProc