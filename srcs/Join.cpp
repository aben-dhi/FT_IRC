/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 23:37:18 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/23 03:55:59 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

bool Client::isInChannel(const std::string& channelName) const
{
    std::map<std::string, Channel*>::const_iterator it = this->_isChannel.find(channelName);
    return it != this->_isChannel.end();
}

std::string	Server::_joinChannel( Request request, int i )
{
	int j = 1;
	if (!this->_clients[i]->getRegistered())
		return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));
	if (request._args.size() == 0)
		return (_printMessage("461", this->_clients[i]->getNickname(), " JOIN :Not enough parameters"));
	 std::string ChannelName = request._args[0];
	if (this->_clients[i]->isInChannel(ChannelName))
        return (_printMessage("443", this->_clients[i]->getNickname(), ChannelName + " :is already on channel"));
	
	if (request._args[0] == "0")
		return(this->_clients[i]->leaveAllC());
    Channel *channel = getChannelByName(ChannelName);
	if(channel)
	{
		if (channel->isInviteOnly() && !this->_clients[i]->isUserInvited(channel->getName()))
			return (_printMessage("473", this->_clients[i]->getNickname(), ChannelName + " :Cannot join channel (+i)"));
		if (channel->isModeKey() && request._args.size() == 1)
			return (_printMessage("475", this->_clients[i]->getNickname(), ChannelName + " :Cannot join channel (+k)"));
		else if (channel->isModeKey() && request._args.size() == 2 && request._args[1] != channel->getModeKey())
			return (_printMessage("475", this->_clients[i]->getNickname(), ChannelName + " :Cannot join channel (+k)"));
		if (channel->getUserLimit() != -1 && channel->getOnlineUsers() >= channel->getUserLimit())
			return (_printMessage("471", this->_clients[i]->getNickname(), channel->getName() + " :Cannot join channel (channel is full)"));
	}
	std::vector<std::string> parsChannels(_commaSeparator(request._args[0]));
	std::vector<std::string> parsKeys;
	if (request._args.size() == 2)
		parsKeys = _commaSeparator(request._args[1]);
	std::vector<std::string>::iterator itChannels = parsChannels.begin();
	std::vector<std::string>::iterator itKeys = parsKeys.begin();
	while (itChannels != parsChannels.end() && j == 1)
	{
		if ( itKeys != parsKeys.end())
		{
			std::cout<<"here"<<std::endl;
			j = _createPrvChannel(*itChannels, *itKeys, i);
		}
		else
		{
			std::cout << "or here" << std::endl;
			j = _createChannel(*itChannels, i);
		}
		if (j == BADCHANMASK)
			return (_printMessage("476", this->_clients[i]->getNickname(), *itChannels + " :Bad Channel Mask"));
		if (j == BANNEDFROMCHAN)
			return (_printMessage("474", this->_clients[i]->getNickname(), *itChannels + " :Cannot join channel (+b)"));
		if (j == TOOMANYCHANNELS )
			return (_printMessage("405", this->_clients[i]->getNickname(), *itChannels + " :You have joined too many channels"));
		if (j == BADCHANNELKEY )
			return (_printMessage("475", this->_clients[i]->getNickname(), *itChannels + " :Cannot join channel (+k)"));
		if (j == CHANNELISFULL )
			return (_printMessage("471", this->_clients[i]->getNickname(), *itChannels + " :Cannot join channel (+l)"));
		if (j == NOSUCHCHANNEL)
			return (_printMessage("403", this->_clients[i]->getNickname(), *itChannels + " :No such channel"));
		if (itKeys != parsKeys.end())
			itKeys++;
		itChannels++;
	};
	--itChannels;
	return ("");
};

