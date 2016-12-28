# this is makefile for arp test tools, including constructing a arp.request(arp.exe) packet and arp.reply packet(reply.exe)
all:
	@echo 'choosing what you want to do:';
	@echo 'h1 ping h2';
	@echo 'spoofed MAC test';
	@echo 'spoofed IP test';
	@echo 'spoofed arp.reply test';
	
ip:arp.exe
	@echo 'using h3 send arp to h5 with spoofed ip(from h4)'
	h3 ./arp.exe spoofIP h3-eth0 10.0.0.4 10.0.0.5

mac:arp.exe
	@echo 'using h33 send arp to h2 with spoofed mac(from h1)'
	h33 ./arp.exe spoofMAC h33-eth0 00:00:00:00:00:01 10.0.0.2

arptest:arp.exe
	@echo 'h1 ping h2'
	h1 ./arp.exe spoofMAC h1-eth0 00:00:00:00:00:01 10.0.0.2
	
replytest:reply.exe
	h1 ./reply.exe spoofMAC h1-eth0 00:00:00:00:00:01 10.0.0.2
	
arp.exe:
	gcc arping.c -o arp.exe

reply.exe:
	gcc reply.c -o reply.exe
	
clean:
	@rm arp.exe reply.exe;
