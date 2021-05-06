#ifndef PARSADDRESS_H
#define PARSADDRESS_H

#include <vector>
#include <utility>
#include <string>

#include <arpa/inet.h>
#include <fcntl.h>

std::pair<sockaddr_in, std::vector<sockaddr_in>> parsAddress(const std::string& str);

#endif // !PARSADDRESS_H
