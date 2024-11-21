/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:45:34 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/20 06:06:33 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::string	Server::_parsing(std::string message, int i)
{
	Request request(_splitRequest(message));
	
	if (request._isValid)
		return ("Invalid request\n");
	if(request._command == "CAP")
		return ("");
	if (request._command == "PASS" || request._command == "pass")
		return (_setPassWord(request, i));
	else if (request._command == "NICK" || request._command == "nick")
		return (_setNickName(request, i));
	else if (request._command == "USER" || request._command == "user")
		return (_setUserName(request, i));
	else if (request._command == "OPER" || request._command == "oper")
		return (_setOper(request, i));
	else if (request._command == "MODE" || request._command == "mode")
		return (_setMode(request, i));
	else if (request._command == "PRIVMSG" || request._command == "privmsg")
		return (_privmsg(request, i));
	else if (request._command == "NOTICE" || request._command == "notice")
		return (_notice(request, i));
	else if (request._command == "HELP" || request._command == "help")
		return (_printHelpInfo());
	else if (request._command == "JOIN" || request._command == "join")
		return (_joinChannel(request, i));
	else if (request._command == "TOPIC" || request._command == "topic")
		return (_topic(request, i));
	else if (request._command == "KICK" || request._command == "kick")
		return (_kick(request, i));
	else if (request._command == "PART" || request._command == "part")
		return (_part(request, i));
	else if (request._command == "QUIT" || request._command == "quit")
	{
		_quit(request, i);
		return ("QUIT");
	}
	else if (request._command == "SENDFILE" || request._command == "sendfile")
		return (_sendFile(request, i));
	else if (request._command == "GETFILE" || request._command == "getfile")
		return (_getFile(request, i));
	else if (request._command == "ROBOSERV" || request._command == "roboserv")
		return (_RoboServ( request, i));
	else if (request._command == "INVITE" || request._command == "invite")
		return (inviteRequest(request, i));
	else
		return ("Invalid command\n");
}

std::string Server::_notice(Request request, int i)
{
	if (!this->_clients[i]->getRegistered())
		return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));
	if (request._args.size() < 2)
		return (_printMessage("461", this->_clients[i]->getNickname(), ":Not enough parameters"));
	if (request._args.size() == 2)
		_privToUser(request._args[0], request._args[1], "NOTICE", i);
	return ("");
}

int Server::_findByNickname(std::string nickname)
{
	std::map<int, Client*>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		if (it->second->getNickname() == nickname)
			return (it->second->getClientfd());
		it++;
	}
	return (USERNOTINCHANNEL);
}

std::string Server::_topic(Request request, int i)
{
	if (!this->_clients[i]->getRegistered())
		return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));
	if (request._args.size() == 0)
		return (_printMessage("461", this->_clients[i]->getNickname(), ":Not enough parameters"));
	if (request._args.size() == 1)
	{
		if (this->_channels.find(request._args[0])->second->getTopic().empty())
			return (_printMessage("331", this->_clients[i]->getNickname(), request._args[0] + " :No topic is set"));
		else
			return (_printMessage("332", this->_clients[i]->getNickname(), request._args[0] + " :" + this->_channels.find(request._args[0])->second->getTopic()));
	}
	std::map<std::string, Channel *>::iterator it = this->_channels.find(request._args[0]);
	if (it != this->_channels.end())
	{
		std::pair<Client *, int> user = it->second->findUserRole(i);
		if (user.second == 1)
		{
			it->second->setTopic(request._args[1]);
			std::string reply = "TOPIC " + it->second->getName() + ":" + request._args[1] + "\n";
			_sendToEveryone(it->second, reply, i);
		}
		else if (user.second == -1  /* Not in channel */)
			return (_printMessage("442", this->_clients[i]->getNickname(), request._args[0] + " :You're not on that channel"));
		else
			return (_printMessage("482", this->_clients[i]->getNickname(), request._args[0] + " :You're not channel operator"));
	}
	return ("");
}

bool	Server::_validMode(Request request)
{
	// we re rewriting this!!
	char c = request._args[1][1];
	if (request._args[1].length() != 2 || (request._args[1][0] != '-' && request._args[1][0] != '+') )
		return (false);
	if (c != 'a' && c != 'i' && c != 'w' && c != 'r' && c != 'o' && c != 'O' && c != 's')
		return (false);
	return (true);
}

// std::string Server::_printUserModes(std::string ret, int i)
// {
// 	ret.append("a: " + std::string(to_cstr(this->_clients[i]->getModes('a'))));
// 	ret.append("\ni: " + std::string(to_cstr(this->_clients[i]->getModes('i'))));
// 	ret.append("\nw: " + std::string(to_cstr(this->_clients[i]->getModes('w'))));
// 	ret.append("\nr: " + std::string(to_cstr(this->_clients[i]->getModes('r'))));
// 	ret.append("\no: " + std::string(to_cstr(this->_clients[i]->getModes('o'))));
// 	ret.append("\nO: " + std::string(to_cstr(this->_clients[i]->getModes('O'))));
// 	ret.append("\ns: " + std::string(to_cstr(this->_clients[i]->getModes('s')) + "\n"));
// 	return ret;
// }

