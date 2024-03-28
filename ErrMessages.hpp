#pragma once

# define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")

# define RPL_WELCOME(client) 						(":" + client->getServer()->getHostname() + " 001 " + client->getNick() + " :Welcome to the Internet Relay Network " + client->getReal() + "\r\n")
# define RPL_YOURHOST(client)					 	(":" + client->getServer()->getHostname() + " 002 " + client->getNick() + " :Your host is " + client->getServer()->getHostname() + " , running version " + VERSION + "\r\n")
# define RPL_CREATED(client, datetime) 				(":" + client->getServer()->getHostname() + " 003 " + client->getNick() + " :This server was created " + datetime + "\r\n")
# define RPL_MYINFO(c, u_m, c_m, cp_m) 				(":" + client->getServer()->getHostname() + " 004 " + client->getNick() + " " + client->getServer()->getHostname() + " " + VERSION + " " + u_m + " " + c_m + " " + cp_m + "\r\n")
# define RPL_ISUPPORT(client, tokens) 				(":" + client->getServer()->getHostname() + " 005 " + client->getNick() + " " + tokens " :are supported by this server\r\n")

# define ERR_UNKNOWNCOMMAND(client, command) 		(":" + client->getServer()->getHostname() + " 421 " + client->getNick() + " " + command + " :Unknown command\r\n")

//CAP
# define RPL_CAPLS(server)							(":" + server + " CAP * LS:\r\n")
# define RPL_CAPEND(server) 						(":" + server + " CAP * END\r\n")

//INVITE
# define ERR_NEEDMOREPARAMS(command) 				(":" + client->getServer()->getHostname() + " 461 " + client->getNick() + " " + command + " :Not enough parameters.\r\n")
# define ERR_NOSUCHCHANNEL(channel) 				(":" + client->getServer()->getHostname() + " 403 " + client->getNick() + " " + channel + " :No such channel\r\n")
# define ERR_NOTONCHANNEL(channel) 					(":" + client->getServer()->getHostname() + " 442 " + client->getNick() + " " + channel + ": The user is not on this channel.\r\n")
# define ERR_USERONCHANNEL(nick, channel) 			(":" + client->getServer()->getHostname() + " 443 " + client->getNick() + " " + nick + " " + channel + " :Is already on channel\r\n")
# define RPL_INVITELIST(client, channel) 			(":" + client->getServer()->getHostname() + " 336 " + client->getNick() + " " + channel + "\r\n") // 346 or 336
# define RPL_ENDOFINVITELIST(client) 				(":" + client->getServer()->getHostname() + " 337 " + client->getNick() + " :End of /INVITE list")
# define RPL_INVITING(usr_id, nick, channel) 		(usr_id  + " 341 " + client->getNick() + " " + nick + " " + channel + "\r\n")
# define RPL_INVITE(user_id, invited, channel) 		(user_id + " INVITE " + invited + " " + channel + "\r\n")

//JOIN
# define ERR_BANNEDFROMCHAN(channel) 				(":" + client->getServer()->getHostname() + " 474 " + client->getNick() + " " + channel + " :Cannot join channel (+b)\r\n")
# define ERR_BADCHANNELKEY(client, channel) 		(":" + client->getServer()->getHostname() + " 475 " + client->getNick() + " " + channel->getName() + " :Cannot join channel (+k)\r\n")
# define ERR_INVITEONLYCHAN(channel) 				(":" + client->getServer()->getHostname() + " 473 " + client->getNick() + " " + channel + " :Cannot join channel (i)\r\n")
# define ERR_CHANNELISFULL(channel) 				(":" + client->getServer()->getHostname() + " 471 " + client->getNick() + " " + channel + " :Cannot join channel (+l)\r\n")
# define RPL_ENDOFNAMES(channel) 					(":" + client->getServer()->getHostname() + " 366 " + client->getNick() + " " + channel + " :End of /NAMES list.\r\n")
# define RPL_JOIN(user_id, channel) 				(user_id + " JOIN :" + channel + "\r\n")
# define RPL_JOIN_NOTIF(user_id, channel) 			(":" + user_id + "@localhost JOIN " + channel + "\r\n")

//KICK
# define ERR_USERNOTINCHANNEL(c, target, channel) 	(":" + client->getServer()->getHostname() + " 441 " + c->getNick() + " " + target + " " + channel + " :They aren't on that channel\r\n")

//TOPIC
# define RPL_TOPIC(client, channel)					(":" + client->getServer()->getHostname() + " 332 " + client->getNick() + " " + channel->getName() + " " + channel->getTopic() +"\r\n")
# define RPL_NOTOPIC(client, channel) 				(":" + client->getServer()->getHostname() + " 331 " + client->getNick() + " " + channel->getName() + " :No topic is set\r\n")

