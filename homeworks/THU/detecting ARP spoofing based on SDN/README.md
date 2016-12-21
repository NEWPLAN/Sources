# <p align="center">detecting ARP spoofing based on SDN</p>
====================
<h2>Introduction</h2>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;This is a simple tool to prevent ARP-spoofing attack based on SDN, there are two part insides.</p>
<p>the 'ProxyArp.java' in proxyarp is the detection module, it should be add in control plane.</p>
<p>the 'topol.py' in mininet is the simulation module, it should be add in data plane.</p>
<p>the 'arping.py' in mininet is a basic tool to construct a arp.request packet and send it out.</p>
<p>the 'reply.py' in mininet is a basic tool to construct a arp.reply packet and send it out.</p>
<p>the 'run.sh' in mininet is a script file, it describes how to using our tools, including how to compile our tools and how to construct packet in different scenarios.</p>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;This work is based on the open project <h3>Preventing-ARP-Attacks-in-SDN<href:'https://github.com/wuyouke/Preventing-ARP-Attacks-in-SDN'></h3> and <h3>LoadbalancerProxyArp<href:'https://github.com/somiltg/LoadbalancerProxyArp'></h3>. They are deserved our respect.</p>

Instructions for using it
<h2>Required modules</h2>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;There are two main components required, the fist one is mininet<This can be describe as a data-plane, a topology or simulator of a real networks, including switchs, hosts, links and etc., it's based on python so you are supposed to check your python tools work well.>, and the second one is floodlight<It's a controller for SDN, the control-plane, linking to data-plane, it can be used to capture the status of data-plane by communicating with them. It's based on java, so, you are supposed to make sure the java eniromnent are deployed well on your computer.>.</p>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;Besides, we just installed the latest versdion of mininet and floodlight, during our experiment, there are several fatal error confusing us a lot, in general, we suggest that:python >=3.4, java>=1.8 are required.</p>
<h2>how to play...</h2>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;There are several steps in details to help you make these codes run correctly.</p>
<h3>step 1:</h3>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;Adding ProxyArp as floodlightcontroller.proxyarp inside path:<b>src/main/java</b>, this is easy to be done, as you can just copy the packet(proxyarp) into the mentioned path before. Besides, if you want to add it in an visual environment like eclipse(don't forget adjust the java system), you can just create a new package named <b>'floodlightcontroller.proxyarp'</b>, then creat a new java class named 'ProxyArp' and copy the src in packet(proxyarp) to it.</p>
<h3>step 2:</h3>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;After step 1, you are supposed to add the src correctly, then, you should regist the module into file<path: 'src/main/resources/META-INF/services/net.floodlightcontroller.core.module.IFloodlightModule'> by adding 'net.floodlightcontroller.proxyarp.ProxyArp' to that, (appending it at the end of the file is ok).</p>
<h3>step 3:</h3>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;After step 2, you should regsit for another thing. regist the 'net.floodlightcontroller.proxyarp.ProxyArp' module in '/src/main/resources/floodlightdefault.properties'(by adding it at the end of module variable(named floodlight.modules),it necessary to the exactly position to make it run correctly).</p>
<h3>step 4:</h3>
<p align="justify">&nbsp;&nbsp;&nbsp;&nbsp;After the above-mentioned steps, you are supposed to install it well, so you can run it then.</p>

<h1 align="center"> Hope u enjoy it</h1>

<p align="right">NEWPLAN</p>