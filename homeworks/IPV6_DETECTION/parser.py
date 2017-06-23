#!/usr/bin/env python
# coding=utf-8

import  os

allfile = '''
2017-04-21_23_24_37.195445.csv
2017-04-25_00_51_13.715353.csv
2017-04-28_03_57_11.302963.csv
2017-05-02_15_54_21.813382.csv
2017-05-06_18_03_48.876153.csv
2017-05-10_10_42_38.192904.csv
2017-05-14_17_47_19.873467.csv
2017-05-29_16_52_23.032645.csv
2017-06-03_12_46_44.714100.csv
2017-06-07_20_25_16.176063.csv
2017-06-12_08_54_40.849705.csv
2017-06-16_19_14_54.526799.csv
'''


def main():
    global allfile
    eachfile = allfile.strip('\n').split('\n')
    fileindex = []
    print(len(eachfile))
    for index in eachfile:
        #print(index)
        #dump_file(filename=index)
        fileindex.append(loadfile(index))
    # print fileindex

# 处理每个文件的头信息
    for index in fileindex:
        res = index.readline()
        print(res)

    while True:
        flags=[0 for i in range(len(fileindex))]
        for index,ii in zip(fileindex,range(len(fileindex))):
            res = index.readline()
            if res is '':
                print('end for file\n')
                flags[ii]=1
            else:
                print(res)
        if 0 not in flags:
            break
    for index in fileindex:
        index.close()

def count_one_record(abc=[]):
    if abc is None or abc is '' or abc is []:
        return  -1
    tables=['index','domain','ipv4 counts','ping ipv4','ipv6 counts','ping ipv6','v4 html','v6 html','service']
    record=[0 for i in range(len(tables)+1)]
    if '.' in abc[2]:# ipv4 addr
        record[2]+=1
    if ':' in abc[4]:# ipv6 addr
        record[6]+=1
    if 'diff' in abc[7]:
        record[7]+=1
        if 'some' in abc[7]:
            record[8]+=1

        






def dump_file(filename):
    if filename is None:
        print('file can not be empty')
        return -1
    try:
        f_descriptor=open(filename,'w')
        f_descriptor.write('''index,domain,ipv4,v4Ping,ipv6,v6Ping,v4html,v6html,diffServe
1,google.com,172.217.5.78,0.393 ms,2607:f8b0:4007:800::200e,0.786 ms,5218 ms,12389ms,Some diff
2,youtube.com,172.217.5.206,0.406 ms,2607:f8b0:4007:808::200e,0.862 ms,4761 ms,5580ms,Some diff
3,facebook.com,31.13.77.36,11.5 ms,2a03:2880:f122:83:face:b00c:0:25de,9.76 ms,4766 ms,5088ms,Some diff
4,baidu.com,180.149.132.47,193 ms,NOANSWER,NA,7632 ms,NA,NA
5,wikipedia.org,198.35.26.96,13.4 ms,2620:0:863:ed1a::1,9.63 ms,15049 ms,6138ms,No diff
6,yahoo.com,206.190.36.45,72.1 ms,2001:4998:44:204::a7,46.5 ms,7124 ms,4474ms,Some diff
7,google.co.in,172.217.5.195,0.382 ms,2607:f8b0:4007:80d::2003,0.759 ms,5719 ms,5224ms,Some diff
8,qq.com,61.135.157.156,160 ms,NOANSWER,NA,6303 ms,NA,NA
9,reddit.com,151.101.65.140,0.469 ms,NOANSWER,NA,13463 ms,NA,NA
''')
        f_descriptor.close()
    except Exception as e:
        print('create file error')
        print(e)




def loadfile(filename):
    if filename is None:
        print('file is empty');
        return -1
    try:
        f = open(filename, 'r')
        return f
    except Exception as e:
        print(e)
    return


def parser_onebyone():
    global allfile
    eachfile = allfile.strip('\n').split('\n')
    fileindex = []
    print(len(eachfile))
    for index in eachfile:
        has_v4_addr=0
        has_v6_addr=0

        connect_v4_addr=0
        connect_v6_addr=0
        connect_v4_fast=0
        connect_v6_fast=0

        fetch_v4_html=0
        fetch_v6_html=0
        fetch_v4_fast=0
        fetch_v6_fast=0

        differ_service=0
        same_service=0

        with open(index ,'r') as f:
            print('---------------')
            print(f.readline())
            print('first line')
            for eachline in f:
                item=eachline.split(',')
                tables = ['index', 'domain', 'v4_counts', 'ping_v4', 'v6_counts', 'ping_v6', 'v4_html',
                          'v6_html', 'service']

                #print(item)

                ping_v4=0
                ping_v6=0
                fetch_v4=0
                fetch_v6=0

                if '.' in item[tables.index('v4_counts')]:
                    #print('has ipv4')
                    has_v4_addr+=1
                else:
                    pass
                    #print('no ipv4')

                if ':' in item[tables.index('v6_counts')]:
                    #print('has ipv6')
                    has_v6_addr+=1
                else:
                    pass
                    #print('no ipv6')


                if 'ms' in item[tables.index('ping_v4')]:
                    #print('ping v4 success')
                    connect_v4_addr+=1
                    ping_v4=float(item[tables.index('ping_v4')].replace('ms','').replace(' ',''))
                else:
                    pass
                    #print('ping v4 failed')
                if 'ms' in item[tables.index('ping_v6')]:
                    connect_v6_addr+=1
                    ping_v6 = float(item[tables.index('ping_v6')].replace('ms', '').replace(' ', ''))
                    #print('ping v6 success')
                else:
                    pass
                    #print('ping v6 failed')

                if ping_v4 and ping_v6:
                    if ping_v4 < ping_v6:
                        connect_v4_fast+=1
                    else:
                        connect_v6_fast+=1

                if 'ms' in item[tables.index('v4_html')]:
                    fetch_v4 = float(item[tables.index('v4_html')].replace('ms', '').replace(' ', ''))
                    fetch_v4_html+=1
                    #print('fetch v4 html success')
                if 'ms' in item[tables.index('v6_html')]:
                    fetch_v6 = float(item[tables.index('v6_html')].replace('ms', '').replace(' ', ''))
                    fetch_v6_html+=1
                    #print('fetch v6 html success')

                if fetch_v4 and fetch_v6:
                    if fetch_v4 < fetch_v6:
                        fetch_v4_fast+=1
                    else:
                        fetch_v6_fast+=1

                if 'No' in item[tables.index('service')]:
                    #print('No differ')
                    same_service+=1
                elif 'Some' in item[tables.index('service')]:
                    #print('some differ')
                    differ_service+=1
                else:
                    pass
                    #print('No answer')
            print("has v4 addr: %d\nhas v6_addr: %d\nconnect v4 success: %d\nconnect v6 success: %d\n"
                  "v4 faster in connect: %d\nv6 faster in connect: %d\nfetch v4 html success: %d\n"
                  "fetch v6 html success: %d\nv 4faster in fetch html: %d\nv6 faster in fetch html: %d\n"
                  "differ in service: %d\nno differ in service: %d"
                  %(has_v4_addr,has_v6_addr,
                   connect_v4_addr,connect_v6_addr,connect_v4_fast,connect_v6_fast,
                   fetch_v4_html,fetch_v6_html,fetch_v4_fast,fetch_v6_fast,
                   differ_service,same_service))



if __name__ == "__main__":
    parser_onebyone()
    #main()
    # loadfile('parser.py')
