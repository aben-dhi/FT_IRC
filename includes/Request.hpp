/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:07:02 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/08/17 15:53:13 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "Server.hpp"

class Request
{
	public:
	std::string		_command;
	std::vector<std::string>	_args;
	bool			_isValid;

	Request();
	~Request();
	Request(const Request &src);
	Request &operator=(const Request &src);
};

#endif
