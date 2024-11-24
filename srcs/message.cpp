/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 23:41:13 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/23 22:48:52 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::string Server::_welcomeMsg()
{
	std::string welcome;
	welcome.append("██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗\n");
	welcome.append("██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝\n");
	welcome.append("██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗\n");
	welcome.append("██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝\n");
	welcome.append("╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗\n");
	welcome.append(" ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝\n");
	welcome.append("You need to login so you can start chatting OR you can send HELP to see how :) \n");
	return (welcome);
}

int	Server::_sendAll(int fd, std::string message)
{
	int total = 0;
	int bytesleft = message.length();
	int n;

	while (total < (int)message.length())
	{
		n = send(fd, message.c_str() + total, bytesleft, 0);
		if (n == -1)
			break;
		total += n;
		bytesleft -= n;
	}
	if (n == -1)
	{
		std::cerr << "send: " << strerror(errno) << std::endl;
		return (-1);
	}
	return (0);
}

std::string Server::_sendToEveryone(Channel *channel, std::string message, int fd)
{
    std::map<int, Client*> allUsers = channel->getAllUsers();
    std::map<int, Client*>::iterator it = allUsers.begin();

    std::string prefix = this->_clients[fd]->getUserprefix();
    std::string fullMessage = prefix + " " + message;

    while (it != allUsers.end())
    {
        if (it->first != fd)
        {
            if (_sendAll(it->first, fullMessage + "\n") == -1)
            {
                std::cerr << "send: " << strerror(errno) << std::endl;
                return "";
            }
        }
        it++;
    }
    return fullMessage;
}

std::string Server::_sendToEveryone2(Channel *channel, std::string message, int fd)
{
    std::map<int, Client*> allUsers = channel->getAllUsers();
    std::map<int, Client*>::iterator it = allUsers.begin();

    std::string prefix = this->_clients[fd]->getUserprefix();
    std::string fullMessage = prefix + " " + message;

    while (it != allUsers.end())
    {
        if (_sendAll(it->first, fullMessage + "\n") == -1)
        {
            std::cerr << "send: " << strerror(errno) << std::endl;
            return "";
        }
        it++;
    }
    return fullMessage;
}