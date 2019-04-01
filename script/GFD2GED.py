import numpy as np

vsize = 5
esize = 4
Xsize = 5
Ysize = 1

line0 = "##########" + '\n'
line3 = "#Pattern" +'\n'
line4 = "#Condition" + '\n'
line5 = "%X" + '\n'
line6 = "%Y" + '\n'

l = 7
ENDNUM = 10
ADDR = '/home/naughtycat/data/dbpedia_label' + str(l) + '/GFD/dbpedia_label' + str(l)
OUTADDR = '/home/naughtycat/data/dbpedia_label' + str(l) + '/GED_M/dbpedia_label' + str(l)

def same(a, vnum):
    flag = 0
    b = set(a)
    an = len(a)
    bn = len(b)
    if an == bn:
        return flag
    count = np.zeros(vnum, dtype=int)
    for i in range(an):
        for j in b:
            if a[i] == j:
                count[int(j)] = count[int(j)] + 1
    for k in b:
        if count[int(k)] == 2:
            flag = 1
        if count[int(k)] > 2:
            flag = 0
            return flag
    return flag


def finds(a, vnum):
    b = set(a)
    an = len(a)
    count = np.zeros(vnum, dtype=int)
    for i in range(an):
        for j in b:
            if a[i] == j:
                count[int(j)] = count[int(j)] + 1
    for k in b:
        if count[int(k)] == 2:
            c = int(k)
    flag = 1
    for i in range(an):
        if int(a[i]) == c and flag:
            d = i
            flag = 0
        if int(a[i]) == c and not flag:
            e = i
    return [c, d, e]

def writevar(v, e, num, pre, post):
    GEDfile.writelines([line0, line1, line2, line3])
    for i in range(vnum):
        if i < num:
            GEDfile.write(vline[i])
        if i == num:
            GEDfile.writelines(['v', '\t', str(int(v[i][0])), '\t', v[i][1], '\t', v[i][2]])
            GEDfile.writelines(['v', '\t', str(int(v[i][0])+1), '\t', v[i][1], '\t', v[i][2]])
        if i > num:
            GEDfile.writelines(['v', '\t', str(int(v[i][0])+1), '\t', v[i][1], '\t', v[i][2]])
    for j in range(enum):
        GEDfile.writelines(['e', '\t'])
        if int(e[j][0]) > num:
            GEDfile.write(str(int(e[j][0]) + 1))
        else:
            GEDfile.write(e[j][0])
        GEDfile.write('\t')
        GEDfile.write(e[j][1])
        GEDfile.write('\t')
        if int(e[j][2]) > num:
            GEDfile.write(str(int(e[j][2]) + 1))
            GEDfile.write('\n')
        elif j == post:
            GEDfile.write(str(int(e[j][2]) + 1))
            GEDfile.write('\n')
        else:
            GEDfile.write(e[j][2])
    GEDfile.write(line4)
    GEDfile.write(line5)
    for k in range(Xnum):
        GEDfile.write(X[k])
    GEDfile.write(line6)
    GEDfile.writelines(['eq-id', '\t', str(v[num][0]), '\t', str(v[num+1][0]), '\n'])
    return

def writeids(numnum):
    GEDfile.writelines([line0, line1, line2, line3])
    for i in range(vnum):
        GEDfile.write(vline[i])
    for j in range(enum):
        GEDfile.write(eline[j])
    GEDfile.write(line4)
    GEDfile.write(line5)
    for k in range(Xnum):
        GEDfile.write(X[k])
    GEDfile.write(line6)
    GEDfile.writelines(['eq-id', '\t', '0', '\t', 'id', '\t', str(numnum - 1), '\t', 'id', '\n'])
    return

def writelit():
    GEDfile.writelines([line0, line1, line2, line3])
    for i in range(vnum):
        GEDfile.write(vline[i])
    for j in range(enum):
        GEDfile.write(eline[j])
    GEDfile.write(line4)
    GEDfile.write(line5)
    for k in range(Xnum):
        GEDfile.write(X[k])
    GEDfile.write(line6)
    for l in range(Ynum):
        GEDfile.write(Y[l])

