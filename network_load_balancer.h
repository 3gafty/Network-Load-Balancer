#ifndef NETWORK_LOAD_BALANCER_H
#define NETWORK_LOAD_BALANCER_H

#include <utility>
#include <vector>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

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
		std::pair<sockaddr_in, std::vector<sockaddr_in>> connections_;
		unsigned int numberOfMessagesPerSecond_;
		int listener_;
		bool run_;
	};
}

#endif	//	!NETWORK_LOAD_BALANCER_H