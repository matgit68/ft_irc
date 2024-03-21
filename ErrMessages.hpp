#pragma once

# define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")

# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
# define RPL_YOURHOST(client, servername, version) (":localhost 002 " + client + " :Your host is " + servername + " (localhost), running version " + version + "\r\n")
# define RPL_CREATED(client, datetime) (":localhost 003 " + client + " :This server was created " + datetime + "\r\n")
# define RPL_MYINFO(client, servername, version, user_modes, chan_modes, chan_param_modes) (":localhost 004 " + client + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
# define RPL_ISUPPORT(client, tokens) (":localhost 005 " + client + " " + tokens " :are supported by this server\r\n")

# define ERR_UNKNOWNCOMMAND(client, command) (":localhost 421 " + client + " " + command + " :Unknown command\r\n")
//INVITE
# define ERR_NEEDMOREPARAMS(command) (":localhost 461 " + client->getNick() + " " + command + " :Not enough parameters.\r\n")
# define ERR_NOSUCHCHANNEL(channel) (":localhost 403 " + client->getNick() + " " + channel + " :No such channel\r\n")
# define ERR_NOTONCHANNEL(channel) (":localhost 442 " + client->getNick() + " " + channel + ": The user is not on this channel.\r\n")
# define ERR_USERONCHANNEL(nick, channel) (":localhost 443 " + client->getNick() + " " + nick + " " + channel + " :Is already on channel\r\n")
# define RPL_INVITING(user_id, nick, channel) (user_id  + " 341 " + client->getNick() + " " + nick + " " + channel + "\r\n")
# define RPL_INVITE(user_id, invited, channel) (user_id + " INVITE " + invited + " " + channel + "\r\n")
# define RPL_INVITELIST(client, channel) (":localhost 336 " + client + " " + channel + "\r\n") // 346 or 336
# define RPL_ENDOFINVITELIST(client) (":localhost 337 " + client + " :End of /INVITE list")

//JOIN
# define RPL_JOIN(user_id, channel) (user_id + " JOIN :" + channel + "\r\n")
# define RPL_JOIN_NOTIF(user_id, channel) (":" + user_id + "@localhost JOIN " + channel + "\r\n")
# define ERR_BANNEDFROMCHAN(channel) (":localhost 474 " + client->getNick() + " " + channel + " :Cannot join channel (+b)\r\n")
# define ERR_BADCHANNELKEY (":localhost 475 " + client->getNick() + " " + _name + " :Cannot join channel (+k)\r\n")
# define ERR_INVITEONLYCHAN(channel) (":localhost 473 " + client->getNick() + " " + channel + " :Cannot join channel (i)\r\n")
# define ERR_CHANNELISFULL(channel) (":localhost 471 " + client->getNick() + " " + channel + " :Cannot join channel (+l)\r\n")
# define RPL_ENDOFNAMES(channel) (":localhost 366 " + client->getNick() + " " + channel + " :End of /NAMES list.\r\n")

//KICK
# define ERR_USERNOTINCHANNEL(client, target, channel) (":localhost 441 " + client->getNick() + " " + target + " " + channel + " :They aren't on that channel\r\n")

//TOPIC
# define RPL_TOPIC (":localhost 332 " + client->getNick() + " " + _name + " " + _topic +"\r\n")
# define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " " + channel + " :No topic is set\r\n")

//NICK
# define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client + " :There is no nickname.\r\n")
# define ERR_NICKNAMEINUSE(client, nickname) (":localhost 433 " + client + " " + nickname + " :Nickname is already in use.\r\n")
# define RPL_NICK(oclient, uclient, client) (":" + oclient + "!" + uclient + "@localhost NICK " + client + "\r\n")
# define ERR_ERRONEUSNICKNAME(client, nickname) (":localhost 432 " + client + " " + nickname + " :Erroneus nickname\r\n")
//NOTICE
# define RPL_NOTICE(nick, username, target, message) (":" + nick + "!" + username + "@localhost NOTICE " + target + " " + message + "\r\n")

//QUIT
# define RPL_QUIT(user_id, reason) (user_id + " QUIT : Quit: " + reason + "\r\n")
# define RPL_ERROR(user_id, reason) (user_id + " ERROR :" + reason + "\r\n")

//PRIVMSG
# define ERR_NOSUCHNICK(client, target) ("401 " + client + " " + target + " :No such nick/channel\r\n")
# define ERR_NORECIPIENT(client) ("411 " + client + " No recipient given PRIVMSG\r\n")
# define ERR_NOTEXTTOSEND(client) (":localhost 412" + client + " :No text to send\r\n")
//# define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + " " + message + "\r\n")
# define RPL_PRIVMSG(nick, name, msg) (":" + nick + " PRIVMSG " + name + " " + msg + "\r\n")
//# define ERR_CANNOTSENDTOCHAN(client, channel) ("404 " + client + " " + channel + " :Cannot send to channel\r\n")
# define RPL_AWAY(client, nick, message) ("301 " + client + " " + nick + " :" + message + "\r\n")

//USER
# define ERR_ALREADYREGISTERED(client) (":localhost 462 " + client + " :You may not reregister.\r\n")

//MODE
/* user mode */
# define MODE_USERMSG(client, mode) (":" + client + " MODE " + client + " :" + mode + "\r\n")
# define ERR_UMODEUNKNOWNFLAG() (":localhost 501 " + client->getNick() + " :Unknown MODE flag\r\n")
# define ERR_USERSDONTMATCH(client) ("502 " + client + " :Cant change mode for other users\r\n")
# define RPL_UMODEIS(client, mode) (":localhost 221 " + client + " " + mode + "\r\n")
/* channel mode */
# define MODE_CHANNELMSG(channel, mode) (":localhost MODE " + channel + " " + mode + "\r\n")
# define MODE_CHANNELMSGWITHPARAM(channel, mode, param) (":localhost MODE " + channel + " " + mode + " " + param + "\r\n")
# define RPL_CHANNELMODEIS(client, channel, mode) (":localhost 324 " + client + " " + channel + " " + mode + "\r\n")
# define RPL_CHANNELMODEISWITHKEY(client, channel, mode, password) (":localhost 324 " + client + " " + channel + " " + mode + " " + password + "\r\n")
# define ERR_CANNOTSENDTOCHAN(client, channel) ("404 " + client + " " + channel + " :Cannot send to channel\r\n")
# define ERR_CHANOPRIVSNEEDED(client, channel) (":localhost 482 " + client + " " + channel + " :You're not channel operator\r\n")
# define ERR_INVALIDMODEPARAM(client, channel, mode, password) ("696 " + client + " " + channel + " " + mode + " " + password + " : password must only contained alphabetic character\r\n")
# define RPL_ADDVOICE(nickname, username, channel, mode, param) (":" + nickname + "!" + username + "@localhost MODE " + channel + " " + mode + " " + param + "\r\n")

//NAMES
# define RPL_NAMREPLY (":localhost 353 " + client->getNick() + " = " + _name + " :" + client->getNick() + "\r\n")