//NICK
# define ERR_NONICKNAMEGIVEN(client) 				(":" + client->getServer()->getHostname() + " 431 " + client->getNick() + " :There is no nickname.\r\n")
# define ERR_NICKNAMEINUSE(client, nickname) 		(":" + client->getServer()->getHostname() + " 433 " + client->getNick() + " " + nickname + " :Nickname is already in use.\r\n")
# define ERR_ERRONEUSNICKNAME(client, nickname) 	(":" + client->getServer()->getHostname() + " 432 " + client->getNick() + " " + nickname + " :Erroneus nickname\r\n")
# define RPL_NICK(oclient, uclient, client) 		(":" + oclient + "!" + uclient + "@" + client->getServer()->getHostname() + " NICK " + client->getNick() + "\r\n")
//NOTICE
# define RPL_NOTICE(nick, username, target, msg) 	(":" + nick + "!" + username + "@localhost NOTICE " + target + " " + msg + "\r\n")

//QUIT
# define RPL_QUIT(user_id, reason) 					(user_id + " QUIT : Quit: " + reason + "\r\n")
# define RPL_ERROR(user_id, reason) 				(user_id + " ERROR :" + reason + "\r\n")

//PRIVMSG
# define ERR_NOSUCHNICK(client, target) 			("401 " + client->getNick() + " " + target + " :No such nick/channel\r\n")
# define ERR_NORECIPIENT(client) 					("411 " + client->getNick() + " No recipient given PRIVMSG\r\n")
# define ERR_NOTEXTTOSEND(client) 					(":" + client->getServer()->getHostname() + " 412" + client->getNick() + " :No text to send\r\n")
# define RPL_PRIVMSG(client, chan, msg)				(":" + client->getNick() + " PRIVMSG " + chan->getName() + " " + msg + "\r\n")
# define RPL_AWAY(client, nick, message) 			("301 " + client->getNick() + " " + nick + " :" + message + "\r\n")

//USER
# define ERR_ALREADYREGISTERED(client) 				(":" + client->getServer()->getHostname() + " 462 " + client->getNick() + " :You may not reregister.\r\n")
# define ERR_PASSWDMISMATCH(client)                 (":" + client->getServer()->getHostname() + " 464 " + client->getNick() + " :Password incorrect.\r\n")
//MODE
/* user mode */
# define MODE_USERMSG(client, mode) 				(":" + client->getNick() + " MODE " + client->getNick() + " :" + mode + "\r\n")
# define ERR_UMODEUNKNOWNFLAG() 					(":" + client->getServer()->getHostname() + " 501 " + client->getNick() + " :Unknown MODE flag\r\n")
# define ERR_USERSDONTMATCH(client) 				("502 " + client + " :Cant change mode for other users\r\n")
# define RPL_UMODEIS(client, mode) 					(":" + client->getServer()->getHostname() + " 221 " + client->getNick() + " " + mode + "\r\n")
/* channel mode */
# define MODE_CHANNELMSG(channel, mode) 			(":" + client->getServer()->getHostname() + " MODE " + channel->getName() + " " + mode + "\r\n")
# define MODE_CHANNELMSGWITHPARAM(channel, mode, p) (":" + client->getServer()->getHostname() + " MODE " + channel->getName() + " " + mode + " " + p + "\r\n")
# define RPL_CHANNELMODEIS(client, channel, mode) 	(":" + client->getServer()->getHostname() + " 324 " + client->getNick() + " " + channel->getName() + " " + mode + "\r\n")
# define RPL_CHANNELMODEISWITHKEY(cl, ch, pwd) 		(":" + cl->getServer()->getHostname() + " 324 " + cl->getNick() + " " + ch->getName() + " " + ch->getMode() + " " + pwd + "\r\n")
# define ERR_CANNOTSENDTOCHAN(client, channel) 		("404 " + client->getNick() + " " + channel + " :Cannot send to channel\r\n")
# define ERR_CHANOPRIVSNEEDED(client, channel) 		(":" + client->getServer()->getHostname() + " 482 " + client->getNick() + " " + channel->getName() + " :You're not channel operator\r\n")
# define ERR_INVALIDMODEPARAM(cl, ch, mode, pwd) 	("696 " + cl->getNick() + " " + ch->getName() + " " + mode + " " + pwd + " : password must only contained alphabetic character\r\n")

//NAMES
# define RPL_NAMREPLY 								(":" + client->getServer()->getHostname() + " 353 " + client->getNick() + " = " + _name + " :" + client->getNick() + "\r\n")
//PART
# define RPL_PART(user_id, channel, reason) (":" + client->getNick() + "@localhost PART " + channel + " : " + reason  + "\r\n")
