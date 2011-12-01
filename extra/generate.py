import sys

A = [2, 3, 4, 5, 6, 7, 8, 9]
B = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
u = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
len = 7
r = ['0', '0', '0', '0', '0', '0', '0']
limit = 2

f = open('sample.txt', 'w')
N = 0

def dive(d):
    if d == len:
        res = "".join(r)
        global N
        print res, N
        N += 1
        f.write('{0}\n'.format(res))
        if N > 1000:
            f.close()
            sys.exit(0)
    else:
#        if (len - d) % 2 == 0:
#            E = B
#        else:
#            E = A
        for c in B:
            if u[c] < limit:
                if d < 1 or (chr(c + ord('0')) != r[d - 1]):
                    if d < 2 or (chr(c + ord('0')) != r[d - 2]):
                        u[c] += 1
                        r[d] = chr(c + ord('0'))
                        dive(d + 1)
                        u[c] -= 1


dive(0)
f.close()
