#ifndef ERRMESSAGES_HPP
# define ERRMESSAGES_HPP

// //INVITE
# define ERR_NEEDMOREPARAMS(command) (":localhost 461 " + client->getNick() + " " + command + " :Not enough parameters.\r\n")
# define ERR_NOSUCHCHANNEL(channel) (":localhost 403 " + client->getNick() + " #" + channel + " :No such channel\r\n")
# define ERR_NOTONCHANNEL(channel) (":localhost 442 " + client->getNick() + " #" + channel + ": The user is not on this channel.\r\n")
# define ERR_USERONCHANNEL(nick, channel) (":localhost 443 " + client->getNick() + " " + nick + " #" + channel + " :Is already on channel\r\n")
# define RPL_INVITING( nick, channel) (client->getNick()  + " 341 " + client->getNick() + " " + nick + " #" + channel + "\r\n")
# define RPL_INVITE(invited, channel) (client->getNick() + " INVITE " + invited + " #" + channel + "\r\n")

// //JOIN
# define RPL_JOIN(user_id, channel) (user_id + " JOIN :#" + channel + "\r\n")
# define ERR_BANNEDFROMCHAN(client, channel) (":localhost 474 " + client + " #" + channel + " :Cannot join channel (+b)\r\n")
# define ERR_BADCHANNELKEY(client, channel) (":localhost 475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n")
# define ERR_INVITEONLYCHAN (client, channel) (":localhost 473 " + client + " " + channel + " :Cannot join channel (i)\r\n")
# define ERR_CHANNELISFULL (client, channel) (":localhost 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")
# define RPL_ENDOFNAMES (client, channel) (":localhost 366" + client + " #" + channel + " :End of /NAMES list.\r\n")

// //KICK
// # define ERR_USERNOTINCHANNEL (client, nickname, channel) (":localhost 441" + client + " " + nickname + " #" +channel + " :They aren't on that channel\r\n")

// //TOPIC
// # define RPL_TOPIC (client, channel, topic) (":localhost 332 " + client + " #" + channel + " " + topic +"\r\n")
// # define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " #" + channel + " :No topic is set\r\n")

//NICK
# define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client + " :There is no nickname.\r\n")

#endif