std::string Server::_setMode(Request request, int i)
{
    if (!this->_clients[i]->getRegistered())
        return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));

    if (request._args.size() < 2)
    {
        std::string ret;
        ret.append("461 " + this->_clients[i]->getNickname() + " :Not enough parameters\n");
        return ret;
    }

    std::string channelName = request._args[0];
    std::string modeChanges = request._args[1];
	std::cout<<modeChanges<<std::endl;
    Channel *channel = getChannelByName(channelName);

    if (!channel)
        return (_printMessage("403", this->_clients[i]->getNickname(), channelName + " :No such channel"));
	
	std::pair<Client *, int> user = channel->findUserRole(i);
    if (user.second != 1)
        return (_printMessage("482", this->_clients[i]->getNickname(), channelName + " :You're not channel operator"));
    Client *targetClient = getClientByNickname(request._args[2]);

    bool addMode = true;
    for (size_t j = 0; j < modeChanges.size(); ++j)
    {
        switch (modeChanges[j])
        {
            case '+':
                addMode = true;
                break;
            case '-':
                addMode = false;
                break;
            case 'i':
                channel->setInviteOnly(addMode);
                break;
            case 't':
                channel->setTopicRestricted(addMode);
                break;
            case 'k':
                if (addMode)
                {
                    if (request._args.size() < 3)
                        return (_printMessage("461", this->_clients[i]->getNickname(), ":Key not provided"));
                    channel->setModeKey(request._args[2]);
                }
                else
                {
                    channel->setModeKey("");
                }
                break;
            case 'o':
                if (request._args.size() < 3)
                    return (_printMessage("461", this->_clients[i]->getNickname(), ":Nickname not provided"));
                if (!targetClient)
                    return (_printMessage("401", this->_clients[i]->getNickname(), request._args[2] + " :No such nick"));
                if (addMode)
                    channel->addOperator(targetClient);
                else
                    channel->removeOperator(i);
                break;
            case 'l':
                if (addMode)
                {
                    if (request._args.size() < 3)
                        return (_printMessage("461", this->_clients[i]->getNickname(), ":Limit not provided"));
                    int limit = std::stoi(request._args[2]);
                    channel->setUserLimit(limit);
                }
                else
                {
                    channel->setUserLimit(-1);
                }
                break;
            default:
                return (_printMessage("472", this->_clients[i]->getNickname(), std::string(1, modeChanges[j]) + " :is unknown mode char to me"));
        }
    }

    return (_printMessage("221", this->_clients[i]->getNickname(), modeChanges));
}

// std::string Server::_setMode(Request request, int i)
// {
// 	if (!this->_clients[i]->getRegistered())
// 		return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));
// 	if (request._args.size() < 2)
// 	{
// 		std::string ret;
// 		// if (request._args.size() == 1 && request._args[0] == this->_clients[i]->getNickname())
// 		// 	return (_printUserModes(ret, i));
// 		ret.append("461 " + this->_clients[i]->getNickname() + " :Not enough parameters\n");
// 		return ret;
// 	}
// 	if (request._args[0] != this->_clients[i]->getNickname())
// 		return (_printMessage("502", this->_clients[i]->getNickname(), ":Can't change mode for other users"));
// 	if (!_validMode(request))
// 		return (_printMessage("501", this->_clients[i]->getNickname(), ":Invalid mode"));
// 	// if (request._args[1][0] == '+')
// 	// 	this->_clients[i]->setModes(true, request._args[1][1]);
// 	// else
// 	// 	this->_clients[i]->setModes(false, request._args[1][1]);
// 	return (_printMessage("221", this->_clients[i]->getNickname(), request._args[1]));
// }

std::string	Server::_setOper(Request request, int i)
{
	if (!this->_clients[i]->getRegistered())
		return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));
	if (request._args.size() < 2)
		return (_printMessage("461", this->_clients[i]->getNickname(), "PASS :Not enough parameters"));
	if (request._args[0] != "ADMIN")
		return (_printMessage("464", this->_clients[i]->getNickname(), ":Username/Password incorrect"));
	if (request._args[1] != "ROBOSERV")
		return (_printMessage("464", this->_clients[i]->getNickname(), ":Username/Password incorrect"));
	this->_clients[i]->setIsop(true);
	return (_printMessage("381", this->_clients[i]->getNickname(), ":You are now an IRC operator"));
}

std::string	Server::_setPassWord(Request request, int i)
{
	if (request._args.size() < 1)
		return (_printMessage("461", this->_clients[i]->getNickname(), "PASS :Not enough parameters"));
	if (this->_clients[i]->getRegistered())
		return (_printMessage("462", this->_clients[i]->getNickname(), ":Unauthorized command (already registered)"));
	if (request._args[0] != this->_password)
		return (_printMessage("997", this->_clients[i]->getNickname(), ":Incorrect password"));
	else
		this->_clients[i]->setAuth(true);
	return ("");
};

