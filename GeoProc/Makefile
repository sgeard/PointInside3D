SRC := $(wildcard *.cpp)

# Fortran source needs to be ordered because of module dependencies
SRCF := Triangle.f90 Vector.f90 Polyhedron.f90

OBJ := ${SRC:.cpp=.o}
OBJF := ${SRCF:.f90=.o}

CXXOPTS := -O0
FOPTS := -Ofast -ipo

.PHONY: all clean

all: libGeo.a libGeoF.a

libGeo.a: $(OBJ)
	ar r $@ $(OBJ)

libGeoF.a: $(OBJF)
	ar r $@ $(OBJF)
	
clean:
	rm -vf $(OBJ) $(OBJF) *~ *.mod libGeo.a libGeoF.a

$(OBJF): %.o: %.f90
	ifort -c $(FOPTS) -o $@ $<
	
$(OBJ): %.o: %.cpp
	g++ -c $(CXXOPTS) -o $@ $<
