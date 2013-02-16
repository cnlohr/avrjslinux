#ifndef _HEADER_TT_H
#define _HEADER_TT_H

/* From various */
#define O_RDWR		00000002
#define errno (* __errno_location() )                                           
extern int *__errno_location() __attribute__((__const__));                      
extern char *strerror (int __errnum);
#define __user
typedef char *__caddr_t;


/* From ioctl */

#define _IOC_NRBITS	8
#define _IOC_TYPEBITS	8
# define _IOC_SIZEBITS	14

#define _IOC_NRSHIFT	0
#define _IOC_TYPESHIFT	(_IOC_NRSHIFT+_IOC_NRBITS)
#define _IOC_SIZESHIFT	(_IOC_TYPESHIFT+_IOC_TYPEBITS)
#define _IOC_DIRSHIFT	(_IOC_SIZESHIFT+_IOC_SIZEBITS)


#define _IOC(dir,type,nr,size) \
	(((dir)  << _IOC_DIRSHIFT) | \
	 ((type) << _IOC_TYPESHIFT) | \
	 ((nr)   << _IOC_NRSHIFT) | \
	 ((size) << _IOC_SIZESHIFT))

# define _IOC_WRITE	1U
# define _IOC_READ	2U
#define _IOC_TYPECHECK(t) (sizeof(t))
#define _IOW(type,nr,size)	_IOC(_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOR(type,nr,size)	_IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
/*#define IOCPARM_MASK    0x7f
#define IOC_VOID        0x20000000
#define IOC_OUT         0x40000000
#define IOC_IN          0x80000000
#define IOC_INOUT       (IOC_IN|IOC_OUT)*/
extern int ioctl (int __fd, unsigned long int __request, ...);


/* from linux/socket.h */


typedef unsigned short	sa_family_t;

struct sockaddr {
	sa_family_t	sa_family;	/* address family, AF_xxx	*/
	char		sa_data[14];	/* 14 bytes of protocol address	*/
};


/* From net/if stuff. */

#define	ifr_flags	ifr_ifru.ifru_flags
#define IFF_TUN		0x0001
#define ifr_name	ifr_ifrn.ifrn_name
#define TUNSETIFF	_IOW('T', 202, int)
#define IF_NAMESIZE	16
# define IFHWADDRLEN	6
# define IFNAMSIZ	IF_NAMESIZE


struct ifmap
  {
    unsigned long int mem_start;
    unsigned long int mem_end;
    unsigned short int base_addr;
    unsigned char irq;
    unsigned char dma;
    unsigned char port;
    /* 3 bytes spare */
  };


struct ifreq
  {
    union
      {
	char ifrn_name[IFNAMSIZ];	/* Interface name, e.g. "en0".  */
      } ifr_ifrn;

    union
      {
	struct sockaddr ifru_addr;
	struct sockaddr ifru_dstaddr;
	struct sockaddr ifru_broadaddr;
	struct sockaddr ifru_netmask;
	struct sockaddr ifru_hwaddr;
	short int ifru_flags;
	int ifru_ivalue;
	int ifru_mtu;
	struct ifmap ifru_map;
	char ifru_slave[IFNAMSIZ];	/* Just fits the size */
	char ifru_newname[IFNAMSIZ];
	__caddr_t ifru_data;
      } ifr_ifru;
  };

#endif

