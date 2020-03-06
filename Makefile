CXX := g++

CXXFLAGS := -fPIC -Wall -Wextra -fdiagnostics-color=always -fmax-errors=10

all: gfft_test gfft_test2

gfft_test: src/example.cpp inc/AssocVector.h inc/Factory.h inc/NullType.h inc/Typelist.h inc/TypeManip.h inc/LokiTypeInfo.h inc/gfft.h
	$(CXX) $(CXXFLAGS) -o $@ $^ -Iinc

gfft_test2: src/example2.cpp inc/AssocVector.h inc/Factory.h inc/NullType.h inc/Typelist.h inc/TypeManip.h inc/LokiTypeInfo.h inc/gfft.h
	$(CXX) $(CXXFLAGS) -o $@ $^ -Iinc

clean:
	rm -f gfft_test
	rm -f gfft_test2
