#!/usr/bin/env python


import dns.resolver, time;
import threading;
import urllib.request
# import datetime;
from datetime import datetime

domains = []


def my_query(addr, iptype):
    try:
        answer = dns.resolver.query(addr, iptype);
        # print(answer[0])
        return answer[0];
    except dns.resolver.NoAnswer:
        # print("No " + iptype)
        return str('NO' + iptype);
    except dns.resolver.NXDOMAIN:
        # print("No such domain")
        return 'NODOMAIN';
    except dns.resolver.NoNameservers:
        # print('No answer');
        return 'NOANSWER';
    except dns.resolver.YXDOMAIN:
        # print('domain is too long');
        return 'TOOLONG';
    except dns.exception.Timeout:
        # print('time out');
        return 'TIMEOUT';
    except KeyboardInterrupt:
        print("\nGoodbye!")
    except  Exception as e:
        print('unknown exception ', e, addr, iptype)
        return None;


# domains=['abs','eee','rrr']

def save_result(filename=None):
    if filename is None:
        filename = str(datetime.now()).replace(' ', '_').replace(':', '_') + '.csv'
    counts = 1
    with open(filename, 'w') as f:
        for index in domains:
            f.write(str(counts) + ',' + str(index) + '\n')
            counts += 1
    pass;


def load_domain(filename=None):
    if filename is None:
        print('file can not be empty!')
        exit(-1)
    global domains
    try:
        with open(filename, 'r') as f:
            for line in f:
                index, addr = line.strip('\r\n').split(',')
                # print('%s\t%s' % (index, r'www.' + addr))
                domains.append(addr)
        print(len(domains))
        return
    except:
        pass;
    pass;


def query_domain(start, end):
    # print(start,end)
    global domains
    for index in range(start, end):
        rtev4 = my_query(domains[index], 'A')#query IPV4
        rtev6 = my_query(domains[index], 'AAAA')#query IPV6
        if rtev4 is None:
            domains[index] = domains[index] + ', '
        else:
            domains[index] = domains[index] + ',' + str(rtev4)
        if rtev6 is None:
            domains[index] = domains[index] + ', '
        else:
            domains[index] = domains[index] + ',' + str(rtev6)
            # print(index,domains[index])
            # time.sleep(0.001);

    return


# 2402:f000:1:404:166:111:4:100
# 166.111.4.100

def request_url(ipv4=None, ipv6=None, loops=3):
    if ipv4 is None or ipv6 is None:
        return None
        pass
    interval = 0
    html4 = None
    html6 = None
    for i in range(loops):
        try:
            startTime = datetime.now().microsecond
            retval = urllib.request.urlopen('http://' + ipv4, timeout=10)
            #print(retval.geturl())
            html4 = retval.read()
            print('from v4', html4)
            endTime = datetime.now().microsecond
            interval = endTime - startTime
        except Exception as e:
            print(e, 'error in request url : ', ipv4)
            pass;
    for i in range(loops):
        try:
            startTime = datetime.now().microsecond
            retval = urllib.request.urlopen('http://[' + ipv6 + ']', timeout=10)
            # print(retval.info())
            # print(retval.geturl())
            html6 = retval.read()
            print('from v6', html6)
            endTime = datetime.now().microsecond
            interval = endTime - startTime
        except Exception as e:
            print(e, 'error in request url : ', ipv6)
            pass;
    if html4 is None or html6 is None:
        return None
    # elif html6 is None:
    #     return None;
    else:
        if html4 == html6:
            print('no different from v4 VS v6')
        else:
            print('some different in v4 VS v6')
        print(html4)
        print(html6)

    pass;


def main():

    time1=datetime.now()
    load_domain('top-1m.csv')
    time2=datetime.now()

    thread_id = []
    N = len(domains) // 1000
    print('loading %d domain from top-1m.csv using %f Seconds;\
    \n now testing dns using %d threads!' % (len(domains),(time2-time1).total_seconds(), N))

    for index in range(0, len(domains) // 1000):
        thread_id.append(threading.Thread(target=query_domain,
                                          args=(index * 1000, (index + 1) * 1000)))
    for index in range(0, len(domains) // 1000):
        thread_id[index].start()
    for index in range(0, len(domains) // 1000):
        thread_id[index].join()
    time1=datetime.now()
    print('all finished using %d Seconds'%(time1-time2).total_seconds())
    save_result()
    pass;


def Process(url, n):
    minSpan = 10.0
    maxSpan = 0.0
    sumSpan = 0.0
    over1s = 0
    for i in range(n):
        startTime = datetime.now().microsecond;
        try:
            res = urllib.request.urlopen('http://' + url, timeout=10)
            html = res.read();
            print(html)
        except:
            pass
        endTime = datetime.now().microsecond;
        span = (endTime - startTime)
        sumSpan = sumSpan + span
        if span < minSpan:
            minSpan = span
        if span > maxSpan:
            maxSpan = span
        # 超过一秒的
        if span > 1:
            over1s = over1s + 1
        print(u'%s Spent :%s MS' % (url, span))
    print(
        u'requested:%s times,Total Spent:%s seconds,avg:%s seconds, max:%s seconds,min:%s seconds,over 1 secnod:%s times' % (
        n, sumSpan, sumSpan / n, maxSpan, minSpan, over1s))
    print('\n')

import subprocess
import shlex
import re

def ping_test(addr=None,type=None,loops=None):
    if addr is None or type is None or loops is None:
        cmd = 'ping -4 www.baidu.com -n 1'
    else:
        cmd = 'ping -' + type + ' ' + addr + ' -n ' + loops
    rtt=r'[\d]+[\' \']*ms[\' \']*[\r\n]*'
    matcher=re.compile(rtt)
    args=shlex.split(cmd)
    try:
        p=subprocess.Popen(args,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=False)
        #p.stdin.write(cmd)
        #p.stdin.write(cmd.encode('utf-8'))
        out =p.stdout.read()
        print(out)
        print(out.decode('gbk'))
        groups=matcher.findall(out.decode('gbk'));
        for index in groups:
            print(index.strip())

    except Exception as e:
        print(e,'error')


'''
import urllib.request

response = urllib.request.urlopen("http://220.181.57.217:80")
html = response.read()
print(html)
'''
'''
if __name__ == '__main__':
  #  Process('166.111.4.100', 3)
    Process('[2402:f000:1:404:166:111:4:100]', 3)
   # res = urllib.request.urlopen('http://119.75.218.70', timeout=10)
    #html = res.read();
    #print(html)

'''
# 2402:f000:1:404:166:111:4:100
# 166.111.4.100
if __name__ == '__main__':
    #ping_test()
    request_url('166.111.4.100', '2402:f000:1:404:166:111:4:100')
    # save_result()
    #main()
    # get_addr('top-1m.csv');

    #    main()
