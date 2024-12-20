/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transferFile.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ta9ra9 <ta9ra9@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 15:32:08 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/11/23 10:44:28 by ta9ra9           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::string	Server::_sendFile(Request request, int i)
{
	if (!this->_clients[i]->getRegistered())
		return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));
	if (request._args.size() < 2)
		return (_printMessage("461", this->_clients[i]->getNickname(), " SENDFILE :Not enough parameters"));
	if (_findByNickname(request._args[0]) == USERNOTFOUND)
		return (_printMessage("401", this->_clients[i]->getNickname(), request._args[0] + " :No such nick/channel"));
	std::fstream ifs(request._args[1].c_str(), std::fstream::in);
	if (ifs.fail())
		return (_printMessage("998", this->_clients[i]->getNickname(), ":Invalid file path"));
	size_t	pos = request._args[1].find_last_of('/');
	std::string	filename = request._args[1].substr(pos + 1);
	File file(filename, request._args[1], this->_clients[i]->getNickname(), request._args[0]);
	if (this->_files.find(filename) != this->_files.end())
		return (_printMessage("996", this->_clients[i]->getNickname(), ":File by this name already exists"));
	this->_files.insert(std::pair<std::string, File>(filename, file));
	_privToUser(request._args[0], this->_clients[i]->getNickname() + " wants to send you a file called " + filename + ".", "NOTICE", i);
	return ("");
}

std::string	Server::_getFile(Request request, int i)
{
	if (!this->_clients[i]->getRegistered())
		return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));
	if (request._args.size() < 2)
		return (_printMessage("461", this->_clients[i]->getNickname(), " GETFILE :Not enough parameters"));
	if (this->_files.find(request._args[0]) == this->_files.end())
		return (_printMessage("995", this->_clients[i]->getNickname(), ":No such file on the server"));
	File file(this->_files.find(request._args[0])->second);
	if (file.receiver != this->_clients[i]->getNickname())
		return (_printMessage("994", this->_clients[i]->getNickname(), ":Permission Denied"));
    std::fstream	ofs((request._args[1] + "/" + request._args[0]).c_str(), std::fstream::out);
	std::fstream	ifs(file.path.c_str(), std::fstream::in);
	if (ofs.is_open())
		ofs << ifs.rdbuf();
	this->_files.erase(file.name);
	return ("");
}