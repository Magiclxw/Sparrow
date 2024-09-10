#ifndef __SYS_CONFIG_H
#define __SYS_CONFIG_H

#include <stdlib.h>

#ifndef NBBY
# define NBBY 8		/* number of bits in a byte */
#endif
#ifndef HZ
# define HZ (60)
#endif
#ifndef NOFILE
# define NOFILE	(60)
#endif
#ifndef PATHSIZE
# define PATHSIZE (1024)
#endif

#define MAXPATHLEN PATH_MAX

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

#ifndef howmany
#define    howmany(x, y)   (((x)+((y)-1))/(y))
#endif

#define OPERATE_SUCCESS 							        (0)
#define OPERATE_ERROR_INVALID_PARAMETERS 					(-1)
#define OPERATE_ERROR_UNSUPPORTED 						    (-2)
#define OPERATE_ERROR_EXCEPTION 							(-3)
#define OPERATE_ERROR_BUSY 									(-4)
#define OPERATE_ERROR_CREATEFAILED 							(-5)
#define OPERATE_ERROR_OPENFAILED 							(-6)
#define OPERATE_ERROR_TIMEOUT 								(-7)
#define OPERATE_ERROR_BUFFNOTENOUGH 						(-8) 

#endif