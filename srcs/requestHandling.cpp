/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestHandling.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:19:03 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/09/05 20:44:00 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::_ClientRequest(int i)
{
	char buffer[6000];
	int sender_fd = this->_pfds[i].fd;
	int nbytes = recv(sender_fd, buffer, sizeof(buffer) - 1, 0);
	if (nbytes < 0)
	{
		if(errno == EWOULDBLOCK  || errno == EAGAIN)
			return;
		else
		{
			std::cerr << "recv() error:" << strerror(errno) << std::endl;
			close(sender_fd);
			_removeFromPoll(i);
		}
	}
	else if (nbytes == 0)
	{
		std::cout << "[" << currentDateTime() << "]:" << "socket " << sender_fd << " hung up" << std::endl;
		close(sender_fd);
		_removeFromPoll(i);
	}
	else
	{
		buffer[nbytes] = '\0';
		std::string message(buffer, nbytes);
		std::string ret = _parsing(message, this->_pfds[i].fd);
		if (send(sender_fd, ret.c_str(), ret.length(), 0) == -1)
			std::cerr << "send() error:" << strerror(errno) << std::endl;
	}
	memset(buffer, 0, 6000);
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
	if(!request._args[0].empty() && request._args[0].back() == '\n')
		request._args[0].pop_back();


	// // DEBUG
	// std::cout<<request._command<<std::endl;
	// for (size_t i = 0; i < request._args.size(); i++)
	// 	std::cout<<request._args[i]<<std::endl;
	
	
	return (request);
}