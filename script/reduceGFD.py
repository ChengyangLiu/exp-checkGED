import numpy as np

ENDNUM = 10
ADDR = '/home/naughtycat/data/dbpedia_label64/GFD/dbpedia_label64.gfd.vali_m'

vsize = 10
esize = 10
Xsize = 5
Ysize = 1
maxnum = 100000

v = np.zeros((maxnum, vsize, 3), dtype=list)
vline = np.zeros((maxnum, vsize), dtype=list)
Vnum = np.zeros(maxnum, dtype=int)
e = np.zeros((maxnum, esize, 3), dtype=list)
eline = np.zeros((maxnum, esize), dtype=list)
Enum = np.zeros(maxnum, dtype=int)
X = np.zeros((maxnum, Xsize), dtype=list)
Xnum = np.zeros(maxnum, dtype=int)
Y = np.zeros(maxnum, dtype=list)
line1 = np.zeros(maxnum, dtype=list)
line2 = np.zeros(maxnum, dtype=list)

line0 = "##########" + '\n'
line3 = "#Pattern" + '\n'
line4 = "#Condition" + '\n'
line5 = "%X" + '\n'
line6 = "%Y" + '\n'

def same(i,j):
    sameflag = True
    for k in range(Vnum[i]):
        if v[i][k][0] != v[j][k][0] or v[i][k][1] != v[j][k][1]:
            sameflag = False
    for l in range(Enum[i]):
        if e[i][l][0] != e[j][l][0] or e[i][l][1] != e[j][l][1] or e[i][l][2] != e[j][l][2]:
            sameflag = False
    return sameflag

def reduceGFD(k):
    GFDfile = open(ADDR + str(k), 'r')
    rGFDfile = open(ADDR + str(k) + '.reduced', 'w')

    num = 0
    while 1:
        line = GFDfile.readline()
        if not line:
            break
        spline = line.split('\t')
        if spline[0][0] == '#':
            if spline[0][1] == '#':
                vnum = 0
                enum = 0
                xnum = 0
        elif spline[0][0] == '%':
                if spline[0][1] == 'G':
                    line1[num] = line
                if spline[0][1] == 'L':
                    line2[num] = line
                if spline[0][1] == 'X':
                    flag = 1
                if spline[0][1] == 'Y':
                    flag = 0
        elif spline[0][0] == 'v':
            vvnum = int(spline[1])
            vline[num][vvnum] = line
            v[num][vvnum][0] = spline[1]
            v[num][vvnum][1] = spline[2]
            v[num][vvnum][2] = spline[3]
            if vvnum >= vnum:
                vnum = vvnum + 1
        elif spline[0][0] == 'e':
            if len(spline[0]) == 1:
                e[num][enum][0] = spline[1]
                e[num][enum][1] = spline[2]
                e[num][enum][2] = spline[3]
                eline[num][enum] = 'e' + '\t' + str(e[num][enum][0]) + '\t' + str(e[num][enum][1]) + '\t' + str(e[num][enum][2])
                enum = enum + 1
            elif spline[0][1] == 'q':
                if flag:
                    #X[Xnum] = spline[0] + '\t' + spline[2] + '\t' + 'name' + '\t' + spline[3]
                    X[num][xnum] = line
                    xnum = xnum + 1
                else:
                    Vnum[num] = vnum
                    Enum[num] = enum
                    Xnum[num] = xnum
                    Y[num] = line
                    num = num + 1
    print("total number:", num)
    Flag = np.ones(num, dtype=bool)

    for i in range(num):
        if Flag[i] == 1:
            for j in range(i+1, num):
                if Flag[j] == 1:
                    if Vnum[i] == Vnum[j] and Enum[i] == Enum[j]:
                        if same(i, j):
                            if Xnum[i] <= Xnum[j]:
                                Flag[j] = 0
                            else:
                                Flag[i] = 0
                                break
        print(i)


    for i in range(num):
        if Flag[i] == 1:
            rGFDfile.write(line0)
            rGFDfile.write(line1[i])
            rGFDfile.write(line2[i])
            rGFDfile.write(line3)
            for j in range(Vnum[i]):
                rGFDfile.write(vline[i][j])
            for k in range(Enum[i]):
                rGFDfile.write(eline[i][k])
            rGFDfile.write(line4)
            rGFDfile.write(line5)
            for l in range(Xnum[i]):
                rGFDfile.write(X[i][l])
            rGFDfile.write(line6)
            rGFDfile.write(Y[i])

    GFDfile.close()
    rGFDfile.close()

def main():
    for i in range(ENDNUM + 1):
        reduceGFD(i)

if __name__ == '__main__':
    main()
