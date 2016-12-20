# <p align="center">detecting ARP spoofing based on SDN</p>
====================

SDN Floodlight controller modules for balancing load and address resolution within a subnet

Instructions for using it

Download floodlight module 1.0.
Add the two modules i.e. ServerLoadBalance and ProxyArp as net.floodlightcontroller.serverloadbalance and net.floodlightcontroller.proxyarp inside src/main/java
Register the modules by adding the following entries in the file with path as src/main/resources/META-INF/services/net.floodlightcontroller.core.module.IFloodlightModule- net.floodlightcontroller.proxyarp.ProxyArp net.floodlightcontroller.serverloadbalance.AdvancedLoadBalancer
In /src/main/resources/floodlightdefault.properties, add net.floodlightcontroller.proxyarp.ProxyArp and net.floodlightcontroller.serverloadbalance.AdvancedLoadBalancer to floodlight.modules variable Module is ready to run

In mininet OS, execute topo1.py to create sample topology. However one can also use mn to create topology.

NOTE: to change vip or set virtual proxy address and virtual macs find setServerandVIP() in AdvancedLoadbalancer and setVirtualDevices() in proxyarp module.