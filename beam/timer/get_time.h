#pragma once

// TODO: Use precise macro to distinguish POSIX.
#ifdef __GNUC__
# include "get_time_posix.h"
#elif defined(WIN32)
# include "get_time_win.h"
#else
# error get_time is supported only on POSIX or Windows.
#endif
