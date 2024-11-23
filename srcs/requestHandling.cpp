/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestHandling.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ta9ra9 <ta9ra9@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:19:03 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/23 10:42:19 by ta9ra9           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::vector<std::string> Server::splitBuffer(std::string buffer)
{
	std::vector<std::string> requests;
	std::istringstream sstr(buffer);
	std::string line;
	while(std::getline(sstr, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		requests.push_back(line);
	}
	return requests;
}

void	Server::_ClientRequest(int i)
{
	std::vector<std::string> requests;
	char buffer[6000];
	memset(buffer, 0, 6000);
	int sender_fd = this->_pfds[i].fd;
	int nbytes = recv(sender_fd, buffer, sizeof(buffer) - 1, 0);
	if (nbytes == 0)
	{
		std::cerr << "[" << currentDateTime() << "]:" << "socket " << sender_fd << " hung up" << std::endl;
		_removeFromPoll(sender_fd);
		return;
	}
	else
	{
		this->_clients[sender_fd]->set_buffer(buffer);
		if(this->_clients[sender_fd]->get_buffer().find_first_of("\r\n") == std::string::npos)
			return ;
		requests = splitBuffer(this->_clients[sender_fd]->get_buffer());
		for(size_t i = 0; i < requests.size(); i++)
		{
			std::cout<<"["<<currentDateTime()<<"]:"<<requests[i]<<std::endl;
			std::string ret = _parsing(requests[i], sender_fd);
			if(ret == "QUIT")
				return ;
			std::cout<<"["<<currentDateTime()<<"]:"<<ret<<std::endl;
			if (send(sender_fd, ret.c_str(), ret.length(), 0) == -1)
				std::cerr << "send() error:" << strerror(errno) << std::endl;
		}
	}
	memset(buffer, 0, 6000);
	if(this->_clients[sender_fd])
		this->_clients[sender_fd]->clear_buffer();
}

Request Server::_splitRequest(std::string req)
{
	Request request;
	size_t i = 0;
	size_t j = 0;

	if (req[i] == ' ' || !req[i])
	{
		request._isValid = true;
		return (request);
	}
	j = i;
	while (req[i])
	{
		if (req[i] == ' ')
		{
			if (req[i + 1] == ' ')
			{
				request._isValid = true;
				return (request);
			}
			request._args.push_back(req.substr(j, i - j));
			while (req[i] == ' ')
				i++;
			j = i;
		}
		if (req[i] == ':')
		{
			if (req[i + 1] == ' ')
			{
				request._isValid = true;
				return (request);
			}
			request._args.push_back(req.substr(i + 1, req.length() - i));
			request._command = request._args[0];
			request._args.erase(request._args.begin());
			return (request);
		}
		i++;
	}
	if (i && req[j])
		request._args.push_back(req.substr(j, i - j));
	request._command = request._args[0];
	request._args.erase(request._args.begin());
	if(request._args.size() != 0)
	{
		if(!request._args[0].empty() && request._args[0][request._args[0].size() - 1] == '\n')
			request._args[0].erase(request._args[0].size() - 1);;
	}
	
	return (request);
}