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
<<<<<<< HEAD
        f.write('domain,ipv4,v4Connect,ipv6,v6connet,v4html,v6html,differents\n')
=======
        f.write('domain,ipv4,v4Ping,ipv6,v6Ping,v4html,v6html,diffServe\n')
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc
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
<<<<<<< HEAD
        urldomain=domains[index]
=======
        urldomain = domains[index]
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc
        rtev4 = my_query(domains[index], 'A')  # query IPV4
        rtev6 = my_query(domains[index], 'AAAA')  # query IPV6
        if rtev4 is None:
            domains[index] = domains[index] + ', , '
        else:
            domains[index] = domains[index] + ',' + str(rtev4)
            ###check the conncetion time
<<<<<<< HEAD
            retval=ping_test_linux(addr=urldomain,type='v4')
            if retval is not None:
                domains[index]+=','+retval
            else:
                domains[index]+=',NA'
=======
            retval = ping_test_linux(addr=urldomain, type='v4')
            if retval is not None:
                domains[index] += ',' + retval
            else:
                domains[index] += ',NA'
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc
        if rtev6 is None:
            domains[index] = domains[index] + ', , '
        else:
            domains[index] = domains[index] + ',' + str(rtev6)
<<<<<<< HEAD
            retval=ping_test_linux(addr=urldomain,type='v6')
            if retval is not None:
                domains[index]+=','+retval
            else:
                domains[index]+=',NA'
            # print(index,domains[index])
            # time.sleep(0.001);
        add_compare=''
        if rtev4 is not None:
            v4_html,v4time=fetch_html(domain,'4')
        if rtev6 is not None:
            v6_html,v6time=fetch_html(domain,'6')
        print(domains[index])
=======
            retval = ping_test_linux(addr=urldomain, type='v6')
            if retval is not None:
                domains[index] += ',' + retval
            else:
                domains[index] += ',NA'
                # print(index,domains[index])
                # time.sleep(0.001);
        # add_compare=''
        domain_add = ['NA', 'NA', 'NA']
        if rtev4 is not None:
            v4_html, v4time = fetch_html(urldomain, '4')
            #print('v4html %s\n' % v4_html)
            if v4_html is not None and v4_html != b'':
                domain_add[0] = str(v4time) + ' ms'
        if rtev6 is not None:
            v6_html, v6time = fetch_html(urldomain, '6')
            #print('v6html %s\n' % v6_html)
            if v6_html is not None and v6_html != b'':
                domain_add[1] = str(v6time) + 'ms'
        if (rtev4 is not None) and (rtev6 is not None):
            if (v4_html is not None) and (v6_html is not None):
                if v4_html != b'' and v6_html != b'':
                    if v4_html == v6_html:
                        domain_add[2] = 'No diff'
                    else:
                        domain_add[2] = 'Some diff'
                else:
                    domain_add[2] = 'fetch_html_failed'
        #print(','.join(domain_add))

        #print(domains[index]+','+','.join(domain_add))
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc
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
            # print(retval.geturl())
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
    pass


def main():
    time1 = datetime.now()
    load_domain('top-1m.csv')
    time2 = datetime.now()

    thread_id = []
    N = len(domains) // 1000
    print('loading %d domain from top-1m.csv using %f Seconds;\
    \n now testing dns using %d threads!' % (len(domains), (time2 - time1).total_seconds(), N))

    for index in range(0, len(domains) // 100000):
        thread_id.append(threading.Thread(target=query_domain,
                                          args=(index * 100000, (index + 1) * 100000)))
    for index in range(0, len(domains) // 100000):
        thread_id[index].start()
    for index in range(0, len(domains) // 100000):
        thread_id[index].join()
    time1 = datetime.now()
    print('all finished using %d Seconds' % (time1 - time2).total_seconds())
    save_result()
    pass


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

<<<<<<< HEAD
=======

>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc
# for windows system
def ping_test(addr=None, type=None, loops=None):
    if addr is None or type is None or loops is None:
        cmd = 'ping -4 www.baidu.com -n 1'
    else:
        cmd = 'ping -' + type + ' ' + addr + ' -n ' + loops
    rtt = r'[\d]+[\' \']*ms[\' \']*[\r\n]*'
    matcher = re.compile(rtt)
    args = shlex.split(cmd)
    try:
        p = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, shell=False)
        # p.stdin.write(cmd)
        # p.stdin.write(cmd.encode('utf-8'))
        out = p.stdout.read()
        print(out)
        print(out.decode('gbk'))
        groups = matcher.findall(out.decode('gbk'));
        for index in groups:
            print(index.strip())

    except Exception as e:
        print(e, 'error')

<<<<<<< HEAD
#for linux system
def ping_test_linux(addr=None, type=None, loops=1):
    cmd =''
    if type =='v4':
        cmd+='ping '
    else :
        cmd+='ping6 '
    if addr is not None:
        cmd += addr+' ';
    else:
        cmd +='www.baidu.com '
    if loops >=1:
        cmd += '-c '+str(loops)
=======

# for linux system
def ping_test_linux(addr=None, type=None, loops=1):
    cmd = ''
    if type == 'v4':
        cmd += 'ping '
    else:
        cmd += 'ping6 '
    if addr is not None:
        cmd += addr + ' ';
    else:
        cmd += 'www.baidu.com '
    if loops >= 1:
        cmd += '-c ' + str(loops)
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc

    rtt = r'([\d]+[\.]{0,1}[\d]+[\' \']*ms)[\' \']*[\r\n]*'
    matcher = re.compile(rtt)
    args = shlex.split(cmd)
    try:
        p = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, shell=False)
        # p.stdin.write(cmd)
        # p.stdin.write(cmd.encode('utf-8'))
        out = p.stdout.read()
<<<<<<< HEAD
        #print(out)
        #print(out.decode('gbk'))
        groups = matcher.findall(out.decode('utf-8'));
        if groups is not None and len(groups) !=0:
            return groups[0]
        return
#        for index in groups:
#            print(index.strip())
=======
        # print(out)
        # print(out.decode('gbk'))
        groups = matcher.findall(out.decode('utf-8'));
        if groups is not None and len(groups) != 0:
            return groups[0]
        return
    # for index in groups:
    #            print(index.strip())
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc
    except Exception as e:
        print(e, 'error')


def fetch_html(domain=None, type=None):
    if domain is None or type is None:
        cmd = 'curl --connect-timeout 10 -m 20 -4 -L www.baidu.com'
    else:
        cmd = 'curl --connect-timeout 10 -m 20 -' + type + ' -L ' + domain
    args = shlex.split(cmd)
<<<<<<< HEAD
    deltaTime=0
    #print(cmd)
=======
    out=b''
    deltaTime = 0
    # print(cmd)
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc
    try:
        startTime = datetime.now().microsecond;
        p = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, shell=False)
