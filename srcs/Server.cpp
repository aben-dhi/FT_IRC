/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 00:45:58 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/09/05 20:49:00 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server() : _name(""), _password(""), _socketfd(0), _clients(), _pfds(NULL), _online_c(0), _max_c(0), _prefx(":"), _channels(), _wrongUser(), _nicknames(), _files()
{
}
Server::Server(std::string name, std::string password, int max_c, std::string port) : _clients()
{
	this->_name = name;
	this->_password = password;
	this->_online_c = 0;
	this->_max_c = max_c + 1;
	this->_pfds = new struct pollfd[this->_max_c];
	_getSocket(port);
	this->_pfds[0].fd = this->_socketfd;
	this->_pfds[0].events = POLLIN;
	this->_online_c++;
}

Server::~Server()
{
	if (this->_pfds)
		delete[] this->_pfds;
	std::map<int, Client*>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		delete it->second;
		it++;
	}
	this->_clients.clear();
	std::map<std::string, Channel*>::iterator it2 = this->_channels.begin();
	while (it2 != this->_channels.end())
	{
		delete it2->second;
		it2++;
	}
	this->_channels.clear();
}

std::string Server::_printMessage(std::string message, std::string nickname, std::string channel)
{
	if (nickname.empty())
		nickname = "*";
	return (":" + this->_name + " " + message + " " + nickname + " " + channel + "\n");
}

void	Server::_newClient()
{
	struct sockaddr_in	client;
	socklen_t		client_len = sizeof(client);
	int			newfd = accept(this->_socketfd, (struct sockaddr *)&client, &client_len);
	if (newfd == -1)
	{
		std::cerr << "accept: " << strerror(errno) << std::endl;
		return ;
	}
	else
	{
		_addToPoll(newfd);
		std::string welcome = _welcomeMsg();
		if (send(newfd, welcome.c_str(), welcome.size(), 0) == -1)
			std::cerr << "send: " << strerror(errno) << std::endl;
		std::cout << "New client connected" << std::endl;
		std::cout << "[" << currentDateTime() << "] new connection " 
		<< inet_ntoa(((struct sockaddr_in*) &client)->sin_addr)
		<< " on socket " << newfd << std::endl;
	}
}

void	Server::_run()
{
	while (77)
	{
		int poll_ct = poll(this->_pfds, this->_online_c, -1);
		if (poll_ct == -1)
		{
			std::cerr << "poll: " << strerror(errno) << std::endl;
			exit (-1); ;
		}
		
		for (int i = 0; i < this->_online_c; i++)
		{
			if (this->_pfds[i].revents & POLLIN)
			{
				if (i == 0)
					_newClient();
				else
					_ClientRequest(i);
			}
		}
	}
}

std::string Server::_getPassword() const
{
	return (this->_password);
}