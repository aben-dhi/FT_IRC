/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:08:46 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/08/09 13:09:38 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/File.hpp"

File::File() {};
File::File(std::string name, std::string path, std::string sender, std::string receiver) : name(name), path(path), sender(sender), receiver(receiver) {};
File::File(const File &src) { *this = src; };
File::~File() {};

File& File::operator=( const File& src )
{
	if (this == &src)
		return (*this);
	this->name = src.name;
	this->path = src.path;
	this->sender = src.sender;
	this->receiver = src.receiver;
	return (*this);
};