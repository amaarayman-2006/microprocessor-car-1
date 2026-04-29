#include "STD_Types.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"
#include "GPIO_private.h"
#include "GPIO_config.h"

/* =====================================================================
 *  GPIO_program.c — تنفيذ APIs الـ GPIO على ATmega328P
 *
 *  بنستخدم Bit-level operations عبر MACROs من BIT_MATH.h:
 *      Set_Bit(REG, BIT)   →  REG |=  (1 << BIT)
 *      Clear_Bit(REG, BIT) →  REG &= ~(1 << BIT)
 *      Get_Bit(REG, BIT)   →  ((REG >> BIT) & 1)
 *
 *  بنتأكد من صحة الـ Port والـ Pin قبل أي عملية (حماية).
 * ===================================================================== */

/* ---------- تحديد اتجاه البنّة (Input / Output) ---------- */
void GPIO_SetPinDirection(u8 Port, u8 Pin, u8 Direction)
{
    /* حماية: تأكد إن البورت والبِنّة في المدى الصحيح */
    if ((Port <= GPIO_PORTD) && (Pin <= GPIO_PIN7))
    {
        if (Direction == GPIO_OUTPUT)
        {
            /* نضع 1 في DDRx → البنّة تبقى Output */
            switch (Port)
            {
                case GPIO_PORTB: Set_Bit(DDRB, Pin); break;
                case GPIO_PORTC: Set_Bit(DDRC, Pin); break;
                case GPIO_PORTD: Set_Bit(DDRD, Pin); break;
                default: break;
            }
        }
        else if (Direction == GPIO_INPUT)
        {
            /* نضع 0 في DDRx → البنّة تبقى Input */
            switch (Port)
            {
                case GPIO_PORTB: Clear_Bit(DDRB, Pin); break;
                case GPIO_PORTC: Clear_Bit(DDRC, Pin); break;
                case GPIO_PORTD: Clear_Bit(DDRD, Pin); break;
                default: break;
            }
        }
    }
}

/* ---------- كتابة قيمة (HIGH / LOW) على بنّة ---------- *
 * ملاحظة: لو البنّة Input وكتبت HIGH عليها → ده بيفعّل الـ Pull-up
 *         الداخلي (مفيد لمفاتيح Active-LOW).
 */
void GPIO_WritePin(u8 Port, u8 Pin, u8 Value)
{
    if ((Port <= GPIO_PORTD) && (Pin <= GPIO_PIN7))
    {
        if (Value == GPIO_HIGH)
        {
            /* نضع 1 في PORTx → البنّة تبقى HIGH (5V) */
            switch (Port)
            {
                case GPIO_PORTB: Set_Bit(PORTB, Pin); break;
                case GPIO_PORTC: Set_Bit(PORTC, Pin); break;
                case GPIO_PORTD: Set_Bit(PORTD, Pin); break;
                default: break;
            }
        }
        else if (Value == GPIO_LOW)
        {
            /* نضع 0 في PORTx → البنّة تبقى LOW (0V) */
            switch (Port)
            {
                case GPIO_PORTB: Clear_Bit(PORTB, Pin); break;
                case GPIO_PORTC: Clear_Bit(PORTC, Pin); break;
                case GPIO_PORTD: Clear_Bit(PORTD, Pin); break;
                default: break;
            }
        }
    }
}

/* ---------- قراءة قيمة بنّة (0 أو 1) ---------- */
u8 GPIO_ReadPin(u8 Port, u8 Pin)
{
    u8 LOC_u8Status = 0;

    if ((Port <= GPIO_PORTD) && (Pin <= GPIO_PIN7))
    {
        /* بنقرا من PINx (مش PORTx) — ده اللي بيدّينا الحالة الفعلية للبنّة */
        switch (Port)
        {
            case GPIO_PORTB: LOC_u8Status = Get_Bit(PINB, Pin); break;
            case GPIO_PORTC: LOC_u8Status = Get_Bit(PINC, Pin); break;
            case GPIO_PORTD: LOC_u8Status = Get_Bit(PIND, Pin); break;
            default: break;
        }
    }
    return LOC_u8Status;
}
