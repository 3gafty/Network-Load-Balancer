#include "networkLoadBalancer.h"

#include <iostream>
#include <chrono>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>

void networkLoadBalancer(const std::pair<sockaddr_in, std::vector<sockaddr_in>>& conn, const unsigned int freq)
{
	using namespace std::chrono;

        int listener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (listener < 0) {
		throw std::runtime_error("socket");
	}

	int opt = 1;
	if (setsockopt (listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		throw std::runtime_error("setsockopt");
	}

	if (bind(listener, (struct sockaddr *)&conn.first, sizeof(conn.first)) < 0) {
		throw std::runtime_error("bind");
	}

	bool timeFlag{false};
	int num{0};
	unsigned int dgCounter{0};
	uint64_t elapsedTimer{0};
	auto lastTimePoint = steady_clock::now();
	while (true) {
		char buf[SIZEBUFF];
		sockaddr_in clientAddr = conn.second[num];
		socklen_t sizeAddr = sizeof(clientAddr);
		int err = recvfrom(listener, buf, SIZEBUFF, MSG_CTRUNC, 0, 0);
		auto timePointCurrent = steady_clock::now();
		if (err > 0 && err < SIZEBUFF) {
			++dgCounter;
			if (timeFlag) {
				auto timeDur = timePointCurrent - lastTimePoint;
				elapsedTimer += duration_cast<milliseconds>(timeDur).count();
			}
			if (dgCounter <= freq && elapsedTimer <= ONE_SECOND) {
				//std::cout << "sendto port - " << ntohs(clientAddr.sin_port) << std::endl;
				int senderr = sendto(listener, buf, err, 0, (struct sockaddr *)&clientAddr, sizeAddr);
				if (senderr > SIZEBUFF)
					std::cerr << "PacketSize more then SIZEBUFF. Not implemented case" << std::endl;
				else
					std::cerr << "Error sendto" << std::endl;
				++num %= conn.second.size();
			}
			else if (elapsedTimer > ONE_SECOND) {
				dgCounter = 0;
				elapsedTimer = 0;
			}
			//std::cout << std::endl;
		}
		else {
			std::cerr << "Error resieved" << std::endl;
		}
		timeFlag = true;
		lastTimePoint = timePointCurrent;
	}
}
