#pragma once

#define BEAM_CAT(a, b) BEAM_CAT_(a, b)
#define BEAM_CAT_(a, b) a ## b

#ifdef _MSC_VER
# define BEAM_LINE __COUNTER__
#else
# define BEAM_LINE __LINE__
#endif

#define BEAM_STRINGIZE(x) BEAM_STRINGIZE_(x)
#define BEAM_STRINGIZE_(x) #x

#if !defined(BEAM_NO_SHORT_MACRO) && !defined(BEAM_NO_SHORT_PP_MACRO)
# define CAT BEAM_CAT
# define LINE BEAM_LINE
# define STRINGIZE BEAM_STRINGIZE
#endif
