#
# Makefile networkLoadBalancer
#

cc?=g++

cppflags=-Wall -W -std=c++17

nwb_src=main.cpp parsAddress.cpp parsFreqOfUDP.cpp networkLoadBalancer.cpp

nwb_obj=$(notdir $(nwb_src:.cpp=.o))

%.o: %.cpp
	$(cc) $(cppflags) -c $^ -o $@

debug: cppflags+=-g3 -O0
debug: nwb

release: cppflags+=-O2
release: nwb

nwb: $(nwb_obj)
	$(cc) $(nwb_obj) -o $@ -lm -lpthread

clean:
	rm -f *.o nwb
