# =======================================================================
# Author: Isai Damier
# Title: Permutation
# Project: geekviewpoint
# Package: algorithms
#
# Statement:
#   Returns a list of all the permutations of the given set
#
# Sample Input: [1,2,3]
# Sample Output: [1,2,3]; [1,3,2]; [2,1,3]; [2,3,1]; [3,1,2]; [3,2,1]
#
# Description:
#
# Technical Details:
#   One way of verifying the correctness of the result is to count the
#   number of permutations returned. From arithmetics, we know that the
#   number of permutations for a set is equal to the factorial of the
#   size of the set: 3! = 6.
#
#   This recursive algorithm is usually referred to as Heap's
#   permutation, in honor of B. R. Heap.
#
#   If a programmer simply wishes to print the permutations instead of
#   returning them in a list, [code 2] should replace [code 1] below.
#
#   [code 1]:
#     if(1 == n)
#       factorials.add(Arrays.copyOf(list, list.length));
#
#   [code 2]:
#    if 1 == n:
#     for i in aList
#      print("%s, ") % i
#
# =======================================================================
def permutation(aList):
    factorials = []
    _permutation(aList, len(aList), factorials)
    return factorials


def _permutation(aList, n, factorials):
    if 1 == n:
        factorials.append(list(aList))
    else:
        for i in range(n):
            _permutation(aList, n - 1, factorials)
            if 0 == n % 2:
                swap(aList, 0, n - 1)
            else:
                swap(aList, i, n - 1)


def swap(aList, x, y):
    t = aList[x]
    aList[x] = aList[y]
    aList[y] = t

if __name__=='__main__':
	rteval=permutation([1,2,3,4,5,6,7,8])
	for i in rteval:
		print(i)
	print(len(rteval))
