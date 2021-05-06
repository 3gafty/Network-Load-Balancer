#include "networkLoadBalancer.h"

void networkLoadBalancer(const std::pair<sockaddr_in, std::vector<sockaddr_in>>& conn, const int freq)
{
        int listener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (listener < 0) {
		perror("socket");
		exit(1);
	}

	int opt = 1;
	if (setsockopt (listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt");
		exit(3);
	}

	if (bind(listener, (struct sockaddr *)&conn.first, sizeof(conn.first)) < 0) {
		perror("bind");
		exit(4);
	}

        bool run{true};
	int num{0};
	unsigned int dgCounter{0};
	while (run) {
		char buf[SIZEBUFF];
		sockaddr_in clientAddr = conn.second[num++];
		num %= conn.second.size();
		socklen_t sizeAddr = sizeof(clientAddr);
		int err = recvfrom(listener, buf, SIZEBUFF, 0, 0, 0);
		auto timePoint = std::chrono::steady_clock::now();
		if (err > 0 && err < SIZEBUFF) {
			++dgCounter;
			buf[err] = 0;
                        std::string output(buf);
			std::cout << "sendto port - " << ntohs(clientAddr.sin_port) << std::endl;
			sendto(listener, buf, err, 0, (struct sockaddr *)&clientAddr, sizeAddr);
			std::cout << output << std::endl;
			output.clear();
		}
		else {
			std::cerr << "Error resieved" << std::endl;
		}
	}
}
