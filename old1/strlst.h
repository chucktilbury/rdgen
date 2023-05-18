#ifndef _STRLST_H
#define _STRLST_H

#include "ptrlst.h"

typedef  PtrLst StrLst;

#define create_str_lst()    (StrLst*)create_lst()
#define add_str_lst(l, p)   add_lst((PtrLst*)(l), (p))
#define reset_str_lst(p)    reset_lst((PtrLst*)(p))
#define iterate_str_lst(p)  iterate_lst((PtrLst*)(p))

#endif
