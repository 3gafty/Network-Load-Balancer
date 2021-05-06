#include "parsAddress.h"

std::pair<sockaddr_in, std::vector<sockaddr_in>> parsAddress(const std::string& f)
{
        std::ifstream input(f);
        if (!input)
	{
		throw std::runtime_error("File could not be opened for reading!");
	}

        int value;
        input >> value;

        int port;
        std::string ip;
        std::vector<sockaddr_in> cl;
        cl.reserve(value);

        for (int i{0}; i < value; ++i) {
                input >> port >> ip;
                std::cout << "port_" << (i + 1) << " - " << port << " ip - " << ip << std::endl;
                sockaddr_in sendAddr;
                memset(&sendAddr, 0, sizeof(sendAddr));
                sendAddr.sin_family = AF_INET;
		sendAddr.sin_port = htons(port);
		sendAddr.sin_addr.s_addr = inet_addr(ip.c_str());
                cl.push_back(sendAddr);
        }

        input >> port >> ip;
        std::cout << "Incomin from port - " << port << " ip - " << ip << std::endl;
        sockaddr_in incominUDP;
        memset(&incominUDP, 0, sizeof(incominUDP));
        incominUDP.sin_family = AF_INET;
	incominUDP.sin_port = htons(port);
	incominUDP.sin_addr.s_addr = inet_addr(ip.c_str());

        input.close();

        return {incominUDP, cl};
}
