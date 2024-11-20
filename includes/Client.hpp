/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 23:49:03 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/20 06:08:17 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Server.hpp"

class Channel;

struct Modes
{
	bool	away;
	bool	invisible;
	bool	wallops;
	bool	rest;
	bool	oper;
	bool	local;
	bool	server;
};

const std::string currentDateTime();
std::string fill(std::string str, size_t len);

class Client
{
	public:
		Client();
		Client(int fd);
		~Client();
		Client(const Client &src);
		Client &operator=(const Client &src);

	private:
		int	_clientfd;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		const std::string	_hostname;	
		std::string	_ID;
		bool	_registered;
		bool	_isop;
		bool	_auth;
		struct sockaddr_storage	_remoteaddr;
		struct Modes	_modes;
		socklen_t	_remoteaddr_len;
		std::map<std::string, Channel*>	_isChannel;
		std::string _buffer;

	public:
		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getRealname() const;
		std::string	getID() const;
		std::string	getHostname() const;
		int	getClientfd() const;
		bool	getAuth() const;
		int getRegistered() const;
		int	getIsop() const;
		int	getModes(char mode) const;
		std::string	getUserprefix() const;
		std::string	getUserInfo() const;
		std::string	getChannels() const;
		std::map<std::string, Channel*>	getUserChannel() const;

		
		void	setNickname(std::string nickname);
		void	setUsername(std::string username);
		void	setRealname(std::string realname);

		void	setID(std::string ID);
		void    setClienfd(int fd);
		void	setAuth(bool auth);
		void	setRegistered(int registered);
		void	setIsop(bool isop);
		void	setModes(int value, char mode);
		void	setChannel(std::string channel, Channel *channel_ptr);
		void	removeChannel(std::string channel);
		std::string	leaveAllC();

		int	isJoined(std::string channel) const;
		void	joinChannel(std::string ChannelName, Channel *channel);
		std::string joinedChannels() const;
		std::string get_buffer();
		void set_buffer(std::string buffer);
		void clear_buffer();
};

#endif