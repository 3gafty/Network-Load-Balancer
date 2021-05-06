#include "parsAddress.h"
#include "parsFreqOfUDP.h"
#include "networkLoadBalancer.h"

#include <iostream>

int main(int argc, const char* argv[])
{
        try
        {
                std::cout << "start" << std::endl;
                std::pair<sockaddr_in, std::vector<sockaddr_in>> addrsConnections;
                int freq;

                if (argc < 2) {
                        throw std::runtime_error(std::string("Not select config file"));
                }
                else if (argc > 2) {
                        throw std::runtime_error(std::string("To many arguments."));
                }
                else {
                        std::string fileName(argv[1]);
                        addrsConnections = parsAddress(fileName);
                        freq = parsFreqOfUDP(fileName);
                }

                networkLoadBalancer(addrsConnections, freq);
        }
        catch(const std::exception& e)
        {
                std::cerr << e.what() << '\n';
                return 1;
        }
        return 0;
}
