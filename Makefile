all: simpleCostMap

simpleCostMap: simpleCostMap.cpp
	g++ -g $^ -o $@ -Wall `pkg-config opencv --cflags --libs`
