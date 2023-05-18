#!/usr/bin/env python3
import re

aname = ""
pname = ""
psource = []
asource = []
pheader = []
aheader = []
rules = {}

def get_name(line):

    g = re.search(r"[\"](.+)[\"]", line)
    #print("lastindex: ", g.lastindex)
    if g.lastindex >= 1:
        return g[1]
    else:
        return "source"

def get_pname(line):

    global pname
    s = get_name(line)
    pname = "parse_"+s

def get_aname(line):

    global aname
    s = get_name(line)
    aname =  "ast_"+s

# todo: ignore comments in the code
def count_curlies(line):

    count = 0

    for c in line:
        if c == '{':
            count += 1
        elif c == '}':
            count -= 1

    return count

def get_block(line, fd):

    blk = []
    count = 0

    count += count_curlies(line)
    idx = line.find("{")
    if idx >= 0:
        blk.append(line[idx:])

    for s in fd:
        if count == 0:
            return blk

        count += count_curlies(s)
        blk.append(s)

    raise Exception("Invalid {} count in block")

def get_psource(line, fd):

    global psource

    blk = get_block(line, fd)
    #print(blk)
    for s in blk:
        psource.append(s)

def get_asource(line, fd):

    global asource

    blk = get_block(line, fd)
    for s in blk:
        asource.append(s)

def get_pheader(line, fd):

    global pheader

    blk = get_block(line, fd)
    for s in blk:
        pheader.append(s)

def get_aheader(line, fd):

    global aheader

    blk = get_block(line, fd)
    for s in blk:
        aheader.append(s)

def get_directive(line, fd):

    if 0 <= line.find('psource'):
        get_psource(line, fd)
    elif 0 <= line.find('asource'):
        get_asource(line, fd)
    elif 0 <= line.find('pheader'):
        get_pheader(line, fd)
    elif 0 <= line.find('aheader'):
        get_aheader(line, fd)
    elif 0 <= line.find('pname'):
        get_pname(line)
    elif 0 <= line.find('aname'):
        get_aname(line)

def get_rule(line, fd):

    global rules

    for x in fd:


def read_file(fname):

    with open(fname, "r") as fd:
        for x in fd:
            line = x.strip()
            idx = line.find("#")
            #print(idx)
            if idx >= 0:
                line = line[0:idx]
                if len(line) == 0:
                    continue

            if len(line) == 0:
                continue
            elif line[0] == '%':
                get_directive(x, fd)
            else:
                get_rule()


read_file('test1.rdgen')
print()
print("psource: ", psource)
print("asource: ", asource)
print("pheader: ", pheader)
print("aheader: ", aheader)
print("aname: ", aname)
print("pname: ", pname)
