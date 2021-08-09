#include "sender_recipient.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void sender(uint16_t port) {
	sockaddr_in sender;
	sender.sin_family = AF_INET;
	sender.sin_port = htons(port);
	sender.sin_addr.s_addr = inet_addr("127.0.0.1");

}