/*
 *	BIRD -- Declarations Common to Unix Port
 *
 *	(c) 1998 Martin Mares <mj@ucw.cz>
 *
 *	Can be freely distributed and used under the terms of the GNU GPL.
 */

#ifndef _BIRD_UNIX_H_
#define _BIRD_UNIX_H_

/* io.c */

void io_init(void);
void io_loop(void);
void get_sockaddr(struct sockaddr_in *sa, ip_addr *a, unsigned *port);

/* sync-if.c */

extern int if_scan_sock;
extern int if_scan_period;

void scan_if_init(void);

#endif