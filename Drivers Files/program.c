#include "types.h"
#include "private.h"
#include "interface.h"

void adc_init(voltage_ref vref, pre_Scalar Scalar)
{

    switch (vref)
    {
        case AREF:
            clearBIT(ADMUX, REFS1);
            clearBIT(ADMUX, REFS0);
            break;
        case AVCC:
            clearBIT(ADMUX, REFS1);
            setBIT  (ADMUX, REFS0);
            break;
        case INTERNAL:
            setBIT(ADMUX, REFS1);
            setBIT(ADMUX, REFS0);
            break;
    }

    clearBIT(ADMUX, ADLAR);

    clearBIT(ADCSRA, ADPS0);
    clearBIT(ADCSRA, ADPS1);
    clearBIT(ADCSRA, ADPS2);

    switch (Scalar)
    {
        case DIV_2:

            break;
        case DIV_4:
            setBIT(ADCSRA, ADPS1);
            break;
        case DIV_8:
            setBIT(ADCSRA, ADPS0);
            setBIT(ADCSRA, ADPS1);
            break;
        case DIV_16:
            setBIT(ADCSRA, ADPS2);
            break;
        case DIV_32:
            setBIT(ADCSRA, ADPS0);
            setBIT(ADCSRA, ADPS2);
            break;
        case DIV_64:
            setBIT(ADCSRA, ADPS1);
            setBIT(ADCSRA, ADPS2);
            break;
        case DIV_128:
            setBIT(ADCSRA, ADPS0);
            setBIT(ADCSRA, ADPS1);
            setBIT(ADCSRA, ADPS2);
            break;
    }

    setBIT(ADCSRA, ADEN);
}

u16 analogRead(u8 channel)
{

    if (readBIT(ADCSRA, ADEN) == 0U)
    {
        adc_init(AVCC, DIV_128);
    }

    ADMUX = (u8)((ADMUX & 0xF0U) | (channel & 0x0FU));

    setBIT(ADCSRA, ADSC);

    while (readBIT(ADCSRA, ADSC))
    {

    }

    u8 low  = ADCL;
    u8 high = ADCH;

    return (u16)(((u16)(high & 0x03U) << 8) | low);
}

