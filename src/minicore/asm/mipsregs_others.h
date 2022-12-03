#ifndef _MIPS_REGS_OTHERS_H_
#define _MIPS_REGS_OTHERS_H_

//#include "bits_opt.h"

#define CAUSEB_DC              27
#define CAUSEF_DC              (_ULCAST_(1)   << 27)

#define ECCB_WST               29
#define ECCF_WST               (_ULCAST_(1)   << 29)

#define STATUSB_CU1            29
#define STATUSF_CU1            (_ULCAST_(1)   << 29)

#endif /* _MIPS_REGS_OTHERS_H_ */