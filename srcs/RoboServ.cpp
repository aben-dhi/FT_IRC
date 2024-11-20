/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RoboServ.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 14:50:20 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/20 06:09:11 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"


std::string	Server::_getBotMessage()
{
	std::string Greeting;
	Greeting.append("\n\n\n\n\n\n\n██████████████████████████████████████████████████████████████████████████████████\n");
	Greeting.append("█                                                                                █\n");
	Greeting.append("█\t ██████╗  ██████╗ ██████╗  ██████╗ ███████╗███████╗██████╗ ██╗   ██╗ \t █\n");
	Greeting.append("█\t ██╔══██╗██╔═══██╗██╔══██╗██╔═══██╗██╔════╝██╔════╝██╔══██╗██║   ██║ \t █\n");
	Greeting.append("█\t ██████╔╝██║   ██║██████╔╝██║   ██║███████╗█████╗  █████╔╝ ██║   ██║ \t █\n");
	Greeting.append("█\t ██╔══██╗██║   ██║██╔══██╗██║   ██║╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝ \t █\n");
	Greeting.append("█\t ██║  ██║╚██████╔╝██████╔╝╚██████╔╝███████║███████╗██║  ██║ ╚████╔╝  \t █\n");
	Greeting.append("█\t ╚═╝  ╚═╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝   \t █\n");
	Greeting.append("█                                                                                █\n");
	Greeting.append("██████████████████████████████████████████████████████████████████████████████████\n");

	Greeting.append( "█                                                                                █\n");
	Greeting.append( "█\t"  " Usage: ROBOSERV [COMMAND_NUMBER] [MORE_OPTIONS]"  "\r\t\t\t\t\t\t\t\t\t\t █\n");
	Greeting.append( "█\t How Can I Help You: (You can use The following list of commands)\r\t\t\t\t\t\t\t\t\t\t █\n");
	Greeting.append( "█\t "  "[0]"  " : to List all Your stats"  "\r\t\t\t\t\t\t\t\t\t\t █\n");
	Greeting.append( "█\t "  "[1]"  " : to List all Your Joined Channels"  "\r\t\t\t\t\t\t\t\t\t\t █\n");
	Greeting.append( "█\t "  "[2]"  " : to see How many user online"  "\r\t\t\t\t\t\t\t\t\t\t █\n");
	Greeting.append( "█\t "  "[3]"  " : to List all Channels in Server"  "\r\t\t\t\t\t\t\t\t\t\t █\n");
	Greeting.append( "█\t "  "[4]"  " : to List stats of specific Channel"  "\r\t\t\t\t\t\t\t\t\t\t █\n");
	Greeting.append( "█\t "  "[5]"  " : to List Infos about the Server"  "\r\t\t\t\t\t\t\t\t\t\t █\n");
	Greeting.append( "█                                                                                █\n");
	Greeting.append( "██████████████████████████████████████████████████████████████████████████████████\n\n\n\n");
	return (Greeting);
};

std::string	Server::_RoboServ( Request req, int i )
{
	std::string Greeting(_getBotMessage());
	if (req._args.size() == 0)
	{
		return (Greeting);
	}
	if (req._args.size() > 0)
	{
		if (req._args[0] == "0")
			return (this->_clients[i]->getUserInfo());
		else if (req._args[0] == "1")
			return (this->_clients[i]->getChannels());
		else if (req._args[0] == "2")
			return ("Online Users: " + std::string(to_cstr(this->_online_c - 1) + "\n"));
		else if (req._args[0] == "3")
			return (_listAllChannels());
		else if (req._args[0] == "4")
		{
			if (req._args.size() == 2)
				return(_channelInfo(req._args[1], i));
			else
				return ("Usage of this Command: ROBOSERV 4 [CHANNEL NAME]\n");
		}
		else if (req._args[0] == "5")
			return (_serverInfo());
		else
			return (Greeting);
	}
	return (Greeting);
};

std::string Server::_serverInfo() const
{
	std::string server("Server Name: " + this->_name + "\n");
	server.append("Online Users: " + std::string(to_cstr(this->_online_c - 1) + "\n"));
	server.append("Max Online Users: " + std::string(to_cstr(this->_max_c) + "\n"));
	server.append("Number of Channels in the Server: " + std::string(to_cstr(this->_channels.size()) + "\n"));
	return (server);
}

std::string Server::_channelInfo(std::string ChannelName, int i)
{
	std::map<std::string, Channel *>::const_iterator it = this->_channels.find(ChannelName);
	if (it != this->_channels.end())
	{
		if (this->_clients[i]->isJoined(ChannelName))
		{
			std::string Info;
			Info.append("Channel Name: " + it->second->getName() + "\n");
			Info.append("Channel Creator: " + it->second->getCreator()->getRealname() + "\n");
			Info.append("Online Users: " + std::string(to_cstr(it->second->getOnlineUsers()) + "\n"));
			Info.append("Channel Topic: " + it->second->getTopic() + "\n");
			return (Info);
		}
		else
			return ("You Need To Join th channel first\n");
	}
	return ("There's No Channel Named " + ChannelName + "!\n");
}

std::string	Server::_listAllChannels() const
{
	std::string channels( "███████████████████████████████████████████████████████████████████████████████████████\n");
	channels.append("█              █              █                    █                                  █\n");
	channels.append("█"  " Channel Name "  "█ "  "Online Users "  "█ "  "Creator Of Channel "  "█ "  "          Channel Topic          "  "█\n");
	channels.append("█              █              █                    █                                  █\n");
	channels.append("███████████████████████████████████████████████████████████████████████████████████████\n");
	std::map<std::string, Channel *>::const_iterator it = this->_channels.begin();
	while (it != this->_channels.end())
	{
		channels.append("█              █              █                    █                                  █\n");
		channels.append("█ "  + fill(it->first, 12));
		channels.append( "█      "  + fill(std::string(to_cstr(it->second->getOnlineUsers())), 7));
		channels.append( "█ "  + fill(it->second->getCreator()->getRealname(), 18));
		channels.append( "█ "  + fill(it->second->getTopic(), 32));
		channels.append( "█\n");
		channels.append( "█              █              █                    █                                  █\n");
		channels.append("███████████████████████████████████████████████████████████████████████████████████████\n");
		it++;
	};
	if (this->_channels.size() == 0)
	{
		channels.append("█                                                                                     █\n");
		channels.append("█                               "  "NO CHANNEL IN THE SERVER"  "                              █\n");
		channels.append("█                                                                                     █\n");
		channels.append("███████████████████████████████████████████████████████████████████████████████████████\n");
	}
	channels.append( "\n\n");
	return (channels);
};