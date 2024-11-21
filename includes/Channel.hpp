/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-dhi <aben-dhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:10:13 by aben-dhi          #+#    #+#             */
/*   Updated: 2024/08/17 15:53:07 by aben-dhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#define	USERALREADYJOINED 0
#define USERISJOINED 1
#define NOTINCHANNEL 2
#define BANNEDFROMCHAN 3
#define TOOMANYCHANNELS 4
#define BADCHANNELKEY 5
#define CHANNELISFULL 6
#define NOSUCHCHANNEL 7
#define USERISBANNED 8
#define BADCHANMASK 9
#define USERNOTINCHANNEL -1
#define USERNOTFOUND -1
#include "Server.hpp"

class Client;

struct Modes
{
    bool inviteOnly;
    bool topicRestricted;
    std::string key;
    bool operatorPrivilege;
    int userLimit;

    Modes() : inviteOnly(false), topicRestricted(false), key(""), operatorPrivilege(false), userLimit(-1) {}
};


class Channel
{
	private:
		char	_prefix;
		Client*	_creator;
		int	_onlineUsers;
		std::string	_name;
		std::string	_key;
		std::string	_topic;
		std::map<int, Client *>	_members;
		std::map<int, Client *>	_operators;
		std::map<int, Client *>	_voice;
		std::vector<std::string>	_banned;
		struct Modes _modes;
		Channel();
		std::map<std::string, Client *> _bannedClients;
	public:
		Channel(std::string name, Client *creator);
		Channel(std::string name, std::string key, Client *creator);
		Channel(const Channel &src);
		Channel &operator=(const Channel &src);
		~Channel();
		
		char							const &getPrefix()		const;
		int								const &getOnlineUsers()	const;
		std::string						const &getName() 		const;
		std::string						const &getKey()			const;
		std::string						const &getTopic()		const;
		std::map<int, Client *>			const &getMembers()		const;
		std::map<int, Client *>			const &getOperators()	const;
		std::map<int, Client *>			const &getVoice()		const;
		std::map<std::string, Client *>	const &getBanned()		const { return _bannedClients; }

		Client*						getCreator() const;
		std::map<int, Client *>		getAllUsers() const;
		std::pair<Client *, int>	findUserRole( int i );
		std::string					listAllUsers() const;
		
		int	addMember(Client *member);
		int	addOperator(Client *member);
		int	banUser(Client *member);
		void	removeOperator(int i);
		void	removeVoice(int i);
		void	removeBanned(std::string nickName);
		void	removeMember(int i);

		void	setPrefix(char prefix);
		void	setOnlineUsers(int online);
		void	setName(std::string name);
		void	setKey(std::string key);
		void	setTopic(std::string topic);

		void setInviteOnly(bool inviteOnly);
        void setTopicRestricted(bool topicRestricted);
        void setModeKey(const std::string &key);
        void setOperatorPrivilege(bool operatorPrivilege);
        void setUserLimit(int userLimit);
        bool isInviteOnly() const;
        bool isTopicRestricted() const;
        const std::string& getModeKey() const;
        bool hasOperatorPrivilege() const;
        int getUserLimit() const;
		// const std::map<std::string, Client *> &getBanned() const { return _bannedClients;}
};

#endif