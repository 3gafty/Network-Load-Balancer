#include "parser.h"

#include <arpa/inet.h>
#include <errno.h>
#include <fstream>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace My_NLB
{
	using namespace std;
	using json = nlohmann::json;
	json j_object;

	Parser::Parser(const string path)
    {
		ifstream j_file(path);
		if (!j_file.is_open())
        {
			throw runtime_error("Can't open config.json file(" + path + "), errno value is " + to_string(errno));
		}

		j_object = json::parse(j_file);
		j_file.close();

		nomps_ = j_object["num_of_msg_per_sec"].get<uint32_t>();

		sockaddr_in source;
		source.sin_family = AF_INET;
		source.sin_port = htons(j_object.at("source").at("port").get<uint16_t>());
		source.sin_addr.s_addr = inet_addr(j_object.at("source").at("ip").get<string>().c_str());

		vector<sockaddr_in> destinations;
		for (const auto& el : j_object.at("destinations"))
        {
			sockaddr_in dest;
			dest.sin_family = AF_INET;
			dest.sin_port = htons(el.at("port").get<uint16_t>());
			dest.sin_addr.s_addr = inet_addr(el.at("ip").get<string>().c_str());
			destinations.push_back(dest);
		}
		conns_ = make_pair(source, destinations);
	}

	pair<sockaddr_in, vector<sockaddr_in>>& Parser::getConns()
    {
		return conns_;
	}

	uint32_t Parser::getNomps() const
    {
		return nomps_;
	}

}	//!My_NLB