<<<<<<< HEAD
        
        endTime = datetime.now().microsecond;
        out = p.stdout.read()
        deltaTime=endTime-startTime
        #err=p.stderr.read()
        try:
            #print(out)
            #print(out.decode('utf-8'))
            #print(err)
            #print(err.decode('utf-8'))
            return out.decode('utf-8'),deltaTime
        except Exception as e:
            #print(e,'error in decode')
            return out,deltaTime
=======

        endTime = datetime.now().microsecond;
        out = p.stdout.read()
        deltaTime = endTime - startTime
        #return out, deltaTime
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc
    except Exception as e:
        print(e, 'error')
        #return b'', 0
    finally:
        return out,deltaTime


def diff_compare(domain=None):
    if domain is None:
        return 'domain is empty'
<<<<<<< HEAD
    v4_html,v4time=fetch_html(domain,'4')
    v6_html,v6time=fetch_html(domain,'6')
    #print(v4time,v6time)
    if v6_html is None or v4_html is None:
        #print('can not fetch pages')
        return v4time,v6time,'can\'t fetch htmls'
    if len(v6_html) and len(v4_html):
        if v4_html ==v6_html:
            #print('no difference')
            return v4time,v6time,'no difference'
        else:
            #print('some differences')
            return v4time,v6time,'some differences'
    else:
        #print('can\'t fetch htmls')
        return v4time,v6time,'can\'t fetch htmls'
=======
    v4_html, v4time = fetch_html(domain, '4')
    v6_html, v6time = fetch_html(domain, '6')
    # print(v4time,v6time)
    if v6_html is None or v4_html is None:
        # print('can not fetch pages')
        return v4time, v6time, 'can\'t fetch htmls'
    print('v4html ',v4_html)
    print('v6html ',v6_html)
    if len(v6_html) and len(v4_html):
        if v4_html == v6_html:
            # print('no difference')
            return v4time, v6time, 'no difference'
        else:
            # print('some differences')
            return v4time, v6time, 'some differences'
    else:
        # print('can\'t fetch htmls')
        return v4time, v6time, 'can\'t fetch htmls'

>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc

# 2402:f000:1:404:166:111:4:100
# 166.111.4.100
if __name__ == '__main__':
<<<<<<< HEAD
    v4time,v6time,differ=diff_compare('www.baidu.com')
    retval4=ping_test_linux(addr='baidu.com',type='v4')
    retval6=ping_test_linux(addr='google.com',type='v6')
    print(v4time,v6time,differ)
    print('ipv4',retval4)
    print('ipv6',retval6)
    #request_url('172.217.5.78', '2607:f8b0:4007:80c::200e')
    # request_url('166.111.4.100', '2402:f000:1:404:166:111:4:100')
    # save_result()
    #main()
=======
    '''
    v4time, v6time, differ = diff_compare('jawalplus.com')
    retval4 = ping_test_linux(addr='baidu.com', type='v4')
    retval6 = ping_test_linux(addr='google.com', type='v6')
    print(v4time, v6time, differ)
    print('ipv4', retval4)
    print('ipv6', retval6)
    '''
    # request_url('172.217.5.78', '2607:f8b0:4007:80c::200e')
    # request_url('166.111.4.100', '2402:f000:1:404:166:111:4:100')
    # save_result()
    main()
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc
    # get_addr('top-1m.csv');

    #    main()