std::string	Server::_setNickName(Request request, int i)
{
	if (!this->_clients[i]->getAuth())
		return (_printMessage("998", this->_clients[i]->getNickname(), ":You need to authenticate first"));
	if (request._args.size() < 1)
		return (_printMessage("431", this->_clients[i]->getNickname(), ":No nickname given"));
	int	j = 0;
	while (request._args[0][j])
	{
		if (!isalnum(request._args[0][j]) && request._args[0][j] != '-' && request._args[0][j] != '\r')
			return (_printMessage("432", this->_clients[i]->getNickname(), request._args[0] + " :Erroneous nickname"));
		j++;
	}
	if (std::find(this->_nicknames.begin(), this->_nicknames.end(), request._args[0]) != this->_nicknames.end())
		return (_printMessage("433", this->_clients[i]->getNickname(), request._args[0] + " :Nickname is already in use"));

	this->_clients[i]->setNickname(request._args[0]);
	this->_nicknames.push_back(this->_clients[i]->getNickname());
	if (this->_clients[i]->getUsername() != "") {
		this->_clients[i]->setID(this->_clients[i]->getNickname() + "!" + this->_clients[i]->getUsername() + "@" + this->_clients[i]->getHostname());
		this->_clients[i]->setRegistered(true);
		return (_printMessage("001", this->_clients[i]->getNickname(), "Welcome to the Internet Relay Network " + this->_clients[i]->getID()));
	}
	return ("");
}

std::string	Server::_setUserName(Request request, int i)
{
	if (!this->_clients[i]->getAuth())
		return (_printMessage("998", this->_clients[i]->getNickname(), ":You need to authenticate first"));
	if (this->_clients[i]->getRegistered())
		return (_printMessage("462", this->_clients[i]->getNickname(), ":Unauthorized command (already registered)"));
	if (request._args.size() < 4)
		return (_printMessage("461", this->_clients[i]->getNickname(), "USER :Not enough parameters"));

	this->_clients[i]->setUsername(request._args[0]);
	this->_clients[i]->setRealname(request._args[3]);
	if (this->_clients[i]->getNickname() != "") {
		this->_clients[i]->setID(this->_clients[i]->getNickname() + "!" + this->_clients[i]->getUsername() + "@" + this->_clients[i]->getHostname());
		this->_clients[i]->setRegistered(true);
		return (_printMessage("001", this->_clients[i]->getNickname(), "Welcome to the Internet Relay Network " + this->_clients[i]->getID()));
	}
	return ("");
};

void	Server::_quit(Request request, int i)
{
	std::string ret = this->_clients[i]->getUserprefix() + "QUIT ";
	if (request._args.size())
		ret.append(":" + request._args[0] + "\n");
	else
		ret.append("\n");
	std::map<std::string, Channel *> channels = this->_clients[i]->getUserChannel();
	std::map<std::string, Channel *>::iterator it = channels.begin();
	while (it != channels.end())
	{
		_sendToEveryone(it->second, ret, i);
		it++;
	}
	this->_clients[i]->leaveAllC();
	_removeFromPoll(i);
};

std::string	Server::_printHelpInfo()
{
	std::string	helpInfo;
	helpInfo.append("STEP 1: PASS\n");
	helpInfo.append("\tUse PASS command to set a password. e.g: PASS [Server Password]\n\n");
	helpInfo.append("STEP 2: NICK\n");
	helpInfo.append("\tUse NICK command to set a nickname. e.g: NICK Robo\n\n");
	helpInfo.append("STEP 3: USER\n");
	helpInfo.append("\tUse USER command to register your username and fullname.e.g: USER robo * * :robo serv\n\n");
	return (helpInfo);
};

std::string Server::inviteRequest(Request request, int i)
{
    Client *client = getClientByNickname(request._args[0]);
    if (!client)
        return (_printMessage("401", this->_clients[i]->getNickname(), request._args[0] + " :No such nick"));
    Channel *channel = getChannelByName(request._args[1]);
    if (!channel)
        return (_printMessage("403", this->_clients[i]->getNickname(), request._args[1] + " :No such channel"));
    std::pair<Client *, int> user = channel->findUserRole(i);
    if (user.second != 1)
        return (_printMessage("482", this->_clients[i]->getNickname(), request._args[1] + " :You're not channel operator"));
    if (!channel->isInviteOnly())
        return (_printMessage("518", this->_clients[i]->getNickname(), request._args[1] + " :Channel is not invite only"));
    if (client->isInChannel(channel->getName()))
        return (_printMessage("443", this->_clients[i]->getNickname(), request._args[1] + " :You're already in that channel"));
    std::map<std::string, Client *> listofbanned = channel->getBanned();
    if (listofbanned.find(client->getNickname()) != listofbanned.end())
        return (_printMessage("465", this->_clients[i]->getNickname(), request._args[1] + " :You're banned from that channel"));
    if (client->isUserInvited(channel->getName()))
        return (_printMessage("443", this->_clients[i]->getNickname(), request._args[1] + " :You're already invited to that channel"));
    client->setInvite(channel->getName(), true);
    return (_printMessage("341", this->_clients[i]->getNickname(), request._args[0] + " " + request._args[1]));
}