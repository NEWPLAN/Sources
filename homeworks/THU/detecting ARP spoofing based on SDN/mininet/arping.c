/**
 * @file arp_request.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <netpacket/packet.h>

/* 以太网帧首部长度 */
#define ETHER_HEADER_LEN sizeof(struct ether_header)
/* 整个arp结构长度 */
#define ETHER_ARP_LEN sizeof(struct ether_arp)
/* 以太网 + 整个arp结构长度 */
#define ETHER_ARP_PACKET_LEN ETHER_HEADER_LEN + ETHER_ARP_LEN
/* IP地址长度 */
#define IP_ADDR_LEN 4
/* 广播地址 */
#define BROADCAST_ADDR {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}

void err_exit(const char *err_msg)
{
	perror(err_msg);
	exit(1);
}

/* 填充arp包 */
struct ether_arp *fill_arp_packet(const unsigned char *src_mac_addr, const char *src_ip, const char *dst_ip)
{
	struct ether_arp *arp_packet;
	struct in_addr src_in_addr, dst_in_addr;
	unsigned char dst_mac_addr[ETH_ALEN] = BROADCAST_ADDR;

	/* IP地址转换 */
	inet_pton(AF_INET, src_ip, &src_in_addr);
	inet_pton(AF_INET, dst_ip, &dst_in_addr);

	/* 整个arp包 */
	arp_packet = (struct ether_arp *)malloc(ETHER_ARP_LEN);
	arp_packet->arp_hrd = htons(ARPHRD_ETHER);
	arp_packet->arp_pro = htons(ETHERTYPE_IP);
	arp_packet->arp_hln = ETH_ALEN;
	arp_packet->arp_pln = IP_ADDR_LEN;
	arp_packet->arp_op = htons(ARPOP_REQUEST);
	memcpy(arp_packet->arp_sha, src_mac_addr, ETH_ALEN);
	memcpy(arp_packet->arp_tha, dst_mac_addr, ETH_ALEN);
	memcpy(arp_packet->arp_spa, &src_in_addr, IP_ADDR_LEN);
	memcpy(arp_packet->arp_tpa, &dst_in_addr, IP_ADDR_LEN);

	return arp_packet;
}

/* arp请求 */
void arp_request(const char *if_name, const char *arp_src_ip, const char *arp_src_mac, const char *dst_ip)
{
	struct sockaddr_ll saddr_ll;
	struct ether_header *eth_header;
	struct ether_arp *arp_packet;
	struct ifreq ifr;
	char buf[ETHER_ARP_PACKET_LEN];
	unsigned char src_mac_addr[ETH_ALEN];
	unsigned char dst_mac_addr[ETH_ALEN] = BROADCAST_ADDR;
	char *src_ip;
	int sock_raw_fd, ret_len, i;

	if ((sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1)
		err_exit("socket()");

	bzero(&saddr_ll, sizeof(struct sockaddr_ll));
	bzero(&ifr, sizeof(struct ifreq));
	/* 网卡接口名 */
	memcpy(ifr.ifr_name, if_name, strlen(if_name));

	/* 获取网卡接口索引 */
	if (ioctl(sock_raw_fd, SIOCGIFINDEX, &ifr) == -1)
		err_exit("ioctl() get ifindex");
	saddr_ll.sll_ifindex = ifr.ifr_ifindex;
	saddr_ll.sll_family = PF_PACKET;

	/* 获取网卡接口IP */
	if (ioctl(sock_raw_fd, SIOCGIFADDR, &ifr) == -1)
		err_exit("ioctl() get ip");
	src_ip = inet_ntoa(((struct sockaddr_in *) & (ifr.ifr_addr))->sin_addr);
	printf("local ip:%s\n", src_ip);

	/* 获取网卡接口MAC地址 */
	if (ioctl(sock_raw_fd, SIOCGIFHWADDR, &ifr))
		err_exit("ioctl() get mac");
	memcpy(src_mac_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);

	printf("local mac");
	for (i = 0; i < ETH_ALEN; i++)
		printf(":%02x", src_mac_addr[i]);
	printf("\n");

	bzero(buf, ETHER_ARP_PACKET_LEN);
	/* 填充以太首部 */
	eth_header = (struct ether_header *)buf;
	memcpy(eth_header->ether_shost, src_mac_addr, ETH_ALEN);
	memcpy(eth_header->ether_dhost, dst_mac_addr, ETH_ALEN);
	eth_header->ether_type = htons(ETHERTYPE_ARP);
	/* arp包 */

	if (arp_src_mac != NULL)/*spoofed arpMAC*/
	{
		sscanf(arp_src_mac, "%02x:%02x:%02x:%02x:%02x:%02x", &src_mac_addr[0], &src_mac_addr[1], &src_mac_addr[2], &src_mac_addr[3], &src_mac_addr[4], &src_mac_addr[5]);
	}
	if (arp_src_ip != NULL)/*spoofed arpIP*/
	{
		printf("src_ip:%s\n", src_ip);
		strcpy(src_ip, arp_src_ip);
		printf("src_ip:%s\n", src_ip);
	}
	//src_mac_addr[0] = 1;/*spoofed detected 1 for srcmac!=ethmac*/
	//src_ip[strlen(src_ip) - 1] = '9';/*srcmac!=srcip*/ printf("local ip:%s\n", src_ip);
	arp_packet = fill_arp_packet(src_mac_addr, src_ip, dst_ip);
	memcpy(buf + ETHER_HEADER_LEN, arp_packet, ETHER_ARP_LEN);

	/* 发送请求 */
	ret_len = sendto(sock_raw_fd, buf, ETHER_ARP_PACKET_LEN, 0, (struct sockaddr *)&saddr_ll, sizeof(struct sockaddr_ll));
	if ( ret_len > 0)
		printf("sendto() ok!!!\n");

	close(sock_raw_fd);
}

int main(int argc, const char *argv[])
{
	const char* ethdevide = NULL, * arpsrcip = NULL, * arpdstip = NULL, * arpsrcmac = NULL;

	if (argc >= 5)
	{
		ethdevide = argv[2];
		if (!strcmp(argv[1], "spoofMAC"))
			arpsrcmac = argv[3];
		else if (!strcmp(argv[1], "spoofIP"))
			arpsrcip = argv[3];
		else
			goto error_format;
		arpdstip = argv[4];
	}
	else
	{
error_format:
		printf("Useage: arp.exe EthDevice srcIP srcMAC, dstIP\n");
		printf("Useage:\nfor spoofing ARPMAC: ./arp.exe spoofMAC Ethdevice arpMAC dstIP\n");
		printf("for spoofing ARPIP: ./arp.exe spoofIP Ethdevice arpIP dstIP\n");
		printf("the default is: h1 ping h2\n");
		ethdevide = "h1-eth0";
		/*
		arpsrcip = "10.0.0.1";
		arpsrcmac = "00:00:00:00:01";*/
		arpdstip = "10.0.0.2";
	}
	arp_request(ethdevide, arpsrcip, arpsrcmac, arpdstip);
	return 0;
}
