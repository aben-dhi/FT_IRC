/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollHandling.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 23:35:06 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/09/05 20:46:22 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::_addToPoll(int fd)
{
	if (this->_online_c == this->_max_c)
	{
		this->_max_c *= 2;
		this->_pfds = (struct pollfd *)realloc(this->_pfds, this->_max_c);
	}
	this->_pfds[this->_online_c].fd = fd;
	this->_pfds[this->_online_c].events = POLLIN;
	this->_clients.insert(std::pair<int, Client*>(fd, new Client(fd)));
	this->_online_c++;
}

void	Server::_removeFromPoll(int fd)
{
	close(this->_pfds[fd].fd);
	this->_pfds[fd] = this->_pfds[this->_online_c - 1];
	this->_clients.erase(this->_pfds[fd].fd);
	this->_online_c--;
}