/**
 *
 */
#include "test-tap.h"

int fd = -1;
int BUFF_LEN = 2049;
char *dev = "/dev/tap0";

void tap_close() {
	if (fd >= 0) close(fd);
	//Close the sockets
}

int tap_open(const char *device){
	if ((fd = open(device, O_RDWR)) < 0) {
		fprintf(stderr, "Opening %s failed. (Are we not root?)\n", dev);
		tap_close();
		return -1;
	}
	return fd;
}

int tap_read(char *buf, int len){
	int rcount;
	if ((rcount = read(fd, buf, len)) < 0) {
		//fprintf(stderr, "tap read failed\n");
		fprintf(stderr, "tap read failed\n");
		return -1;
	}
	return rcount;
}

#if defined(APPLE)
/**
 * Sets the IPv4 address for the device, given a string, such as
 * "172.31.0.100". The `prefix_len` specifies how many bits are specifying the
 * subnet, aka the routing prefix or the mask length.
 */
int
tap_set_ipv4_addr(const char *presentation, unsigned int prefix_len, char * my_ip4)
{
    struct sockaddr_in socket_address = {
        .sin_family = AF_INET,
        .sin_port = 0
    }; // sin_addr will get set next:

    // convert the presentation ip address string to a binary format (p-to-n)
    // thus filling in socket_address.sin_addr
    if (inet_pton(AF_INET, presentation, &socket_address.sin_addr) != 1) {
        fprintf(stderr, "inet_pton failed (Bad IPv4 address format?) - %s:%d\n", __FILE__, __LINE__);
        tap_close();
        return -1;
    }
    // we have to wrap our sockaddr_in struct into a sockaddr struct
    memcpy(&ifr.ifr_addr, &socket_address, sizeof(struct sockaddr));

    // Copies IPv4 address to my_ip4. IPv4 address starts at sa_data[2] and
    // terminates at sa_data[5]
    memcpy(my_ip4, ifr.ifr_addr.sa_data+2,4); 

    if (ioctl(ipv4_configuration_socket, SIOCSIFADDR, &ifr) < 0) {
        fprintf(stderr, "Failed to set IPv4 tap device address - %s:%d\n", __FILE__, __LINE__);
        tap_close();
        return -1;
    }

    //tap_plen_to_ipv4_mask(prefix_len, &ifr.ifr_netmask);
    if (ioctl(ipv4_configuration_socket, SIOCSIFNETMASK, &ifr) < 0) {
        fprintf(stderr, "Failed to set IPv4 tap device netmask - %s:%d\n", __FILE__, __LINE__);
        tap_close();
        return -1;
    }
    return 0;
}
#endif

int main(int argc, const char *argv[]) {
	unsigned char buf[BUFF_LEN], bufcpy[BUFF_LEN], bufout[BUFF_LEN];
	int buf_len;
	if (tap_open(dev)<0){
		return -1;
	}
	fprintf(stderr, "Tap device opened\n");
//Configuring ip address
/*
	if (ioctl(fd, SIOCSIFADDR, &ifr) < 0) {
		fprintf(stderr, "Failed to set IPv4 tap device address - %s:%d\n", __FILE__, __LINE__);
		tap_close();
		return -1;
	}*/
	fprintf(stderr, "ifconfig tap0 192.168.0.165/24 up\n");
	system("dtruss ifconfig tap0 192.168.0.165/24 up");
	while (1){
		memset(&buf, 0, BUFF_LEN);
		buf_len=tap_read(buf, BUFF_LEN);
		if (buf_len < 0){
			tap_close();
			fprintf(stderr, "Tap device read failed\n");
			break;
		}
		memcpy(bufcpy, buf, buf_len);
		char* buf_str = (char*) malloc (2*buf_len + 1);
		char* buf_ptr = buf_str;
		for (int i = 0; i < buf_len; i++) {
		    buf_ptr += sprintf(buf_ptr, "%02x", bufcpy[i]);
		}
		sprintf(buf_ptr,"\0");
		fprintf(stderr, "Packet received  %d=%s\n", buf_len, buf_str);
	}
	tap_close();
	fprintf(stderr, "Tap device closed\n");
	return 0;

}
