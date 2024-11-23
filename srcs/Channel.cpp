/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:03:02 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/23 05:55:29 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel() : _prefix(), _creator(), _onlineUsers(), _name(), _key(), _topic(), _members(), _operators(),_banned() {};
Channel::Channel( const Channel& x ) { *this = x; };
Channel::Channel(std::string channelName, Client *Creator) : _prefix(), _creator(Creator), _onlineUsers(1), _name(channelName), _key(), _topic(), _members(), _operators(), _banned()
{
	this->_operators.insert(std::pair<int, Client *>(Creator->getClientfd(), Creator));
};
Channel::Channel(std::string channelName, std::string channelKey, Client *Creator) : _prefix(), _creator(Creator), _onlineUsers(1), _name(channelName), _key(channelKey), _topic(), _members(), _operators(), _banned()
{
	this->_operators.insert(std::pair<int, Client *>(Creator->getClientfd(), Creator));
};
Channel::~Channel() {};

Channel& Channel::operator=( const Channel& rhs )
{
	if (this == &rhs)
		return (*this);
	this->_prefix = rhs._prefix;
	this->_onlineUsers = rhs._onlineUsers;
	this->_name = rhs._name;
	this->_members.insert(rhs._members.begin(), rhs._members.end());
	this->_operators.insert(rhs._operators.begin(), rhs._operators.end());
	this->_modes = rhs._modes;
	return (*this);
};

char							const &Channel::getPrefix()			const { return this->_prefix; };
int								const &Channel::getOnlineUsers()	const { return this->_onlineUsers; };
std::string						const &Channel::getName() 			const { return this->_name; };
std::string const &Channel::getKey() const {
	if (!this->_modes.key.empty())
		return this->_modes.key;
	return this->_key;
};
std::string						const &Channel::getTopic()			const { return this->_topic; };
std::map<int, Client *>			const &Channel::getMembers()		const { return this->_members; };
std::map<int, Client *>			const &Channel::getOperators()		const { return this->_operators; };

Client*		Channel::getCreator() const { return (this->_creator); };

void	Channel::setPrefix(char prefix)			{ this->_prefix = prefix; };
void	Channel::setOnlineUsers(int online)		{ this->_onlineUsers = online; };
void	Channel::setName(std::string name)		{ this->_name = name; };
void	Channel::setKey(std::string key)		{ this->_key = key;};
void	Channel::setTopic(std::string topic)	{ this->_topic = topic; };

int	Channel::addMember( Client *member )
{
	if (std::find(this->_banned.begin(), this->_banned.end(), member->getNickname()) != this->_banned.end())
		return (BANNEDFROMCHAN);
	if (this->_members.find(member->getClientfd()) == this->_members.end())
	{
		this->_members.insert(std::pair<int, Client *>(member->getClientfd(), member));
		this->_onlineUsers++;
		return (USERISJOINED);
	};
	return (-1);
};

int	Channel::addOperator( Client *member )
{
	if (this->_creator->getClientfd() == member->getClientfd())
		return (0);
	if (std::find(this->_banned.begin(), this->_banned.end(), member->getNickname()) != this->_banned.end())
		return (BANNEDFROMCHAN);
	if (this->_operators.find(member->getClientfd()) == this->_operators.end())
	{
		this->_operators.insert(std::pair<int, Client *>(member->getClientfd(), member));
		// this->_onlineUsers++;
		return (USERISJOINED);
	};
	return (-1);
};

int	Channel::banUser( Client *member )
{
	if (std::find(this->_banned.begin(), this->_banned.end(), member->getNickname()) != this->_banned.end())
		return (BANNEDFROMCHAN);
	this->_banned.push_back(member->getNickname());
	return (USERISBANNED);
};

int	Channel::removeOperator( int i)
{
	if (this->_creator->getClientfd() == i)
		return (-1);
	this->_operators.erase(i);
	return (0);
	// this->_onlineUsers--;
};


void	Channel::removeBanned( std::string nicgetNickname )
{
	if (std::find(this->_banned.begin(), this->_banned.end(), nicgetNickname) != this->_banned.end())
		return ;
	this->_banned.erase(std::find(this->_banned.begin(), this->_banned.end(), nicgetNickname));
};

void	Channel::removeMember( int i)
{
	this->_members.erase(i);
	this->_onlineUsers--;
};


std::map<int, Client *>	Channel::getAllUsers() const
{
	std::map<int, Client *>	allUsers(this->_members.begin(), this->_members.end());
	allUsers.insert(this->_operators.begin(), this->_operators.end());
	return (allUsers);
};

std::pair<Client *, int> Channel::findUserRole( int i )
{
	std::map<int, Client *>::iterator it = this->_operators.find(i);
	if (it != this->_operators.end())
		return (std::pair<Client *, int>(it->second, 1));
	it = this->_members.find(i);
	if (it != this->_members.end())
		return (std::pair<Client *, int>(it->second, 0));
	return (std::pair<Client *, int>(NULL, -1));
};

std::string		Channel::listAllUsers() const
{
	std::string		AllUsers(":");
	std::map<int, Client *>::const_iterator it = this->_operators.begin();
	while (it != this->_operators.end())
	{
		AllUsers.append("@" + it->second->getNickname() + " ");
		it++;
	}
	it = this->_members.begin();
	while (it != this->_members.end())
	{
		AllUsers.append(it->second->getNickname() + " ");
		it++;
	}
	return (AllUsers);
};

void Channel::setInviteOnly(bool inviteOnly)
{
    _modes.inviteOnly = inviteOnly;
}

void Channel::setTopicRestricted(bool topicRestricted)
{
    _modes.topicRestricted = topicRestricted;
}

void Channel::setModeKey(const std::string &key)
{
    _modes.key = key;
}

void Channel::setOperatorPrivilege(bool operatorPrivilege)
{
    _modes.operatorPrivilege = operatorPrivilege;
}

void Channel::setUserLimit(int userLimit)
{
    _modes.userLimit = userLimit;
}

bool Channel::isInviteOnly() const
{
    return _modes.inviteOnly;
}

bool Channel::isTopicRestricted() const
{
	return _modes.topicRestricted;
}

const std::string& Channel::getModeKey() const
{
	return _modes.key;
}

bool Channel::hasOperatorPrivilege() const
{
	return _modes.operatorPrivilege;
}

int Channel::getUserLimit() const
{
    return _modes.userLimit;
}

bool Channel::isModeKey() const
{
	return !_modes.key.empty();
}