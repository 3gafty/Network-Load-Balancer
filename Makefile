#
# Makefile networkLoadBalancer
#

cc?=g++

cppflags=-Wall -W -std=c++17

nwb_src=main.cpp parsAddress.cpp parsFreqOfUDP.cpp network_load_balancer.cpp

nwb_obj=$(notdir $(nwb_src:.cpp=.o))

%.o: %.cpp
	$(cc) $(cppflags) -c $^ -o $@

debug: cppflags+=-g3 -O0
debug: nwb

release: cppflags+=-O2
release: nwb

nwb: $(nwb_obj)
	$(cc) $(nwb_obj) -o $@

clean:
	rm -f *.o nwb
