/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ta9ra9 <ta9ra9@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 23:41:13 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/23 12:09:16 by ta9ra9           ###   ########.fr       */
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

std::string	Server::_sendToEveryone(Channel *Channel, std::string message, int fd)
{
	std::map<int, Client*> allUsers = Channel->getAllUsers();
	std::map<int, Client*>::iterator it = allUsers.begin();
	std::string msg = this->_clients[fd]->getUserprefix();
	msg.append(message);
	while (it != allUsers.end())
	{
		if (it->first != fd)
			if(_sendAll(it->first, msg) == -1)
			{
				std::cerr << "send: " << strerror(errno) << std::endl;
				return("");
			}
		it++;
	}
	std::cout<<msg<<std::endl;
	return(msg);
}

std::string Server::_sendToEveryone2(Channel *channel, std::string message, int fd)
{
    std::map<int, Client*> allUsers = channel->getAllUsers();
    std::map<int, Client*>::iterator it = allUsers.begin();
    std::string msg = this->_clients[fd]->getUserprefix() + message;
    while (it != allUsers.end())
    {
        
            if (_sendAll(it->first, msg) == -1)
            {
                std::cerr << "send: " << strerror(errno) << std::endl;
                return "";
            }
        // }
        it++;
    }
    std::cout << msg << std::endl;
    return msg;
}