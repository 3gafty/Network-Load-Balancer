#include "pars_address.h"

#include <unistd.h>
#include <netinet/in.h>

#include <iostream>
#include <fstream>
#include <cstring>
#include <exception>

std::pair<sockaddr_in, std::vector<sockaddr_in>> parsAddress(const std::string& t_str)
{
	std::ifstream input(t_str);
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

	for (int i{0}; i < value; ++i)
	{
		input >> port >> ip;
		std::cout << "port_" << (i + 1) << " - " << port << " ip - " << ip << std::endl;
		sockaddr_in sen_aAddr;
		memset(&sen_aAddr, 0, sizeof(sen_aAddr));
		sen_aAddr.sin_family = AF_INET;
		sen_aAddr.sin_port = htons(port);
		sen_aAddr.sin_addr.s_addr = inet_addr(ip.c_str());
		cl.push_back(sen_aAddr);
	}

	input >> port >> ip;
	std::cout << "Incomin from port - " << port << " ip - " << ip << std::endl;
	sockaddr_in incomin_udp;
	memset(&incomin_udp, 0, sizeof(incomin_udp));
	incomin_udp.sin_family = AF_INET;
	incomin_udp.sin_port = htons(port);
	incomin_udp.sin_addr.s_addr = inet_addr(ip.c_str());

	input.close();

	return {incomin_udp, cl};
}
