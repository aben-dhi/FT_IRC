/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client1.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:14:33 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/08/17 17:00:02 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"


std::string	Client::joinedChannels() const{
	std::string channels;
	std::map<std::string, Channel*>::const_iterator it = _isChannel.begin();
	while (it != _isChannel.end())
	{
		channels += it->first + " ";
		it++;
	}
	return (channels);
}
