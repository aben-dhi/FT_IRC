/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:11:41 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/08/09 13:12:40 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

Request::Request() : _command(), _args(), _isValid(false)
{
}

Request::Request(const Request &src)
{
	*this = src;
}

Request &Request::operator=(const Request &src)
{
	if (this != &src)
	{
		this->_command = src._command;
		this->_args = src._args;
		this->_isValid = src._isValid;
	}
	return (*this);
}

Request::~Request()
{
}