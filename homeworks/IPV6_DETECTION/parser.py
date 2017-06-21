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
        for index in fileindex:
            res = index.readline()
            print(res)


    for index in fileindex:
        index.close()


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


if __name__ == "__main__":
    main()
    # loadfile('parser.py')
