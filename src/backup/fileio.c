
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "errors.h"
#include "memory.h"

static FILE* fp;
static const char* fname;
static int line_no = 1;
static int col_no  = 1;
static const char* buffer;
static int bidx = 0;

#define CHAR buffer[bidx]

void read_file(const char* fname) {

    if(fname == NULL)
        FATAL("file name is required");

    fp = fopen(fname, "r");
    if(fp == NULL)
        FATAL("cannot open input file: \"%s\": %s", fname, strerror(errno));

    fseek(fp, 0L, SEEK_END);
    size_t len = ftell(fp);
    rewind(fp);

    buffer = _alloc(len + sizeof(int));
    memset((void*)buffer, 0xFF, len + sizeof(int));
    fread((void*)buffer, sizeof(char), len, fp);

    fclose(fp);

    fname = _dup_str(fname);
}

int consume_char() {

    bidx++;
    if(CHAR == '\n') {
        line_no++;
        col_no = 1;
    }
    else if(CHAR < 0x7F)
        col_no++;

    return CHAR;
}

int consume_space() {

    while(true) {
        if(!isspace(CHAR) || CHAR > 0x7F)
            break;
        else
            consume_char();
    }

    return CHAR;
}

int get_char() {

    return CHAR;
}

int get_line() {

    return line_no;
}

int get_col() {

    return col_no;
}

const char* get_name() {

    return fname;
}
