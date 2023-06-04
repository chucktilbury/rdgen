#ifndef _FILEIO_H
#define _FILEIO_H

typedef void* FH;

void read_file(const char* fname);
int get_char();
int consume_char();
int consume_space();

const char* get_fname();
int get_line();
int get_col();

#define END_OF_FILE -1;

#endif /* _FILEIO_H */
