#ifndef KMX32_H_
#define KMX32_H_
#include <stdint.h>

#define KMX32_DISABLE_INTERRUPTS()	asm volatile ( "cst 0x10" );
#define KMX32_ENABLE_INTERRUPTS()	asm volatile ( "sst 0x10" );

#define WRITE_REG(REGNUM, DATA)     \
asm volatile ( "mtpr  prw,  %0\n\t"      \
  "mtpr  r%2, %1\n\t"      \
  "ldrzs prw, 0" : : "r" (REGNUM >> 5), "r" (DATA), "n"(REGNUM & 0x1f))

#define WRITE_REG_OR(REGNUM, DATA)     \
asm volatile ( "mtpr  prw,  %0\n\t"      \
    "mfpr a5, r%2\n\t"      \
    "or  a5, %1\n\t"      \
    "mtpr  r%2, a5\n\t"      \
    "ldrzs prw, 0" : : "r" (REGNUM >> 5), "r"(DATA), "n"(REGNUM & 0x1f): "a5")

#define WRITE_REG_AND(REGNUM, DATA)     \
asm volatile ( "mtpr  prw,  %0\n\t"      \
    "mfpr a5, r%2\n\t"      \
    "and  a5, %1\n\t"      \
    "mtpr  r%2, a5\n\t"      \
    "ldrzs prw, 0" : : "r" (REGNUM >> 5), "r" (DATA), "n"(REGNUM & 0x1f) : "a5")

#define READ_REG(REGNUM, VAR)     \
asm  volatile ( "mtpr  prw,  %0\n\t"      \
  "mfpr  (%1), r%2\n\t"      \
  "ldrzs prw, 0" : : "r" (REGNUM >> 5), "e" (VAR), "n"(REGNUM & 0x1f) )

uint32_t  READ_REGR (uint32_t addf);
void  WRITE_REGR (uint32_t addf, uint32_t datx);


#define READ_REG_CASE(PAGE, RG, VAR) \
asm volatile ( "mtprs prw, %0\n\t" \
"mfpr (%1), r" #RG "\n\t" : : "r" (PAGE), "e" (VAR) )

#define WRITE_REG_CASE(PAGE, RG, DATA) \
asm volatile ( "mtprs prw, %0\n\t" \
"mtpr r" #RG ", %1\n\t" : : "r" (PAGE), "r" (DATA))

#endif /* KMX32_H_ */
