
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "fileio.h"
#include "memory.h"

struct _file_ptr_ {
    FILE* fp;
    const char* fname;
    int line_no;
    int col_no;
    int ch;
    int errors;
    int warnings;
    struct _file_ptr_* next;
};

struct _file_stack_ {
    FPTR root;
    int size;
    int errors;
    int warnings;
};

#define CONVERT(p)      ((struct _file_ptr_*)(p))
#define MKLOCAL(n, p)   struct _file_ptr_*(n)=CONVERT(p)
#define RETURN_PTR(p)   return (FPTR)(p)

FSTK create_file_stk(const char* fname) {

    struct _file_stack_* ptr = _alloc_obj(struct _file_stack_);
    if(fname != NULL) {
        ptr->root = open_input_file(ptr, fname);
        ptr->size = 1;
    }

    return (FSTK)ptr;
}

static int close_fstk(struct _file_stack_* stk) {

    /* printf(">> close: %d: %d\n",
        ((struct _file_ptr_*)stk->root)->line_no,
        ((struct _file_ptr_*)stk->root)->col_no); */
    FPTR fptr = ((struct _file_ptr_*)(stk->root))->next;
    close_file(stk->root);
    stk->root = fptr;
    return consume_char((FSTK)stk);
}

int consume_char(FSTK ptr) {

    struct _file_stack_* fs = (struct _file_stack_*)ptr;
    if(fs->root) {
        int ch = _consume_char(fs->root);
        if(ch == EOF)
            return close_fstk(fs);
        else
            return ch;
    }
    else
        return -1;
}

int get_char(FSTK ptr) {

    struct _file_stack_* fs = (struct _file_stack_*)ptr;
    if(fs->root)
        return _get_char(fs->root);
    else
        return -1;
}

FPTR open_input_file(FSTK stk, const char* fname) {

    struct _file_ptr_* ptr = _alloc_obj(struct _file_ptr_);
    ptr->fp = fopen(fname, "r");
    if(ptr->fp == NULL) {
        fprintf(stderr, "fatal error: cannot open input file: %s: %s\n", fname, strerror(errno));
        exit(1);
    }
    ptr->fname = _dup_str(fname);
    ptr->line_no = 1;
    ptr->col_no = 1;
    ptr->next = NULL;
    ptr->ch = fgetc(ptr->fp);

    if(stk) {
        struct _file_stack_* fs = (struct _file_stack_*)stk;
        if(fs->root != NULL)
            ptr->next = fs->root;
        fs->root = ptr;
        fs->size++;
    }

    RETURN_PTR(ptr);
}

int _consume_char(FPTR ptr) {

    MKLOCAL(p, ptr);

    p->ch = fgetc(p->fp);
    if(p->ch == '\n') {
        p->line_no++;
        p->col_no = 1;
    }
    else
        p->col_no++;

    return p->ch;
}

int _get_char(FPTR ptr) {

    MKLOCAL(p, ptr);
    return p->ch;
}

FPTR open_output_file(const char* fname) {

    struct _file_ptr_*ptr = _alloc_obj(struct _file_ptr_);
    ptr->fp = fopen(fname, "w");
    if(ptr->fp == NULL) {
        fprintf(stderr, "fatal error: cannot open input file: %s: %s\n", fname, strerror(errno));
        exit(1);
    }
    ptr->fname = _dup_str(fname);
    ptr->line_no = 1;
    ptr->col_no = 1;
    ptr->next = NULL;

    RETURN_PTR(ptr);
}

void emit_char(FPTR ptr, int ch) {

    MKLOCAL(p, ptr);

    if(ch == '\n') {
        p->line_no++;
        p->col_no = 1;
    }
    else
        p->col_no++;

    fputc(ch, p->fp);
}

void emit_buf(FPTR ptr, const char* str) {

    for(char* s = (char*)str; *s != '\0'; s++)
        emit_char(ptr, *s);
}

void emit_fmt(FPTR ptr, const char* fmt, ...) {

    va_list args;
    int len;

    va_start(args, fmt);
    len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char* str = _alloc(len+sizeof(int));

    va_start(args, fmt);
    vsnprintf(str, len+1, fmt, args);
    va_end(args);

    emit_buf(ptr, str);
    _free(str);
}

void emit_str(FPTR ptr, Str* str) {

    emit_buf(ptr, raw_str(str));
}

void close_file(FPTR ptr) {

    if(ptr) {
        MKLOCAL(p, ptr);
        if(p->fname)
            _free(p->fname);
        if(p->fp)
            fclose(p->fp);
        _free(p);
    }
}

