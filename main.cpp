#include "parsAddress.h"
#include "parsFreqOfUDP.h"
#include "networkLoadBalancer.h"

#include <iostream>
#include <csignal>
#include <cstring>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

void handleSignal(int signal);

void closingMessage();

volatile bool run{true};

int main(int argc, const char* argv[])
{
	signal(SIGINT, handleSignal);
	signal(SIGTERM, handleSignal);
	try
	{
		std::cout << "Start Program" << std::endl;
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

	std::cout << "Exit program" << std::endl;
	return 0;
}

void handleSignal(int signal) {
	switch(signal) {
		case SIGINT:
			run = false;
			closingMessage();
		break;

		case SIGTERM:
			run = false;
			closingMessage();
		break;

		default:
			std::cerr << "!unknown signal!" << std::endl;
	}
}

void closingMessage()
{
	int finalizer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (finalizer < 0) {
		std::cerr << "Socket failed. The errno value is: " + std::to_string(errno) << std::endl;
	}
	sockaddr_in sendAddrFinal;
	memset(&sendAddrFinal, 0, sizeof(sendAddrFinal));
	sendAddrFinal.sin_family = AF_INET;
	sendAddrFinal.sin_port = htons(12345);
	sendAddrFinal.sin_addr.s_addr = inet_addr("127.0.0.1");

	int opt = 1;
	if (setsockopt (finalizer, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "Setsockopt failde. The errno value is: " + std::to_string(errno) << std::endl;
	}

	std::string byby("exit");
	socklen_t sizeAddr = sizeof(sendAddrFinal);
	int senderrFinal = sendto(finalizer, &byby, byby.size() + 1, 0, (struct sockaddr *)&sendAddrFinal, sizeAddr);
	if (senderrFinal < 0) {
		std::cerr << "Error sendto. The errno value is : " << errno << std::endl;
	}
	close(finalizer);
}
