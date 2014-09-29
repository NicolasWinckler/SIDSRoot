/*include files */

#ifdef WIN32
# include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>

/* define EXPORT */
# ifdef EXPORT
#  undef EXPORT
# endif
#ifdef WIN32
# ifdef __cplusplus
#  define EXPORT extern "C" __declspec(dllexport)
# else
#  define EXPORT __declspec(dllexport)
# endif
#else
# define EXPORT
#endif

#define esrio_badnumarg_err 		-10L
#define esrio_maxnumdecays 		   20L