for k in range(ENDNUM):
    #GFDfile = open(ADDR + '.gfd.vali_m' + str(k + 1) + '.reduced', 'r')
    #GEDfile = open(OUTADDR + '.ged.m' + str(k + 1) + '_r', 'w')

    GFDfile = open(ADDR + '.gfd.vali_m' + str(k + 1), 'r')
    GEDfile = open(OUTADDR + '.ged.m' + str(k + 1), 'w')

    while 1:
        line = GFDfile.readline()
        if not line:
            break
        spline = line.split('\t')
        if spline[0][0] == '#':
            if spline[0][1] == '#':
                v = np.zeros((vsize, 3), dtype=list)
                vline = np.zeros(vsize, dtype=list)
                vnum = 0
                e = np.zeros((esize, 3), dtype=list)
                eline = np.zeros(esize*2, dtype=list)
                enum = 0
                X = np.zeros(Xsize, dtype=list)
                Xnum = 0
                Y = np.zeros(Ysize, dtype=list)
                Ynum = 0
        elif spline[0][0] == '%':
                if spline[0][1] == 'G':
                    line1 = line
                if spline[0][1] == 'L':
                    line2 = line
                if spline[0][1] == 'X':
                    flag = 1
                if spline[0][1] == 'Y':
                    flag = 0
        elif spline[0][0] == 'v':
            vvnum = int(spline[1])
            vline[vvnum] = line
            v[vvnum][0] = spline[1]
            v[vvnum][1] = spline[2]
            v[vvnum][2] = spline[3]
            if vvnum >= vnum:
                vnum = vvnum + 1
        elif spline[0][0] == 'e':
            if len(spline[0]) == 1:
                e[enum][0] = spline[1]
                '''
                for j in range(len(spline[2])):
                    if j == 0 or j == len(spline[2])-1:
                        continue
                    elif j == 1:
                        e[enum][1] = spline[2][j]
                    else:
                        e[enum][1] = e[enum][1] + spline[2][j]
                '''
                e[enum][1] = spline[2]
                e[enum][2] = spline[3]
                '''
                for k in range(len(spline[3])-1):
                    if k == 0:
                        e[enum][2] = spline[3][k]
                        continue
                    e[enum][2] = e[enum][2] + spline[3][k]
                '''
                eline[enum] = 'e' + '\t' + e[enum][0] + '\t' + e[enum][1] + '\t' + e[enum][2]
                enum = enum + 1
            elif spline[0][1] == 'q':
                if flag:
                    #X[Xnum] = spline[0] + '\t' + spline[2] + '\t' + 'name' + '\t' + spline[3]
                    X[Xnum] = spline[0] + '\t' + spline[1] + '\t' + 'name' + '\t' + spline[2]
                    Xnum = Xnum + 1
                else:
                    child = np.zeros(enum, dtype=str)
                    for i in range(enum):
                        child[i] = e[i][2]
                    if same(child, vnum):
                        [num, pre, post] = finds(child, vnum)
                        writevar(v, e, num, pre, post)
                    elif vnum == 2 or vnum == 3:
                        e[enum][0] = vnum
                        e[enum][1] = e[0][1]
                        e[enum][2] = e[0][2]
                        eline[enum] = 'e' + '\t' + str(vnum) + '\t' + e[0][1] + '\t' + e[0][2]
                        enum = enum + 1
                        if vnum == 4:
                            e[enum][0] = vnum
                            e[enum][1] = e[1][1]
                            e[enum][2] = e[2][2]
                            eline[enum] = 'e' + '\t' + str(vnum) + '\t' + e[1][1] + '\t' + e[1][2]
                            enum = enum + 1
                        v[vnum][0] = vnum
                        v[vnum][1] = v[0][1]
                        v[vnum][2] = v[0][2]
                        vline[vnum] = 'v' + '\t' + str(vnum) + '\t' + v[0][1] + '\t' + v[0][2]
                        vnum = vnum + 1
                        writeids(vnum)
                    #else:
                     #   Y[Ynum] = line
                      #  Ynum = Ynum + 1
                       # writelit()
    GFDfile.close()
    GEDfile.close()
