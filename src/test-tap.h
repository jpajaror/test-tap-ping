/**
 *
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/sockio.h>
#include <net/if.h>
#include <netinet/in.h>

#define TUN_PATH "/dev/tap0"


//void tap_close();

struct sockaddr_in socket_address = {
	.sin_family = AF_INET,
	.sin_port = 0
}; // sin_addr will get set next:

static struct ifreq ifr;

