#include "networkLoadBalancer.h"

#include <iostream>
#include <chrono>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>

void networkLoadBalancer(const std::pair<sockaddr_in, std::vector<sockaddr_in>>& conn, const unsigned int freq)
{
	using namespace std::chrono;

        int listener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (listener < 0) {
		std::cerr << "Socket failed. The errno value is: " << errno << std::endl;
		throw std::runtime_error("socket");
	}

	int opt = 1;
	if (setsockopt (listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "Setsockopt failde. The errno value is: " << errno << std::endl;
		throw std::runtime_error("setsockopt");
	}

	if (bind(listener, (struct sockaddr *)&conn.first, sizeof(conn.first)) < 0) {
		std::cerr << "Bind failed. The errno value is: " << errno << std::endl;
		throw std::runtime_error("bind");
	}

	int num{0};
	unsigned int dgCounter{0};
	auto lastTimePoint{steady_clock::now()};
	int senderr{1};
	while (true) {
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
				if (dgCounter > freq) {
					continue;
				}
			}
			do {
				sockaddr_in clientAddr = conn.second[num];
				socklen_t sizeAddr = sizeof(clientAddr);
				senderr = sendto(listener, buf, err, 0, (struct sockaddr *)&clientAddr, sizeAddr);
				if (senderr < 0) {
					std::cerr << "Error sendto. The errno value is : " << errno << std::endl;
				}
				++num;
				num %= conn.second.size();
			}
			while (senderr < 0);
		}
		else {
			std::cerr << "Error resieved. The errno value is : " << errno << std::endl;
		}
	}
}
