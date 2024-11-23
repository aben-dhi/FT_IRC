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

// std::string	Server::_sendToEveryone(Channel *Channel, std::string message, int fd)
// {
// 	std::map<int, Client*> allUsers = Channel->getAllUsers();
// 	std::map<int, Client*>::iterator it = allUsers.begin();
// 	std::string msg = this->_clients[fd]->getUserprefix();
// 	msg.append(message);
// 	while (it != allUsers.end())
// 	{
// 		if (it->first != fd)
// 			if(_sendAll(it->first, msg) == -1)
// 			{
// 				std::cerr << "send: " << strerror(errno) << std::endl;
// 				return("");
// 			}
// 		it++;
// 	}
// 	std::cout<<msg<<std::endl;
// 	return(msg);
// }

std::string Server::_sendToEveryone(Channel *channel, std::string message, int fd)
{
    std::map<int, Client*> allUsers = channel->getAllUsers();
    std::map<int, Client*>::iterator it = allUsers.begin();

    // Construct the full message with the prefix for the sender
    std::string prefix = this->_clients[fd]->getUserprefix();
    std::string fullMessage = prefix + " " + message; // Construct the full message

    while (it != allUsers.end())
    {
        if (it->first != fd) // Don't send the message back to the sender
        {
            // Send the formatted message to each user in the channel
            if (_sendAll(it->first, fullMessage + "\n") == -1) // Ensure new line at the end
            {
                std::cerr << "send: " << strerror(errno) << std::endl;
                return ""; // Return empty on failure
            }
        }
        it++;
    }

    std::cout << fullMessage << std::endl; // Log the sent message
    return fullMessage; // Return the full message sent
}

std::string Server::_sendToEveryone2(Channel *channel, std::string message, int fd)
{
    std::map<int, Client*> allUsers = channel->getAllUsers();
    std::map<int, Client*>::iterator it = allUsers.begin();

    // Construct the full message with the prefix for the sender
    std::string prefix = this->_clients[fd]->getUserprefix();
    std::string fullMessage = prefix + " " + message; // Construct the full message

    while (it != allUsers.end())
    {
        // Send the formatted message to each user in the channel
        if (_sendAll(it->first, fullMessage + "\n") == -1) // Ensure new line at the end
        {
            std::cerr << "send: " << strerror(errno) << std::endl;
            return ""; // Return empty on failure
        }
        it++;
    }

    std::cout << fullMessage << std::endl; // Log the sent message
    return fullMessage; // Return the full message sent
}