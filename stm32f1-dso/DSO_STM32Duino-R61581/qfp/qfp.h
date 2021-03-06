#pragma once

#include "qfplib-m3.h"
       
#if 0 // 640
    #define QMUL(x,y)        (x*y)
    #define QDIV(x,y)        (x/y)
    #define QADD(x,y)        (x+y)
#else
    #define QMUL(x,y)        qfp_fmul(x,y) 
    #define QDIV(x,y)        qfp_fdiv(x,y)
    #define QADD(x,y)        qfp_fadd(x,y)        
    #define QSUB(x,y)        qfp_fsub(x,y)        
#endif
