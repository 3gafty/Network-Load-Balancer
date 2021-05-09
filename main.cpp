#include "parsAddress.h"
#include "parsFreqOfUDP.h"
#include "network_load_balancer.h"
#include "closing_message.h"

#include <iostream>
#include <csignal>
#include <memory>

void handleSignal(int signal);

auto Nlb = std::make_unique<mynamespace::NetworkLoadBalancer>();

int main(int argc, const char* argv[])
{
	signal(SIGINT, handleSignal);
	signal(SIGTERM, handleSignal);
	try
	{
		std::cout << "Start Program" << std::endl;

		if (argc < 2) {
			throw std::runtime_error(std::string("Not select config file"));
		}
		else if (argc > 2) {
			throw std::runtime_error(std::string("To many arguments."));
		}
		else {
			std::string fileName(argv[1]);
			Nlb->setConnections(parsAddress(fileName));
			Nlb->setNumberOfMessagesPerSecond(parsFreqOfUDP(fileName));
		}

		Nlb->run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch(...) {
		std::cerr << "unknown exception!" << std::endl;
		return 2;
	}

	std::cout << "Stop program" << std::endl;
	return 0;
}

void handleSignal(int signal) {
	switch(signal) {
		case SIGINT:
			Nlb->stop();
			closingMessage();
		break;

		case SIGTERM:
			Nlb->stop();
			closingMessage();
		break;

		default:
			std::cerr << "!unknown signal!" << std::endl;
	}
}
