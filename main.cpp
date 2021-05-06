#include "parsAddress.h"
#include "parsFreqOfUDP.h"
#include "networkLoadBalancer.h"

int main(int argc, const char* argv[])
{
	std::cout << "start" << std::endl;
        std::pair<sockaddr_in, std::vector<sockaddr_in>> addrsConnections;
        int freq;

        if (argc < 2) {
                std::cerr << "Not select config file" << std::endl;
                exit(1);
        }
        else if (argc > 2) {
                std::cerr << "To many arguments." << std::endl;
                exit(2);
        }
        else {
                addrsConnections = parsAddress(argv[1]);
                freq = parsFreqOfUDP(argv[1]);
        }

        networkLoadBalancer(addrsConnections, freq);

        return 0;
}