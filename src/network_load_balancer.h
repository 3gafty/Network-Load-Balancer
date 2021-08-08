#ifndef NETWORK_LOAD_BALANCER_H
#define NETWORK_LOAD_BALANCER_H

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <chrono>
#include <list>

#include <utility>
#include <vector>

namespace mynamespace {

	class NetworkLoadBalancer {
	public:
		NetworkLoadBalancer();
		void setConnections(std::pair<sockaddr_in, std::vector<sockaddr_in>>&& t_conn);
		void setNumberOfMessagesPerSecond(unsigned int t_freq);
		void run();
		void stop();
		~NetworkLoadBalancer();

	private:
		void closingMessage();
		std::pair<sockaddr_in, std::vector<sockaddr_in>> connections_;
		unsigned int number_of_messages_per_second_;
		int listener_;
		bool run_;
		std::list<std::chrono::_V2::steady_clock::time_point> time_stamps_of_sended_messages_;
	};
}

#endif	// NETWORK_LOAD_BALANCER_H