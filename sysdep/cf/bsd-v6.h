/*
 *	Configuration for *BSD based systems (tested on FreeBSD and NetBSD)
 *
 *	(c) 2004 Ondrej Filip <feela@network.cz>
 *
 *	Can be freely distributed and used under the terms of the GNU GPL.
 */

#define IPV6

#define CONFIG_AUTO_ROUTES
#undef CONFIG_SELF_CONSCIOUS
#undef CONFIG_MULTIPLE_TABLES

#undef CONFIG_UNIX_IFACE
#undef CONFIG_UNIX_SET

#define CONFIG_ALL_MULTICAST
#define CONFIG_UNNUM_MULTICAST

/*
Link: sysdep/unix
Link: sysdep/bsd
 */