int	Server::_createChannel( std::string ChannelName, int CreatorFd )
{
	std::map<std::string, Channel *>::iterator it = this->_channels.find(ChannelName);
	if (it == this->_channels.end())
	{
		if (ChannelName[0] != '&' && ChannelName[0] != '#')
			return (BADCHANMASK);
		Channel *channel = new Channel(ChannelName, this->_clients[CreatorFd]);
		this->_channels.insert(std::pair<std::string, Channel *>(ChannelName, channel));
		this->_clients[CreatorFd]->joinChannel(ChannelName, channel);
		_sendAll(CreatorFd, this->_clients[CreatorFd]->getUserprefix() + "JOIN " + ChannelName + "\n");
		_sendAll(CreatorFd, _printMessage("332", this->_clients[CreatorFd]->getNickname(), ChannelName + " :" + channel->getTopic()));
		_sendAll(CreatorFd, _printMessage("353", this->_clients[CreatorFd]->getNickname() + " = " + ChannelName, channel->listAllUsers()));
		_sendAll(CreatorFd, _printMessage("353", this->_clients[CreatorFd]->getNickname() + " " + ChannelName, ":End of NAMES list"));
		std::string reply = "JOIN " + ChannelName + "\n";
		_sendToEveryone(channel, reply, CreatorFd);
	}
	else
	{
		if (it->second->getKey().empty())
		{
			int i = 0;
			if (this->_clients[CreatorFd]->getIsop() == true)
				i = it->second->addOperator(this->_clients[CreatorFd]);
			else
				i = it->second->addMember(this->_clients[CreatorFd]);
			if (i == USERISJOINED)
				this->_clients[CreatorFd]->joinChannel( it->first, it->second );
			else if (i == USERALREADYJOINED)
				return (USERALREADYJOINED);
			else if (i == BANNEDFROMCHAN)
				return (BANNEDFROMCHAN);
			_sendAll(CreatorFd, this->_clients[CreatorFd]->getUserprefix() + "JOIN " + ChannelName + "\n");
			_sendAll(CreatorFd, _printMessage("332", this->_clients[CreatorFd]->getNickname(), ChannelName + " :" + it->second->getTopic()));
			_sendAll(CreatorFd, _printMessage("353", this->_clients[CreatorFd]->getNickname() + " = " + ChannelName, it->second->listAllUsers()));
			_sendAll(CreatorFd, _printMessage("353", this->_clients[CreatorFd]->getNickname() + " " + ChannelName, ":End of NAMES list"));
			std::string reply = "JOIN " + ChannelName + "\n";
			_sendToEveryone(it->second, reply, CreatorFd);
			return (USERISJOINED);
		}
	}
	return (USERISJOINED);
};

int	Server::_createPrvChannel( std::string ChannelName, std::string ChannelKey, int CreatorFd)
{
	std::map<std::string, Channel *>::iterator it = this->_channels.find(ChannelName);
	if (it == this->_channels.end())
	{
		if (ChannelName[0] != '&' && ChannelName[0] != '#' && ChannelName[0] != '+' && ChannelName[0] != '!')
			return (BADCHANMASK);
		Channel *channel = new Channel(ChannelName, ChannelKey, this->_clients[CreatorFd]);
		this->_channels.insert(std::pair<std::string, Channel *>(ChannelName, channel));
		this->_clients[CreatorFd]->joinChannel(ChannelName, channel);
	}
	else
	{
		if (it->second->getKey() == ChannelKey)
		{
			int i = 0;
			if (this->_clients[CreatorFd]->getIsop() == true)
				i = it->second->addOperator(this->_clients[CreatorFd]);
			else
				i = it->second->addMember(this->_clients[CreatorFd]);
			if (i == USERISJOINED)
				this->_clients[CreatorFd]->joinChannel( it->first, it->second );
			else if (i == USERALREADYJOINED)
				return (USERALREADYJOINED);
			else if (i == BANNEDFROMCHAN)
				return (BANNEDFROMCHAN);
			_sendAll(CreatorFd, this->_clients[CreatorFd]->getUserprefix() + "JOIN " + ChannelName + "\n");
			_sendAll(CreatorFd, _printMessage("332", this->_clients[CreatorFd]->getNickname(), ChannelName + " :" + it->second->getTopic()));
			_sendAll(CreatorFd, _printMessage("353", this->_clients[CreatorFd]->getNickname() + " = " + ChannelName, it->second->listAllUsers()));
			_sendAll(CreatorFd, _printMessage("353", this->_clients[CreatorFd]->getNickname() + ChannelName, ":End of NAMES list"));
			std::string reply = "JOIN " + ChannelName + "\n";
			_sendToEveryone(it->second, reply, CreatorFd);
			return (USERISJOINED);
		}
		else
			return (BADCHANNELKEY);
	}
	return (USERISJOINED);
};

std::vector<std::string> Server::_commaSeparator(std::string arg)
{
	std::vector<std::string> ret;
	size_t pos = 0;
	while ((pos = arg.find(",")) != std::string::npos)
	{
		ret.push_back(arg.substr(0, pos));
		arg.erase(0, pos + 1);
	}
	ret.push_back(arg.substr(0, pos));
	return (ret);
};