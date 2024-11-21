/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 00:46:28 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/20 06:00:47 by aben-dhi         ###   ########.fr       */
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
#include <fcntl.h>
#include "../includes/Request.hpp"
#include "../includes/Channel.hpp"
#include "../includes/File.hpp"
#include "../includes/Client.hpp"
#include <sstream>

template<typename T>
std::string to_cstr(T value)
{
    std::stringstream ret;
    std::string miw;
    ret << value;
    miw = ret.str();
    return miw;
}


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
	std::map<std::string, File>	_files;
	
private:
	Server();
	void	_getSocket(std::string port);
	std::string	_printMessage(std::string message, std::string nickname, std::string channel);
	void	_newClient();
	std::string	_getPassword() const;
	void	_addToPoll(int fd);
	std::string	_welcomeMsg();
	void	_removeFromPoll(int fd);
	int _sendAll(int fd, std::string message);
	std::string	_sendToEveryone(Channel *channel,std::string message, int fd);
	void	_ClientRequest(int i);
	Request	_splitRequest(std::string req);
	std::string	_parsing(std::string message, int i);
	std::string _notice(Request request, int i);
	int _findByNickname(std::string nickname);
	std::string _topic(Request request, int i);
	bool	_validMode(Request request);
	// std::string _printUserModes(std::string ret, int i);
	std::string _setMode(Request request, int i);
	std::string	_setOper(Request request, int i);
	std::string	_setPassWord(Request request, int i);
	std::string	_setNickName(Request request, int i);
	std::string	_setUserName(Request request, int i);
	void	_quit(Request request, int i);
	std::string	_printHelpInfo();
	std::string	_joinChannel( Request request, int i );
	int	_createChannel( std::string ChannelName, int CreatorFd );
	int	_createPrvChannel( std::string ChannelName, std::string ChannelKey, int CreatorFd);
	std::vector<std::string> _commaSeparator(std::string arg);
	std::string		_kickedFromChannel(std::string ChannelName, std::string message, std::vector<std::string> users, int i);
	std::string	_kick(Request request, int i);
	std::string	_part( Request request, int i );
	int	_partChannel( std::string ChannelName, int i, std::string message, int isPart );
	std::string	_privmsg(Request request, int i);
	std::string 	_privToUser(std::string User, std::string message, std::string cmd, int i);
	std::string 	_privToChannel(std::string ChannelName, std::string message, int i);
	std::string	_getBotMessage();
	std::string	_RoboServ( Request req, int i );
	std::string _serverInfo() const;
	std::string _channelInfo(std::string ChannelName, int i);
	std::string	_listAllChannels() const;
	std::string	_sendFile(Request request, int i);
	std::string	_getFile(Request request, int i);
	std::vector<std::string> splitBuffer(std::string buffer);
	
public:
	Server(std::string name, std::string password, int max_c, std::string port);
	Server(const Server &src);
	Server &operator=(const Server &src);
	~Server();

	void	_run();
};

#endif