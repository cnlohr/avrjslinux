//Copyright 2012 Charles Lohr under the MIT/x11, newBSD, LGPL or GPL licenses.  You choose.

#ifndef _IPARPETC_H
#define _IPARPETC_H

#include <stdint.h>
#ifdef INCLUDE_TCP
#include "tcp.h"
#endif

#define IP_HEADER_LENGTH 20

//enc28j60 calls this.
void enc28j60_receivecallback( uint16_t packetlen );

//You must define these.
extern unsigned char MyIP[];
extern unsigned char MyMask[];
extern unsigned char MyMAC[];


extern unsigned char macfrom[6];
extern unsigned char ipsource[4];
extern unsigned short remoteport;
extern unsigned short localport;

//Utility out
void send_etherlink_header( unsigned short type );
void send_ip_header( unsigned short totallen, const unsigned char * to, unsigned char proto );

void util_finish_udp_packet();

#define ipsource_uint ((uint32_t*)&ipsource)

#ifdef INCLUDE_TCP
void HandleTCP( uint16_t iptotallen );
#endif

void HandleUDP( uint16_t len );

#ifdef ARP_CLIENT_SUPPORT

//Returns -1 if arp not found yet.
//Otherwise returns entry into ARP table.

int8_t RequestARP( uint8_t * ip );

struct ARPEntry
{
	uint8_t mac[6];
	uint8_t ip[4];
};

extern uint8_t ClientArpTablePointer;
extern struct ARPEntry ClientArpTable[ARP_CLIENT_TABLE_SIZE];


#endif

#ifdef PING_CLIENT_SUPPORT

#ifndef ARP_CLIENT_SUPPORT
#error Client pinging requires ARP Client.
#endif

struct PINGEntries
{
	uint8_t ip[4];
	uint8_t id;  //If zero, not in use.
	uint16_t last_send_seqnum;
	uint16_t last_recv_seqnum;
};

extern struct PINGEntries ClientPingEntries[PING_RESPONSES_SIZE];

int8_t GetPingslot( uint8_t * ip );
void DoPing( uint8_t pingslot );


#endif


#endif

