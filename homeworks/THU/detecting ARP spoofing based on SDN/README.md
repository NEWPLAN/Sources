# <p align="center">detecting ARP spoofing based on SDN</p>
====================
<h2>Introduction</h2>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;This is a simple tool to prevent ARP-spoofing attack based on SDN, there are two part insides: control plane and data plane, besides, we create some useful tool to construct arp packet, they are described as follow in detail.</p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;the <b>ProxyArp.java</b> in proxyarp is the detection module, it should be add in control plane.</p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;the <b>topol.py</b> in mininet is the simulation module, it should be add in data plane.</p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;the <b>arping.py</b> in mininet is a basic tool to construct a arp.request packet and send it out.</p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;the <b>reply.py</b> in mininet is a basic tool to construct a arp.reply packet and send it out.</p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;the <b>run.sh</b> in mininet is a script file, it describes how to using our tools, including how to compile our tools and how to construct packet in different scenarios.</p>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;This work is based on the open project <a href="https://github.com/wuyouke/Preventing-ARP-Attacks-in-SDN">Preventing-ARP-Attacks-in-SDN</a> and <a href="https://github.com/somiltg/LoadbalancerProxyArp">LoadbalancerProxyArp</a>.</p>

<h2>Required modules</h2>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;There are two main components required, the first one is mininet (This can be describe as a data-plane, a topology or simulator of a real networks, including switchs, hosts, links and etc., it is based on python so you are supposed to check your python tools work well.); the second one is floodlight (It is a controller for SDN, the control-plane, linking to data-plane, it can be used to capture the status of data-plane by communicating with them. It is based on java, so, you are supposed to make sure the java eniromnent are deployed well on your computer.).</p>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;Besides, we just installed the latest versdion of mininet and floodlight, during our experiment, there are several fatal error confusing us a lot, in general, we think that:</br><b>python (version) >= 3.4</b>;</br><b>java  (version)  >= 1.8</b> </br>are required.</p>

<h2>How to play...</h2>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;There are several steps in details to help you make these codes run correctly.</p>
<h5>step 1:</h5>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;Adding ProxyArp as floodlightcontroller.proxyarp inside path: <b>src/main/java</b>, this is easy to be done, as you can just copy the packet(<b>proxyarp</b>) into the mentioned path before. Besides, if you want to add it in an visual environment like eclipse(do not forget adjust the java system), you can just create a new package named <b>'floodlightcontroller.proxyarp'</b>, then creat a new java class named 'ProxyArp' and copy the src in packet(proxyarp) to it.</p>
<h5>step 2:</h5>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;After step 1, you are supposed to have already added the src correctly, then, you should regist the module into file ( path : <b>src/main/resources/META-INF/services/net.floodlightcontroller.core.module.IFloodlightModule</b>) by adding the string : <b>net.floodlightcontroller.proxyarp.ProxyArp</b> to that (appending it at the end of the file is ok).</p>
<h5>step 3:</h5>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;After step 2, you should regsit for another thing. regist the <b>net.floodlightcontroller.proxyarp.ProxyArp</b> module in <b>/src/main/resources/floodlightdefault.properties</b> (by adding it at the end of module variable(named <b>floodlight.modules</b>), it necessary to the exactly position to make it run correctly).</p>
<h5>step 4:</h5>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;After the above-mentioned steps, you are supposed to install it well, so you can run it then. you should first start floodlight (<b>cd floodlight && ant && java -jar target/floodlight.jar</b>) and mininet (<b>cd mininet && sudo python topol.py</b>), then you can using commands (<b>in run.sh</b>) to test.</p>

<h2>Future work</h2>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;During the whole experiment, there is a strong assumption: we trust in the controller absolutely, this is not reasonable and weak, we have found that, the controller has a naive learning ability, which leads to fatal vulnerabilities, causing the controller is so naive that it cannot judge if the pair of MAC and ip is correct, then it just add this pair into their recording. this is challengeable and should be taken into consideration.</p>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;Besides, how to deal with moveable situation is another problem and how to deal with IPV6 is another question need to de done in further work.</p>

<p align="right">NEWPLAN</p>
