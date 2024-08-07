/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 00:46:28 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/08/05 13:13:29 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <cstring>
#include <algorithm>
#include <utility>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <time.h>
#include "../includes/Request.hpp"
#include "../includes/Channel.hpp"
#include "../includes/File.hpp"
#include "../includes/Client.hpp"

class Client;
class Channel;
class File;
class Request;

class Server
{
private:
	std::string	_name;
	std::string	_password;
	int	_socketfd;
	std::map<int, Client*>	_clients;
	struct pollfd		*_pfds;
	int	_online_c;
	int	_max_c;
	std::string	_prefx;
	std::map<std::string, Channel*>	_channels;
	std::vector<std::string>	_wrongUser;
	std::vector<std::string>	_nicknames;
	std::map<std::string, File*>	_files;
	
private:
	Server();
	void	_getSocket(std::string port);
	std::string	_printMessage(std::string message, std::string nickname, std::string channel);
	void	_newClient();
	std::string	_getPassword() const;
	void	_addToPoll(int fd);
	std::string	_welcomeMessage();
	void	_removeFromPoll(int fd);
	int _sendAll(int fd, std::string message);
	std::string	_sendToEveryone(Channel *channel,std::string message, int fd);
	
	
	
public:
	Server(std::string name, std::string password, int max_c, std::string port);
	Server(const Server &src);
	Server &operator=(const Server &src);
	~Server();

	void	_run();
};

#endif