#ifndef PARS_ADDRESS_H
#define PARS_ADDRESS_H

#include <vector>
#include <utility>
#include <string>

#include <arpa/inet.h>
#include <fcntl.h>

std::pair<sockaddr_in, std::vector<sockaddr_in>> parsAddress(const std::string& str);

#endif //	!PARS_ADDRESS_H
