#ifndef BSPLINEX_MACROS_H
#define BSPLINEX_MACROS_H

#ifdef BSPLINEX_DEBUG_LOG_CALL
#include <cstdio>
#define DEBUG_LOG_CALL() std::puts(__PRETTY_FUNCTION__);
#else
#define DEBUG_LOG_CALL() ;
#endif

#endif // BSPLINEX_MACROS_H
