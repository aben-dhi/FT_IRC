/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ta9ra9 <ta9ra9@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 14:43:39 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/23 09:43:45 by ta9ra9           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::string	Server::_privmsg(Request request, int i)
{
	if (!this->_clients[i]->getRegistered())
		return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));
	if (request._args.size() < 2)
		return (_printMessage("461", this->_clients[i]->getNickname(), " PRIVMSG :Not enough parameters"));
	if (request._args.size() >= 2)
	{
		if (request._args[0].find(",") != std::string::npos)
			return (_printMessage("407", this->_clients[i]->getNickname(), " :Too many targets"));
		std::string msg;
		for (size_t j = 1; j < request._args.size(); j++)
		{
			msg.append(request._args[j]);
			if (j + 1 < request._args.size())
				msg.append(" ");
		}
		if (request._args[0][0] != '&' && request._args[0][0] != '#' && request._args[0][0] != '+' && request._args[0][0] != '!')
			return (_privToUser(request._args[0], msg, "PRIVMSG", i));
		return (_privToChannel(request._args[0], msg, i));
	}
	return ("");
};

std::string 	Server::_privToUser(std::string User, std::string message, std::string cmd, int i)
{
	int userFd = _findByNickname(User);
	if (userFd == USERNOTFOUND)
		return (_printMessage("401", this->_clients[i]->getNickname(), User.append(" :No such nick/channel")));
	std::string reply = this->_clients[i]->getUserprefix();
	reply.append(cmd + " " + User + " :" + message + "\n");
	if (_sendAll(userFd, reply) == -1)
				std::cout << "_sendall() error: " << strerror(errno) << std::endl;
	return ("");
};

std::string 	Server:: _privToChannel(std::string ChannelName, std::string message, int i)
{
	std::map<std::string, Channel *>::iterator it = this->_channels.find(ChannelName);
	if (it != this->_channels.end())
	{
		std::pair<Client *, int> user = it->second->findUserRole(i);
		if (user.second == -1 )
			return (_printMessage("404", this->_clients[i]->getNickname(), ChannelName.append(" :Cannot send to channel")));
		std::string msg("PRIVMSG " + ChannelName + " :" + message + "\n");
		_sendToEveryone(it->second, msg, i);
	}
	else
		return (_printMessage("401", this->_clients[i]->getNickname(), ChannelName.append(" :No such nick/channel")));
	return ("");
};