/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 23:39:29 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/24 04:01:57 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::string Server::_kickedFromChannel(std::string ChannelName, std::string message, std::vector<std::string> users, int i)
{
    std::map<std::string, Channel*>::iterator it = this->_channels.find(ChannelName);
    if (it != this->_channels.end())
    {
        std::pair<Client*, int> user = it->second->findUserRole(i);

        if (user.second == 1) // Channel operator
        {
            std::string result; // Accumulate error messages
            for (std::vector<std::string>::iterator userIt = users.begin(); userIt != users.end(); ++userIt)
            {
                int ret = _findByNickname(*userIt);
                if (ret == USERNOTINCHANNEL)
                {
                    result += _printMessage("441", this->_clients[i]->getNickname(), (*userIt) + " " + ChannelName + " :They aren't on that channel");
                    continue;
                }

                // Construct and broadcast the KICK message
                std::string reply = "KICK " + ChannelName + " " + *userIt;
                if (!message.empty())
                    reply += " :" + message;
                _sendToEveryone2(it->second, reply, i);

                // Update channel state
                // it->second->banUser(this->_clients[ret]);
                _partChannel(ChannelName, ret, "", 0);
            }
            return result; // Only error messages are returned
        }

        // Error: User is not an operator or not in the channel
        if (user.second == -1)
            return _printMessage("442", this->_clients[i]->getNickname(), ChannelName + " :You're not on that channel");
        else
            return _printMessage("482", this->_clients[i]->getNickname(), ChannelName + " :You're not channel operator");
    }

    // Error: Channel does not exist
    return _printMessage("403", this->_clients[i]->getNickname(), ChannelName + " :No such channel");
}



// Process multiple channels and accumulate all results for compliance with ModernIRC
std::string Server::_kick(Request request, int i)
{
    // Check if the user is registered
    if (!this->_clients[i]->getRegistered())
        return _printMessage("451", this->_clients[i]->getNickname(), ":You have not registered");

    // Validate the number of arguments
    if (request._args.size() < 2)
        return _printMessage("461", this->_clients[i]->getNickname(), "KICK :Not enough parameters");

    // Separate channels and users
    std::vector<std::string> channels = _commaSeparator(request._args[0]);
    std::vector<std::string> users = _commaSeparator(request._args[1]);

    // Iterate through channels and accumulate results
    std::string result;
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        std::string ret;
        if (request._args.size() == 3)
            ret = _kickedFromChannel(*it, request._args[2], users, i);
        else
            ret = _kickedFromChannel(*it, "", users, i);

        // Accumulate results
        if (!ret.empty())
            result += ret;
    }

    return result; // Return accumulated results
}
