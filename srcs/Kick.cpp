/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 23:39:29 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/22 18:10:49 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::string		Server::_kickedFromChannel(std::string ChannelName, std::string message, std::vector<std::string> users, int i)
{
	std::map<std::string, Channel *>::iterator it = this->_channels.find(ChannelName);
	if (it != this->_channels.end())
	{
		std::pair<Client *, int> user = it->second->findUserRole(i);
		if (user.second == 1)
		{
			std::vector<std::string>::iterator user = users.begin();
			int ret = 0;
			while (user != users.end())
			{
				ret = _findByNickname(*user);
				if (ret == USERNOTINCHANNEL)
					return (_printMessage("441", this->_clients[i]->getNickname(), (*user).append(" " + ChannelName + " :They aren't on that channel")));
				std::string reply = "KICK " + ChannelName;
				if (message.empty())
					reply.append("\n");
				else
					reply.append(" " + message + "\n");
				_sendToEveryone(it->second, reply, i);
				it->second->banUser(this->_clients[ret]);
				ret = _partChannel(ChannelName, ret, "", 0);
				user++;
			}
		}
		else if (user.second == -1  /* Not in channel */)
			return (_printMessage("442", this->_clients[i]->getNickname(), ChannelName + " :You're not on that channel"));
		else
			return (_printMessage("482", this->_clients[i]->getNickname(), ChannelName + " :You're not channel operator"));
		return ("");
	}
	return (_printMessage("403", this->_clients[i]->getNickname(), ChannelName.append(" :No such channel")));
};

std::string	Server::_kick(Request request, int i)
{
	if (!this->_clients[i]->getRegistered())
		return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));
	if (request._args.size() < 2)
		return (_printMessage("461", this->_clients[i]->getNickname(), " KICK :Not enough parameters"));
	std::vector<std::string> channels(_commaSeparator(request._args[0]));
	std::vector<std::string> users(_commaSeparator(request._args[1]));
	std::vector<std::string>::iterator it = channels.begin();
	while (it != channels.end())
	{
		std::string ret;
		if (request._args.size() == 3)
			ret = _kickedFromChannel(*it, request._args[2], users, i);
		else
			ret = _kickedFromChannel(*it, "", users, i);
		if (!ret.empty())
			return(ret);
		it++;
	}
	return ("");
};