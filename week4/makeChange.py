import sys


def makeChange(m,C):
    L=[]
    for i in range(m+1):
        L.append(10000)
    L[0] =0
    for i in range(1,m+1):
        print(i)
        temp =100000
        for j in reversed(C):
            if i<j:
                continue
            else:
                temp = min(temp,L[i-j]+1)
        L[i] = temp

    return L

if __name__=="__main__":
    C = [1,5,10,25]
    m = 11
    L = makeChange(m,C)
    print(L)