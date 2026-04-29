#ifndef BIT_MATH_H_
#define BIT_MATH_H_

#define Set_Bit(REG,BIT)    (REG |=  (1 << BIT))
#define Clear_Bit(REG,BIT)  (REG &= ~(1 << BIT))
#define Tog_Bit(REG,BIT)    (REG ^=  (1 << BIT))
#define Get_Bit(REG,BIT)    ((REG >> BIT) & 1)

#endif

