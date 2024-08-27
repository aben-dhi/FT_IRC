/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 00:41:58 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/08/17 17:18:36 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return (1);
	}
	Server server("IRC", argv[2], 10, argv[1]);
	try
	{
		server._run();
	}
	catch(const std::exception& e) 
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
