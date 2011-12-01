#coding: utf-8
from numpy import *

MAXLEN = 7
MASK = 59049 # 3^10
dp = zeros([MAXLEN, MASK, 10, 10], int32)
u = zeros([MAXLEN, MASK, 10, 10], bool)
three = array([3 ** i for i in xrange(10)])
n = MAXLEN

def go(len, mask, last, prelast):
    if len == n:
        return 1
    if u[len][mask][last][prelast] == True:
        return dp[len][mask][last][prelast]
    u[len][mask][last][prelast] = 1

    res = dp[len][mask][last][prelast]
    cnt = zeros(10, int32)

    cmask = mask
    i = 0
    while i < 10:
        cnt[i] = cmask % 3
        cmask /= 3
        i += 1

    i = 0
    while i < 10:
        # if i was used less than 2 times
        # previous wasn't i
        # and before it wasn't i
        if cnt[i] != 2 and (len < 1 or last != i) and (len < 2 or prelast != i):
            cmask = mask + three[i]
            res += go(len + 1, cmask, i, last)
        i += 1
    dp[len][mask][last][prelast] = res
    return res

def id2num(k):
    assert(k < go(0, 0, 0, 0));
    res, last, prelast, mask = (0, 0, 0, 0)
    cnt = zeros(10, int32)

    for i in xrange(n):
        j = 0
        while (j < 10):
            if cnt[j] < 2 and (i < 1 or last != j) and (i < 2 or prelast != j):
                if k >= go(i + 1, mask + three[j], j, last):
                    k -= go(i + 1, mask + three[j], j, last)
                else:
                    mask += three[j]
                    cnt[j] += 1
                    prelast = last
                    last = j
                    res = res * 10 + j
                    break
            j += 1
        assert(j <= 10)
    return res

def is_valid(key):
    ok = True and len(key) == n
    for i in xrange(1, n):
        ok = ok and (key[i] != key[i - 1])
        ok = ok and (i < 2 or key[i] != key[i - 2])
        if not ok:
            break
    return ok

def num2id(key):
    """
    key = строка, хаосное число 
    """
    assert(is_valid(key))
    res, last, prelast, mask = (0, 0, 0, 0)
    cnt = zeros(10, int32)

    for i in xrange(n):
        num = ord(key[i]) - ord('0');
        for j in xrange(num):
            if cnt[j] < 2 and (i < 1 or last != j) and (i < 2 or prelast != j):
                res += go(i + 1, mask + three[j], j, last)
        mask += three[num]
        cnt[num] += 1
        prelast = last
        last = num
    return res

#chaosnumbers = ChaosNumbers()
for i in xrange(1000):
    print "{0:07d}".format(id2num(i))

