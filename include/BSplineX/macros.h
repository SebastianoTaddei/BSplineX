#ifndef BSPLINEX_MACROS_H
#define BSPLINEX_MACROS_H

#ifdef BSPLINEX_DEBUG_LOG_CALL
#include <cstdio>
#define DEBUG_LOG_CALL(msg) std::puts(msg);
#else
#define DEBUG_LOG_CALL(msg) ;
#endif

#endif // BSPLINEX_MACROS_H
