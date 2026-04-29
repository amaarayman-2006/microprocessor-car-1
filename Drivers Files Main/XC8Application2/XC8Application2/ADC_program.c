#include "types.h"
#include "private.h"
#include "interface.h"

/* =====================================================================
 *  program.c — ADC driver implementation for ATmega328P
 *
 *  Important fix in this version:
 *  ------------------------------
 *  The robot reads several ADC channels in quick sequence:
 *      A0 = joystick X
 *      A1 = joystick Y
 *      A3 = battery voltage divider
 *
 *  On AVR, after changing the ADC multiplexer channel, the first conversion
 *  can still be affected by the previous channel because the internal
 *  sample/hold capacitor needs time to settle. That can cause exactly the
 *  kind of unstable behaviour seen with corner/diagonal joystick positions
 *  and battery LED readings.
 *
 *  Fix:
 *      - remember the previous ADC channel
 *      - when the requested channel changes, do one dummy conversion
 *      - discard that dummy result
 *      - then do the real conversion and return it
 * ===================================================================== */

/* 0xFF means "no channel selected yet" */
static u8 s_last_adc_channel = 0xFFu;

static u16 ADC_ReadSelectedChannelOnce(void)
{
    /* Start conversion */
    setBIT(ADCSRA, ADSC);

    /* Wait until conversion completes */
    while (readBIT(ADCSRA, ADSC))
    {
        /* wait */
    }

    /* ADC must be read low byte first, then high byte */
    u8 low  = ADCL;
    u8 high = ADCH;

    return (u16)(((u16)(high & 0x03u) << 8) | low);
}

void adc_init(voltage_ref vref, pre_Scalar Scalar)
{
    /* ----------- Voltage reference selection (REFS1:REFS0) ----------- */
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

        case INTERNAL:   /* Internal 1.1V reference on ATmega328P */
            setBIT(ADMUX, REFS1);
            setBIT(ADMUX, REFS0);
            break;

        default:
            clearBIT(ADMUX, REFS1);
            setBIT  (ADMUX, REFS0);   /* safe default: AVCC */
            break;
    }

    /* Right-adjusted result: ADCH contains bits 9..8, ADCL contains bits 7..0 */
    clearBIT(ADMUX, ADLAR);

    /* ----------- Prescaler selection (ADPS2:ADPS0) ----------- */
    clearBIT(ADCSRA, ADPS0);
    clearBIT(ADCSRA, ADPS1);
    clearBIT(ADCSRA, ADPS2);

    switch (Scalar)
    {
        case DIV_2:
            /* 000/001 = /2 on AVR; cleared bits are enough here */
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
        default:
            setBIT(ADCSRA, ADPS0);
            setBIT(ADCSRA, ADPS1);
            setBIT(ADCSRA, ADPS2);
            break;
    }

    /* Enable ADC */
    setBIT(ADCSRA, ADEN);

    /* Force a dummy conversion on the first analogRead() after init */
    s_last_adc_channel = 0xFFu;
}

u16 analogRead(u8 channel)
{
    /* Auto-init safety net */
    if (readBIT(ADCSRA, ADEN) == 0U)
    {
        adc_init(AVCC, DIV_128);
    }

    /* ATmega328P DIP package has ADC0..ADC5; keep the mask conservative. */
    channel &= 0x07u;

    /* Keep reference bits and ADLAR; update only MUX3..MUX0. */
    ADMUX = (u8)((ADMUX & 0xF0u) | (channel & 0x0Fu));

    if (channel != s_last_adc_channel)
    {
        /* Throw away the first result after a channel switch. */
        (void)ADC_ReadSelectedChannelOnce();
        s_last_adc_channel = channel;
    }

    return ADC_ReadSelectedChannelOnce();
}
