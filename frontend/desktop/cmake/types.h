#ifndef _TYPES_H_INCLUDED_
#define _TYPES_H_INCLUDED_

#ifdef DESKTOP
	#define VOID  void
	#define FLOAT double
	#define INT   int
	#define VOID  void 
	#define BOOL  int
	#define TRUE  1
	#define FALSE 0
	#define ZERO_PRECISION  0.000001  
#endif 

typedef enum {
	err_code_OK                    = 0,
	err_code_ZERO_POINTER_PASSED   = 1,
	err_code_OUT_OF_MEMORY         = 2,
	err_code_WRONG_PARAMETER       = 3,
	err_code_BAD_STRUCTURE_CONTENT = 4, 
	err_code_ALGORITHM_FAILURE     = 5
} ERR_CODE;

#define _ABS(x) ((x) > 0 ? (x) :(-(x)))

#endif
