/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:17:15 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/20 06:00:14 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client() : _clientfd(0), _nickname(), _username(), _realname(), _hostname("IRC"), _ID(), _registered(false),  _isop(false), _auth(false), _remoteaddr(), _modes(), _remoteaddr_len(0), _isChannel()
{
}

Client::Client(int fd) : _clientfd(fd), _nickname(), _username(), _realname(), _hostname("IRC"), _ID(), _registered(false),  _isop(false), _auth(false), _remoteaddr(), _modes(), _remoteaddr_len(0), _isChannel()
{
	
}

Client::Client(const Client &src) : _hostname(src._hostname)
{
	*this = src;
}

Client &Client::operator=(const Client &src)
{
	if (this != &src)
	{
		this->_clientfd = src._clientfd;
		this->_auth = src._auth;
		this->_registered = src._registered;
		this->_isop = src._isop;
		this->_username = src._username;
		this->_nickname = src._nickname;
		this->_realname = src._realname;
		this->_ID = src._ID;
		this->_remoteaddr = src._remoteaddr;
		this->_remoteaddr_len = src._remoteaddr_len;
		this->_modes = src._modes;
		this->_isChannel.insert(src._isChannel.begin(), src._isChannel.end());
	}
	return (*this);
}

Client::~Client()
{
}

void	Client::setNickname(std::string nickname) {this->_nickname = nickname;}
void	Client::setUsername(std::string username) {this->_username = username;}
void	Client::setRealname(std::string realname) {this->_realname = realname;}
void	Client::setID(std::string ID) {this->_ID = ID;}
void	Client::setRegistered(int registered) {this->_registered = registered;}
void	Client::setAuth(bool auth) {this->_auth = auth;}
void	Client::setIsop(bool isop) {
	this->_isop = isop;
	this->_modes.oper = isop;
	this->_modes.local = isop;
}
void	Client::setClienfd(int clientfd) {this->_clientfd = clientfd;}
void	Client::setModes(int value, char mode) {
	if (mode == 'i')
		this->_modes.invisible = value;
	else if (mode == 'w')
		this->_modes.wallops = value;
	else if (mode == 'r')
		this->_modes.rest = value;
	else if (mode == 'o')
		this->_modes.oper = value;
	else if (mode == 'l')
		this->_modes.local = value;
	else if (mode == 's')
		this->_modes.server = value;
}
void	Client::removeChannel(std::string channel) {this->_isChannel.erase(channel);}
void	Client::setChannel(std::string channel, Channel *channel_ptr) {
	if (!this->_isChannel.count(channel))
		this->_isChannel.insert(std::pair<std::string, Channel*>(channel, channel_ptr));
}




std::string	Client::getNickname() const {return (this->_nickname);}
std::string	Client::getUsername() const {return (this->_username);}
std::string	Client::getRealname() const {return (this->_realname);}
std::string	Client::getID() const {return (this->_ID);}
std::string	Client::getHostname() const {return (this->_hostname);}
int	Client::getClientfd() const {return (this->_clientfd);}
bool	Client::getAuth() const {return (this->_auth);}
int	Client::getRegistered() const {return (this->_registered);}
int	Client::getIsop() const {return (this->_isop);}
int	Client::getModes(char mode) const {
	if (mode == 'a')
		return (this->_modes.invisible);
	else if (mode == 'w')
		return (this->_modes.wallops);
	else if (mode == 'r')
		return (this->_modes.rest);
	else if (mode == 'o')
		return (this->_modes.oper);
	else if (mode == 'l')
		return (this->_modes.local);
	else if (mode == 's')
		return (this->_modes.server);
	return (0);
}

std::string	Client::getUserprefix() const {
	return (":" + this->_nickname + "!" + this->_username + "@" + this->_hostname + " ");
}

std::string	Client::getUserInfo() const {
	std::string info;
	info.append("User Name: " + this->_username + "\n");
	info.append("Real Name: " + this->_realname + "\n");
	info.append("Nick Name: " + this->_nickname + "\n");
	info.append("Host: " + this->_hostname+ "\n");
	info.append("Joined Channels: " + std::string(to_cstr(this->_isChannel.size()) + "\n"));
	info.append("\n");
	return (info);
}

std::map<std::string, Channel*>	Client::getUserChannel() const {return (this->_isChannel);}

std::string Client::leaveAllC()
{
	std::map<std::string, Channel*>::iterator it = this->_isChannel.begin();
	while(it != this->_isChannel.end())
	{
		std::pair<Client *, int> user = it->second->findUserRole(this->_clientfd);
		if (user.second == 0)
			it->second->removeMember(this->_clientfd);
		else if (user.second == 1)
			it->second->removeOperator(this->_clientfd);
		else
			it->second->removeVoice(this->_clientfd);
		user.first->removeChannel(it->second->getName());
		it = this->_isChannel.begin();
	}
	return ("");
}

std::string	Client::getChannels() const {
	std::string channels;
	std::map<std::string, Channel*>::const_iterator it = this->_isChannel.begin();
	while (it != this->_isChannel.end())
	{
		channels.append(it->first + " ");
		it++;
	}
	return (channels);
}

int		Client::isJoined( std::string Channel ) const
{
	if (this->_isChannel.find(Channel) != this->_isChannel.end())
		return (1);
	return (0);
};

void	Client::joinChannel( std::string ChannelName, Channel *channel )
{
	if (!isJoined(ChannelName))
		this->_isChannel.insert(std::pair<std::string, Channel *>(ChannelName, channel));
};

std::string Client::get_buffer() {return (this->_buffer);}
void Client::set_buffer(std::string buffer) {this->_buffer += buffer;}
void Client::clear_buffer() {this->_buffer.clear();}

