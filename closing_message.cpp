#include "closing_message.h"

#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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