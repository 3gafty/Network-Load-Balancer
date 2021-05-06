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
	while (run) {
		char buf[1024];
		sockaddr_in clientAddr = conn.second[num++];
		num %= conn.second.size();
		socklen_t sizeAddr = sizeof(clientAddr);
                std::this_thread::sleep_for (std::chrono::milliseconds(1000 / freq));
		int err = recvfrom(listener, buf, 1024, 0, 0, 0);
		if (err > 0) {
			buf[err] = 0;
                        std::string output(buf);
			std::cout << "sendto port - " << ntohs(clientAddr.sin_port) << std::endl;
			sendto(listener, buf, err, 0, (struct sockaddr *)&clientAddr, sizeAddr);
			std::cout << output << std::endl;
			output.clear();
		}
		else {
			std::cout << "Error resieved" << std::endl;
			close(listener);
			run = false;
		}
	}
}