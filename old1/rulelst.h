#ifndef _RULELST_H
#define _RULELST_H

#include "ptrlst.h"

typedef  PtrLst RuleLst;

#define create_rule_lst()    (RuleLst*)create_lst()
#define add_rule_lst(l, p)   add_lst((PtrLst*)(l), (p))
#define reset_rule_lst(p)    reset_lst((PtrLst*)(p))
#define iterate_rule_lst(p)  iterate_lst((PtrLst*)(p))

#endif
