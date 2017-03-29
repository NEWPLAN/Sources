import os;
global countccc

def generate(n, A):
    if n == 1:
        global countccc
        countccc += 1
        print(A)
    else:
        for i in range(0,n-1):
            generate(n - 1, A)
            if n%2==0:
                A[i],A[n-1]=A[n-1],A[i]
            else:
                A[0], A[n - 1] = A[n - 1], A[0]
        generate(n - 1, A)


def generate2(n, A):
    c=[];
    for i in range(0,n):
        c.append(0)

    global countccc
    countccc+=1
    print(A)
    
    i=0;
    while i < n:
        if  c[i] < i:
            if i%2==0:
                A[0], A[i] = A[i], A[0]
            else:
                A[c[i]], A[i] = A[i], A[c[i]]
            countccc += 1
            print(A)
            c[i] += 1
            i=0
        else:
            c[i] = 0
            i += 1


if __name__=='__main__':
    countccc = 0
 #   generate(4,[1,2,3,4])
    print(countccc)
    generate2(5, [1, 2, 3, 4,5])
    print(countccc)

