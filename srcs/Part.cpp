/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 14:38:16 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/08/17 14:42:55 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

int	Server::_partChannel( std::string ChannelName, int i, std::string message, int isPart )
{
	std::map<std::string, Channel *>::iterator itCh= this->_channels.find(ChannelName);
	if (itCh == this->_channels.end() /* No such channel */)
		return (NOSUCHCHANNEL);
	else
	{
		std::pair<Client *, int> user(itCh->second->findUserRole(i));
		if (user.second == -1 /* Not in channel */)
			return (NOTINCHANNEL);
		else
		{
			if (user.second == 0)
				itCh->second->removeMember(i);
			else if (user.second == 1)
				itCh->second->removeOperator(i);
			else
				itCh->second->removeVoice(i);
			user.first->removeChannel(itCh->second->getName());
			if (isPart == 1)
			{
				std::string reply = "PART " + ChannelName;
				if (message.empty())
					reply.append("\n");
				else
					reply.append(" " + message + "\n");
				_sendToEveryone(itCh->second, reply, i);
			}
		}
	}
	return (0);
}

std::string	Server::_part( Request request, int i )
{
	if (!this->_clients[i]->getRegistered())
		return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));
	if (request._args.size() == 0)
		return (_printMessage("461", this->_clients[i]->getNickname(), ":Not enough parameters"));
	std::vector<std::string>	parsChannels(_commaSeparator(request._args[0]));
	std::vector<std::string>::iterator it = parsChannels.begin();
	while (it != parsChannels.end())
	{
		int j = 0;
		if (request._args.size() == 2)
			j = _partChannel(*it, i, request._args[1], 1);
		else
			j = _partChannel(*it, i, "", 1);
		if (j == NOSUCHCHANNEL /* No such channel */)
			return (_printMessage("403", this->_clients[i]->getNickname(), *it + " :No such channel"));
		if (j == NOTINCHANNEL /* Not in channel */)
			return (_printMessage("442", this->_clients[i]->getNickname(), *it + " :You're not on that channel"));
		it++;
	}
	return ("");
};