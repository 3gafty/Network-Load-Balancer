#include "network_load_balancer.h"

#include <iostream>
#include <chrono>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>

#define SIZEBUFF 1024
#define ONE_SECOND 1000

namespace mynamespace {

	NetworkLoadBalancer::NetworkLoadBalancer()
	{}

	void NetworkLoadBalancer::setConnections(std::pair<sockaddr_in, std::vector<sockaddr_in>>&& t_conn)
	{
		connections_ = std::move(t_conn);
	}

	void NetworkLoadBalancer::setNumberOfMessagesPerSecond(unsigned int t_freq)
	{
		numberOfMessagesPerSecond_ = t_freq;
	}

	void NetworkLoadBalancer::run()
	{
		using namespace std::chrono;

		run_ = true;

		int listener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (listener < 0) {
			throw std::runtime_error("Socket failed. The errno value is: " + std::to_string(errno));
		}

		int opt = 1;
		if (setsockopt (listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
			throw std::runtime_error("Setsockopt failed. The errno value is: " + std::to_string(errno));
		}

		if (bind(listener, (struct sockaddr *)&connections_.first, sizeof(connections_.first)) < 0) {
			throw std::runtime_error("Bind failed. The errno value is: " + std::to_string(errno));
		}

		int num{0};
		unsigned int dgCounter{0};
		auto lastTimePoint{steady_clock::now()};
		int senderr{1};
		while (run_) {
			char buf[SIZEBUFF];
			int err = recvfrom(listener, buf, SIZEBUFF, MSG_TRUNC, 0, 0);
			if (err > SIZEBUFF) {
				std::cerr << "PacketSize more then SIZEBUFF. Not implemented case" << std::endl;
			}
			else if (err > 0 && err <= SIZEBUFF) {
				auto currentTimePoint = steady_clock::now();
				if (duration_cast<milliseconds>(currentTimePoint - lastTimePoint).count() >= ONE_SECOND) {
					dgCounter = 1;
					lastTimePoint = currentTimePoint;
				}
				else {
					++dgCounter;
					if (dgCounter > numberOfMessagesPerSecond_) {
						continue;
					}
				}
				if (run_) {
					do {
						sockaddr_in clientAddr = connections_.second[num];
						socklen_t sizeAddr = sizeof(clientAddr);
						senderr = sendto(listener, buf, err, 0, (struct sockaddr *)&clientAddr, sizeAddr);
						if (senderr < 0) {
							std::cerr << "Error sendto. The errno value is : " << errno << std::endl;
						}
						++num;
						num %= connections_.second.size();
					}
					while (senderr < 0);
				}
			}
			else {
				std::cerr << "Error resieved. The errno value is : " << errno << std::endl;
			}
		}
	}

	void NetworkLoadBalancer::stop()
	{
		run_ = false;
		closingMessage();
	}

	NetworkLoadBalancer::~NetworkLoadBalancer()
	{
		close(listener_);
		std::cout << "exit NetworkLoadBalancer" << std::endl;
	}

	void NetworkLoadBalancer::closingMessage()
	{
		int finalizer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (finalizer < 0) {
			std::cerr << "Socket failed. The errno value is: " + std::to_string(errno) << std::endl;
		}
		std::string byby("exit");
		socklen_t sizeAddr = sizeof(connections_.first);
		int senderrFinal = sendto(finalizer, &byby, byby.size() + 1, 0, (struct sockaddr *)&connections_.first, sizeAddr);
		if (senderrFinal < 0) {
			std::cerr << "Error sendto. The errno value is : " << errno << std::endl;
		}
		close(finalizer);
	}
}