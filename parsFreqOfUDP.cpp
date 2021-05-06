#include "parsFreqOfUDP.h"

int parsFreqOfUDP(const char* f)
{
        std::ifstream input(f);
        if (!f)
	{
		std::cerr << "File could not be opened for reading!" << std::endl;
		exit(4);
	}

        std::string freq;
        int value;
        while(input >> freq) {
                if (freq == "FREQ") {
                        input >> value;
                        break;
                }
        }

        std::cout << "The freq of messages is " << value << " per second." << std::endl;
        input.close();

        return value;
}