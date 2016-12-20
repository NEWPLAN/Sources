#!/usr/bin/python

"""
Script created by VND - Visual Network Description (SDN version)
"""
from mininet.net import Mininet
from mininet.node import Controller, RemoteController, OVSKernelSwitch, IVSSwitch, UserSwitch
from mininet.link import Link, TCLink
from mininet.cli import CLI
from mininet.log import setLogLevel

def topology():

    "Create a network."
    net = Mininet( controller=RemoteController, link=TCLink, switch=OVSKernelSwitch )

    print "*** Creating nodes"
    h1 = net.addHost( 'h1', mac='00:00:00:00:00:01', ip='10.0.0.1/8' )
    h2 = net.addHost( 'h2', mac='00:00:00:00:00:02', ip='10.0.0.2/8' )
    h3 = net.addHost( 'h3', mac='00:00:00:00:00:03', ip='10.0.0.3/8' )
    h4 = net.addHost( 'h4', mac='00:00:00:00:00:04', ip='10.0.0.4/8' )
    h5 = net.addHost( 'h5', mac='00:00:00:00:00:05', ip='10.0.0.5/8' )
    h6 = net.addHost( 'h6', mac='00:00:00:00:00:06', ip='10.0.0.6/8' )
    h7 = net.addHost( 'h7', mac='00:00:00:00:00:07', ip='10.0.0.7/8' )
    c8 = net.addController( 'c8',ip='127.0.0.1',port=6653 )
    # c8 = net.addController( 'c8',ip='192.168.56.1',port=6653 )
    s9 = net.addSwitch( 's9', listenPort=6673, mac='00:00:00:00:00:09' )
    s10 = net.addSwitch( 's10', listenPort=6674, mac='00:00:00:00:00:10' )
    s11 = net.addSwitch( 's11', listenPort=6675, mac='00:00:00:00:00:11' )
    s12 = net.addSwitch( 's12', listenPort=6676, mac='00:00:00:00:00:12' )
    s13 = net.addSwitch( 's13', listenPort=6677, mac='00:00:00:00:00:13' )
    s14 = net.addSwitch( 's14', listenPort=6678, mac='00:00:00:00:00:14' )
    h33 = net.addHost( 'h33', mac='00:00:00:00:00:33', ip='10.0.0.33/8' )
    h40 = net.addHost( 'h40', mac='00:00:00:00:00:40', ip='10.0.0.40/8' )
    h42 = net.addHost( 'h42', mac='00:00:00:00:00:42', ip='10.0.0.42/8' )
    h48 = net.addHost( 'h48', mac='00:00:00:00:00:48', ip='10.0.0.48/8' )

    print "*** Creating links"
    net.addLink(s14, h48)
    net.addLink(s14, s10)
    net.addLink(s14, h42)
    net.addLink(h42, s9)
    net.addLink(h40, s10)
    net.addLink(h33, s10)
    net.addLink(s14, s11)
    net.addLink(s9, s10)
    net.addLink(s9, s14)
    net.addLink(s10, s13)
    net.addLink(s9, s12)
    net.addLink(s11, s13)
    net.addLink(s12, s11)
    net.addLink(s13, h6)
    net.addLink(h7, s13)
    net.addLink(h3, s13)
    net.addLink(s11, h4)
    net.addLink(s11, h5)
    net.addLink(s12, h2)
    net.addLink(s12, h1)

    print "*** Starting network"
    net.build()
    c8.start()
    s11.start( [c8] )
    s13.start( [c8] )
    s12.start( [c8] )
    s10.start( [c8] )
    s13.start( [c8] )
    s9.start( [c8] )
    s14.start( [c8] )

    hosts=net.hosts
    hosts[0].cmd('arping -c 1 -A -I h1-eth0 10.0.0.1')
    hosts[1].cmd('arping -c 1 -A -I h2-eth0 10.0.0.2')
    hosts[2].cmd('arping -c 1 -A -I h3-eth0 10.0.0.3')
    hosts[3].cmd('arping -c 1 -A -I h4-eth0 10.0.0.4')
    hosts[4].cmd('arping -c 1 -A -I h5-eth0 10.0.0.5')
    hosts[5].cmd('arping -c 1 -A -I h6-eth0 10.0.0.6')
    hosts[6].cmd('arping -c 1 -A -I h7-eth0 10.0.0.7')
    hosts[7].cmd('arping -c 1 -A -I h33-eth0 10.0.0.33')
    hosts[8].cmd('arping -c 1 -A -I h40-eth0 10.0.0.40')
    hosts[9].cmd('arping -c 1 -A -I h42-eth0 10.0.0.42')
    hosts[10].cmd('arping -c 1 -A -I h48-eth0 10.0.0.48')
    print "*** Running CLI"
    CLI( net )

    print "*** Stopping network"
    net.stop()

if __name__ == '__main__':
    setLogLevel( 'info' )
    topology()
