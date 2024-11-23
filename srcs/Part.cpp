/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 14:38:16 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/23 20:28:30 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

// int	Server::_partChannel( std::string ChannelName, int i, std::string message, int isPart )
// {
// 	std::map<std::string, Channel *>::iterator itCh= this->_channels.find(ChannelName);
// 	if (itCh == this->_channels.end() /* No such channel */)
// 		return (NOSUCHCHANNEL);
// 	else
// 	{
// 		std::pair<Client *, int> user(itCh->second->findUserRole(i));
// 		if (user.second == -1 /* Not in channel */)
// 			return (NOTINCHANNEL);
// 		else
// 		{
// 			if (user.second == 0)
// 				itCh->second->removeMember(i);
// 			else if (user.second == 1)
// 				itCh->second->removeOperator(i);
// 			user.first->removeChannel(itCh->second->getName());
// 			if (isPart == 1)
// 			{
// 				std::string reply = "PART " + ChannelName;
// 				if (message.empty())
// 					reply.append("\n");
// 				else
// 					reply.append(" " + message + "\n");
// 				_sendToEveryone(itCh->second, reply, i);
// 			}
// 		}
// 	}
// 	return (0);
// }

// int Server::_partChannel(std::string ChannelName, int i, std::string message, int isPart) {
//     std::map<std::string, Channel *>::iterator itCh = this->_channels.find(ChannelName);
//     if (itCh == this->_channels.end()) // No such channel
//         return (NOSUCHCHANNEL);
    
//     std::pair<Client *, int> user(itCh->second->findUserRole(i));
//     if (user.second == -1) // Not in channel
//         return (NOTINCHANNEL);
    
//     // Remove the user from the channel
//     if (user.second == 0)
//         itCh->second->removeMember(i);
//     else if (user.second == 1)
//         itCh->second->removeOperator(i);
    
//     user.first->removeChannel(itCh->second->getName());
    
//     if (isPart == 1) {
//         // Construct the PART message correctly
//         std::string reply = "PART " + ChannelName;
//         if (!message.empty())
//             reply += " :" + message; // Add reason if provided
//         reply += "\n"; // Ensure newline at the end

//         // Send the PART message to everyone in the channel
//         _sendToEveryone(itCh->second, reply, i);
//     }
//     return 0;
// }

// std::string	Server::_part( Request request, int i )
// {
// 	if (!this->_clients[i]->getRegistered())
// 		return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));
// 	if (request._args.size() == 0)
// 		return (_printMessage("461", this->_clients[i]->getNickname(), " PART:Not enough parameters"));
// 	std::vector<std::string>	parsChannels(_commaSeparator(request._args[0]));
// 	std::vector<std::string>::iterator it = parsChannels.begin();
// 	std::string nickname = this->_clients[i]->getNickname();
// 	std::string username = this->_clients[i]->getUsername();
// 	while (it != parsChannels.end())
// 	{
// 		std::string channelName = *it;
// 		//remove first char from channelName if its # or &
// 		// if (channelName[0] == '#' || channelName[0] == '&')
// 		// 	channelName.erase(0, 1);
// 		int j = 0;
// 		if (request._args.size() == 2) // PART with a reason
// 		{
// 			j = _partChannel(*it, i, request._args[1], 1);
// 			return (_printMessage(" PART " + channelName + 
// 								" :" + request._args[1] + "\n", 
// 								nickname, channelName));
// 		}
// 		else // PART without a reason
// 		{
// 			j = _partChannel(*it, i, "", 1);
// 			return (_printMessage(" PART " + channelName + "\n", 
// 								nickname, channelName));
// 		}

// 		if (j == NOSUCHCHANNEL /* No such channel */)
// 			return (_printMessage("403", nickname, channelName + " :No such channel"));
// 		if (j == NOTINCHANNEL /* Not in channel */)
// 			return (_printMessage("442", nickname, channelName + " :You're not on that channel"));
// 		it++;
// 	}
// 	return ("");
// };


int Server::_partChannel(std::string ChannelName, int i, std::string message, int isPart) {
    std::map<std::string, Channel *>::iterator itCh = this->_channels.find(ChannelName);
    if (itCh == this->_channels.end()) // No such channel
        return (NOSUCHCHANNEL);
    
    std::pair<Client *, int> user(itCh->second->findUserRole(i));
    if (user.second == -1) // Not in channel
        return (NOTINCHANNEL);
    
    // Remove the user from the channel
    if (user.second == 0)
        itCh->second->removeMember(i);
    else if (user.second == 1)
        itCh->second->removeOperator(i);
    
    user.first->removeChannel(itCh->second->getName());
    
    if (isPart == 1) {
        // Construct the PART message correctly
        std::string reply = "PART " + ChannelName;
        if (!message.empty())
            reply += " :" + message; // Add reason if provided
        reply += "\n"; // Ensure newline at the end

        // Send the PART message to everyone in the channel
        _sendToEveryone(itCh->second, reply, i);
    }
    return 0;
}

std::string Server::_part(Request request, int i) {
    if (!this->_clients[i]->getRegistered())
        return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));
    if (request._args.size() == 0)
        return (_printMessage("461", this->_clients[i]->getNickname(), " PART:Not enough parameters"));

    std::vector<std::string> parsChannels(_commaSeparator(request._args[0]));
    std::vector<std::string>::iterator it = parsChannels.begin();
    std::string nickname = this->_clients[i]->getNickname();
    std::string username = this->_clients[i]->getUsername();
    std::string result;

    while (it != parsChannels.end()) {
        std::string channelName = *it;
        int j = 0;
        std::string message;

        if (request._args.size() == 2) { // PART with a reason
            j = _partChannel(channelName, i, request._args[1], 1);
            message = ":" + nickname + "!" + username + "@localhost PART " + channelName + " :" + request._args[1];
        } else { // PART without a reason
            j = _partChannel(channelName, i, "", 1);
            message = ":" + nickname + "!" + username + "@localhost PART " + channelName;
        }

        if (j == NOSUCHCHANNEL) { // No such channel
            result += _printMessage("403", nickname, channelName + " :No such channel") + "\n";
        } else if (j == NOTINCHANNEL) { // Not in channel
            result += _printMessage("442", nickname, channelName + " :You're not on that channel") + "\n";
        } else {
            result += message + "\n";
        }
        it++;
    }
    return result;
}