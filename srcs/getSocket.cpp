/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getSocket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 23:29:50 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/09/05 20:38:52 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::_getSocket(std::string port)
{
	int yes = 1;
	int status;
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = getprotobyname("TCP")->p_proto;

	status = getaddrinfo("0.0.0.0", port.c_str(), &hints, &servinfo);
	if (status != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
		exit(-1);
	}
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		this->_socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (this->_socketfd == -1)
			throw std::runtime_error("Failed to Create Socket");
		setsockopt(this->_socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
			if(fcntl(this->_socketfd, F_SETFL, O_NONBLOCK) == -1)
				throw std::runtime_error("Failed to Set Socket fd to NONBLOCK");
		if (bind(this->_socketfd, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(this->_socketfd);
			continue;
		}
		break;
	}
	freeaddrinfo(servinfo);

	if (p == NULL)
	{
		std::cerr << "bind: " << strerror(errno) << std::endl;
		exit(-1);
	}
	if (listen(this->_socketfd, this->_max_c) == -1)
	{
		std::cerr << "listen: " << strerror(errno) << std::endl;
		exit(-1);
	}
}