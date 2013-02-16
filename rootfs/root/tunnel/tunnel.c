#define USE_TCC
//#define USE_STD

#ifdef USE_STD
#include <net/if.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/if_tun.h>
#include <stdlib.h>
#include <errno.h>

#else

#include "headers.h"

#ifdef USE_TCC

#include <tcclib.h>
#else

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#endif
#endif

int tunnel_id;

//http://www.mjmwired.net/kernel/Documentation/networking/tuntap.txt
//http://tier.cs.berkeley.edu/wiki/HOWTO:IPTunnelling < useful, too.	
int tun_alloc(char *dev)
{
	unsigned i;
	struct ifreq ifr;
//	printf( "SIO: %d / %d / %d\n", sizeof( ifr ), (unsigned int)&(ifr./*ifr_ifru.ifru_newname*/ifr_name) - (unsigned int)&ifr, IFF_TUN  );
	int fd, err;
	fd = open("/dev/net/tun", O_RDWR);
	if( fd < 0 )
	{
		fprintf( stderr, "Error.  Could not open /dev/net/tun.\n" );
		return -1;
	}

//	printf( "FD for /dev/net/tun: %d\n", fd );
	if( fd < 1 )
	{
		fprintf( stderr, "ERROR: Failed to insert TUN device.\n" );
		return -1;
	}

	memset(&ifr, 0, sizeof(ifr));
	// Flags: IFF_TUN   - TUN device (no Ethernet headers) 
	//        IFF_TAP   - TAP device  
	//
	//        IFF_NO_PI - Do not provide packet information  
	ifr.ifr_flags = IFF_TUN; 
	if( *dev )
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);

//	for( i = 0; i < sizeof( ifr ); i++ )
//	{
//		printf( "%02X ", ((unsigned char*)(&ifr))[i] );
//	}
//	printf( "\n" );

	err = ioctl(fd, TUNSETIFF, (void *) &ifr);

	if( err < 0 )
	{
		int eno = errno;

		printf( "Ioctl failed %d (%s) / error: %d\n", errno, strerror(eno), err );
		close(fd);
		return err;
	}
	strcpy(dev, ifr.ifr_name);
	return fd;
}      

	int s1;


int main()
{

	s1 = open( "/dev/ttyS1", (02) ); //O_RDWR

	char gdev[1024] = {0};
	tunnel_id = tun_alloc( gdev );
	if( tunnel_id < 0 )
	{
		fprintf( stderr, "Fatal error (%d)\n", tunnel_id );
		return -1;
	}
	printf( "%d : %s\n", tunnel_id , gdev );

	printf( "Commands done. Tunnel Ready.\n" );

	if( fork() )
	{
		unsigned char buffer[20000];
		int bufferplace = 0;
		int target = 20000;
		while(1)
		{
			do
			{
				int ret = read( s1, buffer + bufferplace, 20000 );
				if( ret == 0 || ret < 0 )
				{
					fprintf( stderr, "Error reading serial stream (%s)\n", gdev );
					continue;
				}

				if( bufferplace + ret > 20000 )
				{
					fprintf( stderr, "Error: incoming stream overflows buffer (%s)\n", gdev );
					bufferplace = 0;
					continue;
				}

				bufferplace += ret;

				if( bufferplace > 2 )
				{
					unsigned short tlr = ntohs(*((unsigned short*)buffer));
					if( bufferplace >= tlr+2 )
						break;
				}
			}

			if( ret > 0 ) write( tunnel_id, buffer, bufferplace );
			if( ret == 0 || ret < 0 )
			{
				fprintf( stderr, "Error reading \"%s\".\n", gdev );
			}
			bufferplace = 0;
		}

	}
	else
	{
		while(1)
		{
			unsigned char buffer[20000];
			int ret = read( tunnel_id, buffer, 20000 );
			if( ret > 0 )
			{
				unsigned short rettosend = htons( ret );
				write( s1, &rettosend, 2 );
				write( s1, buffer, ret );
			}
//			printf( "B: %d\n", ret );
			if( ret == 0 || ret < 0 )
			{
				fprintf( stderr, "Error reading \"%s\".\n", gdev );
			}
		}
	}
}

