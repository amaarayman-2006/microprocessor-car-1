#include "STD_Types.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"
#include "GPIO_private.h"
#include "GPIO_config.h"

void GPIO_SetPinDirection(u8 Port, u8 Pin, u8 Direction)
{

    if ((Port <= GPIO_PORTD) && (Pin <= GPIO_PIN7))
    {
        if (Direction == GPIO_OUTPUT)
        {

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

void GPIO_WritePin(u8 Port, u8 Pin, u8 Value)
{
    if ((Port <= GPIO_PORTD) && (Pin <= GPIO_PIN7))
    {
        if (Value == GPIO_HIGH)
        {

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

u8 GPIO_ReadPin(u8 Port, u8 Pin)
{
    u8 LOC_u8Status = 0;

    if ((Port <= GPIO_PORTD) && (Pin <= GPIO_PIN7))
    {

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

