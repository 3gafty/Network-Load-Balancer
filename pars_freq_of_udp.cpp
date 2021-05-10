#include "pars_freq_of_udp.h"

#include <iostream>
#include <fstream>

int parsFreqOfUDP(const std::string& f)
{
	std::ifstream input(f);
	if (!input) {
		throw std::runtime_error("File could not be opened for reading!");
	}

	std::string freq;
	int value;
	while (input >> freq) {
		if (freq == "FREQ") {
			input >> value;
			break;
		}
	}

	std::cout << "The freq of messages is " << value << " per second." << std::endl;
	input.close();

	return value;
}
