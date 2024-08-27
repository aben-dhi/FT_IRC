/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 21:56:11 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/08/09 13:08:34 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
# define FILE_HPP

#include "Server.hpp"

class File
{
	public:
	std::string	name;
	std::string	path;
	std::string	sender;
	std::string	receiver;

	private:
	File();

	public:

	File(std::string name, std::string path, std::string sender, std::string receiver);
	File(const File &src);
	File &operator=(const File &src);
	~File();
};

#endif