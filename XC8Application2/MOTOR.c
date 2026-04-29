#include "MOTOR.h"

/* =====================================================================
 *  MOTOR.c — تنفيذ موديل الموتورين عبر الـ H-Bridge (L298N / L293D)
 *  -------------------------------------------------------------------
 *  الـ MCU المستخدم: ATmega328P (نفس الشريحة اللي في الـ Arduino UNO).
 *
 *  ENA/ENB على بوردة الـ L298N مفروض يكونوا مجمبرين على +5V (Full Speed).
 *  لو محتاج تتحكم في السرعة بـ PWM لازم تربط:
 *      ENA -> OC0A (PD6, pin 12)  أو  OC0B (PD5, pin 11)
 *      ENB -> OC2A (PB3, pin 17)  أو  OC2B (PD3, pin 5)
 *  وتضيف طبقة PWM فوق الموديل ده.
 *
 *  ⚙️  التوصيلات الفعلية اللي اليوزر عاملها:
 *  --------------------------------------------------------------
 *      Left motor :
 *          IN1  →  D12 = PB4
 *          IN2  →  D13 = PB5
 *      Right motor:
 *          IN3  →  D1  = PD1   (بنستخدمه كـ RM_IN1)
 *          IN4  →  D0  = PD0   (بنستخدمه كـ RM_IN2)
 *
 *  📌 ملاحظة عن قطبية الموتورين:
 *  الموتورين الاتنين متركّبين بالمقلوب فيزيائيًا على الشاسيه،
 *  فعكسنا قطبيتهم الاتنين بنفس النسبة في الكود (بِتَّات IN1/IN2 و IN3/IN4
 *  معكوسين عن الافتراضي).
 *  → النتيجة: لما الكود يقول "FORWARD" الموتور بيلف للقدام فعليًا.
 *  → LEFT/RIGHT بيفضلوا صح لأن العكس متماثل لكلا الموتورين.
 *
 *  ⚠️  تحذير على D0/D1:
 *  الـ D0 = RXD و D1 = TXD ← دول بنّات الـ UART.
 *  لما تستخدمهم كـ GPIO عادي مفيش UART شغّالة، يعني الـ Serial.print
 *  من الـ Arduino IDE هيعطل (ومفيش Bootloader Upload شغّال أثناء التشغيل).
 *  لو حسّيت بمشكلة Upload: شيل الموتور من D0/D1، ارفع الكود، وارجّعهم.
 * ===================================================================== */

/* ----------------- خرائط البِنّات ----------------- */

/* الموتور الشمال (Left) */
#define LM_IN1_PORT   GPIO_PORTB
#define LM_IN1_PIN    GPIO_PIN4   /* D12 = PB4 */
#define LM_IN2_PORT   GPIO_PORTB
#define LM_IN2_PIN    GPIO_PIN5   /* D13 = PB5 */

/* الموتور اليمين (Right) — لاحظ: IN3 على PD1 و IN4 على PD0 (مظبوط على توصيلتك) */
#define RM_IN1_PORT   GPIO_PORTD
#define RM_IN1_PIN    GPIO_PIN1   /* D1 = PD1 = IN3 */
#define RM_IN2_PORT   GPIO_PORTD
#define RM_IN2_PIN    GPIO_PIN0   /* D0 = PD0 = IN4 */

/* =====================================================================
 *  🔘 KILL-SWITCH PIN (يقفل ويفتح القدرة على الموتورين فعليًا)
 *  --------------------------------------------------------------------
 *  بنّة GPIO خاصة بتتحكم في Relay (أو MOSFET) موضوع في خط البطارية
 *  اللي رايح للـ L298N. لما البنّة HIGH → القدرة موصّلة، LOW → مقطوعة.
 *
 *  📍 الاختيار: PB1 = D9 (بِنّة فاضية، قريبة من LED على PB0).
 *
 *  📋 منطق التشغيل (Active-HIGH Relay Module — الأكثر شيوعًا):
 *      HIGH (5V) → Relay Closed → الموتورين بيوصلهم كهربا
 *      LOW  (0V) → Relay Open   → الموتورين مقطوعين (كأن البطارية مفصولة)
 *
 *  ⚠️  لو الـ Relay اللي عندك Active-LOW (بعض الموديولات كده):
 *      اعكس الـ KILL_ON / KILL_OFF تحت — وهتشتغل تمام.
 * ===================================================================== */
#define KILL_PORT     GPIO_PORTB
#define KILL_PIN      GPIO_PIN1   /* D9 = PB1 → IN بتاعت الـ Relay */
#define KILL_ON       GPIO_HIGH   /* القدرة موصّلة */
#define KILL_OFF      GPIO_LOW    /* القدرة مقطوعة */

/* =====================================================================
 *  دورة الـ Software-PWM للحركة القُطرية:
 *  --------------------------------------------------------------------
 *  مفيش Hardware-PWM متاح على البنّات اللي اخترتها (D12, D13, D1, D0)،
 *  فبنعمل PWM شغّال بالـ software:
 *
 *     - عداد 0..PWM_PERIOD-1
 *     - العجلة "الجوّانية" (الأبطأ في القُطر) بتشتغل بس لما العداد
 *       أقل من PWM_DUTY_INNER  (يعني نسبة دوران).
 *     - العجلة "البرّانية" بتشتغل طول الوقت (Full Speed).
 *
 *  PWM_PERIOD = 16 و PWM_DUTY_INNER = 8 → 50% duty
 *  لو العربية بتلف بسرعة على القُطر زيادة، نقّص الـ DUTY (مثلًا 5).
 *  لو بطيئة جدًا في القطر، زوّد الـ DUTY (مثلًا 11).
 * ===================================================================== */
#define PWM_PERIOD       16u
#define PWM_DUTY_INNER    8u   /* 8/16 = 50% (نقدر نزوّد/نقلّل بسهولة) */

/* =====================================================================
 *  MOTOR_Init — تهيئة بنّات الـ H-Bridge كـ OUTPUT + إيقاف
 * ===================================================================== */
void MOTOR_Init(void)
{
    /* بنّات الـ H-Bridge كـ OUTPUT */
    GPIO_SetPinDirection(LM_IN1_PORT, LM_IN1_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(LM_IN2_PORT, LM_IN2_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(RM_IN1_PORT, RM_IN1_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(RM_IN2_PORT, RM_IN2_PIN, GPIO_OUTPUT);

    /* بِنّة الـ Kill-Switch (Relay control) كـ OUTPUT — تبدأ "موصّلة" */
    GPIO_SetPinDirection(KILL_PORT, KILL_PIN, GPIO_OUTPUT);
    GPIO_WritePin       (KILL_PORT, KILL_PIN, KILL_ON);

    MOTOR_Stop();   /* نبدأ بحالة آمنة (الموتورين واقفين) */
}

/* =====================================================================
 *  MOTOR_SetLeft — تحكم في موتور الشمال لوحده
 *  --------------------------------------------------------------------
 *  ⚠️  ملاحظة: الموتور الشمال متركّب "مقلوب" على الشاسيه،
 *      عشان كده عكسنا قطبيته في الكود (IN1/IN2 معكوسين).
 *      الكود بيقول "FORWARD" لكن العتاد بيتحرّك للقدام فعليًا
 *      بعد تعويض القلب الفيزيائي.
 * ===================================================================== */
void MOTOR_SetLeft(Motor_HalfDir_t dir)
{
    switch (dir)
    {
        case MOTOR_HALF_FORWARD:
            /* قطبية معكوسة (تعويض ركوب الموتور بالمقلوب):
             * IN1=0 / IN2=1 → الموتور بيلف "للقدام" فعليًا */
            GPIO_WritePin(LM_IN1_PORT, LM_IN1_PIN, GPIO_LOW );
            GPIO_WritePin(LM_IN2_PORT, LM_IN2_PIN, GPIO_HIGH);
            break;

        case MOTOR_HALF_BACKWARD:
            /* IN1=1 / IN2=0 → الموتور بيلف "للورا" فعليًا */
            GPIO_WritePin(LM_IN1_PORT, LM_IN1_PIN, GPIO_HIGH);
            GPIO_WritePin(LM_IN2_PORT, LM_IN2_PIN, GPIO_LOW );
            break;

        case MOTOR_HALF_STOP:
        default:
            /* IN1=0 / IN2=0 → الموتور واقف (Coast/Brake على حسب الـ driver) */
            GPIO_WritePin(LM_IN1_PORT, LM_IN1_PIN, GPIO_LOW);
            GPIO_WritePin(LM_IN2_PORT, LM_IN2_PIN, GPIO_LOW);
            break;
    }
}

/* =====================================================================
 *  MOTOR_SetRight — تحكم في موتور اليمين لوحده
 *  --------------------------------------------------------------------
 *  ⚠️  ملاحظة: نفس موضوع الشمال — الموتور اليمين كمان متركّب مقلوب،
 *      فعكسنا قطبيته في الكود برضو.
 * ===================================================================== */
void MOTOR_SetRight(Motor_HalfDir_t dir)
{
    switch (dir)
    {
        case MOTOR_HALF_FORWARD:
            /* قطبية معكوسة:
             * IN3=1 / IN4=0 → الموتور اليمين بيلف "للقدام" فعليًا */
            GPIO_WritePin(RM_IN1_PORT, RM_IN1_PIN, GPIO_HIGH);
            GPIO_WritePin(RM_IN2_PORT, RM_IN2_PIN, GPIO_LOW );
            break;

        case MOTOR_HALF_BACKWARD:
            /* IN3=0 / IN4=1 → الموتور اليمين بيلف "للورا" فعليًا */
            GPIO_WritePin(RM_IN1_PORT, RM_IN1_PIN, GPIO_LOW );
            GPIO_WritePin(RM_IN2_PORT, RM_IN2_PIN, GPIO_HIGH);
            break;

        case MOTOR_HALF_STOP:
        default:
            GPIO_WritePin(RM_IN1_PORT, RM_IN1_PIN, GPIO_LOW);
            GPIO_WritePin(RM_IN2_PORT, RM_IN2_PIN, GPIO_LOW);
            break;
    }
}

/* =====================================================================
 *  MOTOR_Move — أوامر "للعربية كلها" (الحركات الأساسية الأربعة + وقوف)
 * ===================================================================== */
void MOTOR_Move(Motor_Direction_t dir)
{
    switch (dir)
    {
        case MOTOR_FORWARD:
            /* الموتورين الاتنين بيلفّوا قدام */
            MOTOR_SetLeft (MOTOR_HALF_FORWARD);
            MOTOR_SetRight(MOTOR_HALF_FORWARD);
            break;

        case MOTOR_BACKWARD:
            /* الموتورين الاتنين بيلفّوا ورا */
            MOTOR_SetLeft (MOTOR_HALF_BACKWARD);
            MOTOR_SetRight(MOTOR_HALF_BACKWARD);
            break;

        case MOTOR_LEFT:
            /* Skid-steer شمال: الموتور الشمال واقف، الموتور اليمين قدام → العربية بتلف شمال */
            MOTOR_SetLeft (MOTOR_HALF_STOP);
            MOTOR_SetRight(MOTOR_HALF_FORWARD);
            break;

        case MOTOR_RIGHT:
            /* Skid-steer يمين: الموتور الشمال قدام، الموتور اليمين واقف → العربية بتلف يمين */
            MOTOR_SetLeft (MOTOR_HALF_FORWARD);
            MOTOR_SetRight(MOTOR_HALF_STOP);
            break;

        case MOTOR_STOP:
        default:
            MOTOR_Stop();
            break;
    }
}

/* =====================================================================
 *  MOTOR_Stop — وقوف كامل
 * ===================================================================== */
void MOTOR_Stop(void)
{
    MOTOR_SetLeft (MOTOR_HALF_STOP);
    MOTOR_SetRight(MOTOR_HALF_STOP);
}

/* =====================================================================
 *  MOTOR_Update_FromJoystick
 *  --------------------------------------------------------------------
 *  بتقرا الجويستيك وبتشغّل الموتورين بالحركة المظبوطة.
 *  بتدعم 9 اتجاهات (4 أساسية + 4 قُطرية + وقوف).
 *
 *  الحركة القُطرية بتتعمل بـ Software-PWM:
 *    - في الـ FORWARD_LEFT مثلًا: العجلة الشمال (الجوّانية) بتتشغّل
 *      بنسبة 50% فقط، والعجلة اليمين (البرّانية) كاملة.
 *      → النتيجة: العربية بتمشي قدام مع انحراف ناعم لشمال.
 *
 *  🔘 سويتش الجويستيك (Kill-Switch هاردوير + كود):
 *  --------------------------------------------------------------------
 *  لما تضغط زرار الجويستيك (SW على D2):
 *    - أول ضغطة → يقطع القدرة عن الموتورين فعليًا (الـ Relay على D9 يفصل)
 *    - تاني ضغطة → يرجّع القدرة (الـ Relay يوصل تاني)
 *
 *  بنستخدم تكنيكين مع بعض:
 *    1) Edge-Detection: نتعامل مع الانتقال من "مش مضغوط" لـ "مضغوط" بس،
 *       عشان كل ضغطة واحدة = توجيل واحد، مش كل لفّة من اللوب.
 *    2) Debounce: نتأكد إن الإشارة ثابتة لـ N لفّات متتالية قبل ما نعتبرها
 *       ضغطة حقيقية، عشان الزراير الميكانيكية بترتد (Bouncing) لما تتضغط
 *       لو عملنا toggle بدون debounce ممكن الـ Relay يعمل "كليك كليك" 3 مرات.
 * ===================================================================== */
/* عدد اللفّات اللي الإشارة لازم تفضل ثابتة فيها قبل ما نعتبرها ضغطة حقيقية.
 * الـ loop بيلف بسرعة كبيرة، فـ 4 لفّات = ~10..20 مللي ثانية تقريبًا،
 * وده كافي لتجاوز الـ Bouncing الميكانيكي بدون تأخير محسوس للمستخدم. */
#define BTN_DEBOUNCE_COUNT   4u

void MOTOR_Update_FromJoystick(void)
{
    /* ---------- متغيّرات بتفضّل قيمتها بين النداءات (static) ---------- */
    static u8 motorsEnabled  = 1u;   /* هل الموتورين شغّالين؟ بيبدأ نعم  */
    static u8 stableBtn      = 0u;   /* الحالة المستقرّة (بعد الـ debounce) */
    static u8 lastRawBtn     = 0u;   /* آخر قراءة خام (لتتبّع الثبات)      */
    static u8 stableCount    = 0u;   /* كم لفّة الإشارة مستقرّة             */
    static u8 pwm_tick       = 0u;   /* عدّاد الـ Software-PWM              */

    /* ---------- منطق الـ Kill-Switch (Debounce + Edge-Detection) ---------- *
     * الفكرة:
     *   1) نقرا الزرار الخام
     *   2) لو القراءة اتغيّرت عن آخر مرة → نصفّر العداد
     *      لو فضلت زي ما هي → نزوّد العداد
     *   3) لما العداد يوصل لحد الـ debounce → نعتبر القراءة "مستقرّة"
     *   4) Edge-Detection على القراءة المستقرّة: انتقال 0→1 = toggle
     */
    u8 rawBtn = JOYSTICK_IsBtnPressed();

    if (rawBtn != lastRawBtn)
    {
        /* الإشارة لسه بترتد — صفّر العداد وانتظر تستقر */
        stableCount = 0u;
        lastRawBtn  = rawBtn;
    }
    else if (stableCount < BTN_DEBOUNCE_COUNT)
    {
        /* الإشارة ثابتة — كمّل عَدّ */
        stableCount++;

        if (stableCount == BTN_DEBOUNCE_COUNT)
        {
            /* وصلنا للـ threshold → القراءة دلوقتي مستقرّة */
            u8 newStable = rawBtn;

            /* Edge-Detection على الحالة المستقرّة فقط:
             * انتقال من "مش مضغوط" (0) لـ "مضغوط" (1) = ضغطة حقيقية */
            if (newStable && !stableBtn)
            {
                /* اعكس حالة التشغيل + حدّث الـ Relay فعليًا */
                motorsEnabled = (motorsEnabled) ? 0u : 1u;
                GPIO_WritePin(KILL_PORT, KILL_PIN,
                              motorsEnabled ? KILL_ON : KILL_OFF);
            }
            stableBtn = newStable;
        }
    }

    /* لو الموتورين متقفّلين بالسويتش:
     *   - الـ Relay قاطع القدرة فعليًا (الموتورين عمليًا متقطّعين)
     *   - بنرسل برضو STOP للـ H-Bridge كـ Defensive Programming
     *     عشان لو لحظة الـ Relay رجع تاني الموتور ميشتغلش بحركة قديمة. */
    if (!motorsEnabled)
    {
        MOTOR_Stop();
        return;
    }

    /* بنرفع العداد ونرجّعه لصفر لما يوصل للنهاية (دورة PWM واحدة) */
    pwm_tick++;
    if (pwm_tick >= PWM_PERIOD) pwm_tick = 0u;

    /* البنطلون الأساسي للـ PWM:
     *   inner_on = 1  لو العداد لسه في النصف الأول من الدورة (شغّل العجلة الجوّانية)
     *   inner_on = 0  لو في النصف التاني (وقّف العجلة الجوّانية)
     */
    u8 inner_on = (pwm_tick < PWM_DUTY_INNER) ? 1u : 0u;

    switch (JOYSTICK_GET_DIR())
    {
        /* ---------- الحركات الأساسية ---------- */
        case JOYSTICK_DIR_FORWARD:
            MOTOR_Move(MOTOR_FORWARD);
            break;

        case JOYSTICK_DIR_BACKWARD:
            MOTOR_Move(MOTOR_BACKWARD);
            break;

        case JOYSTICK_DIR_LEFT:
            MOTOR_Move(MOTOR_LEFT);
            break;

        case JOYSTICK_DIR_RIGHT:
            MOTOR_Move(MOTOR_RIGHT);
            break;

        /* ---------- الحركات القُطرية (Software-PWM) ---------- */

        case JOYSTICK_DIR_FORWARD_LEFT:
            /* العربية بتمشي قدام مع انحراف لشمال:
             *   - العجلة اليمين (البرّانية) → قدام كامل
             *   - العجلة الشمال (الجوّانية) → قدام بـ 50% فقط (PWM)
             */
            MOTOR_SetRight(MOTOR_HALF_FORWARD);
            MOTOR_SetLeft (inner_on ? MOTOR_HALF_FORWARD : MOTOR_HALF_STOP);
            break;

        case JOYSTICK_DIR_FORWARD_RIGHT:
            /* العربية بتمشي قدام مع انحراف ليمين:
             *   - العجلة الشمال (البرّانية) → قدام كامل
             *   - العجلة اليمين (الجوّانية) → قدام بـ 50% فقط (PWM)
             */
            MOTOR_SetLeft (MOTOR_HALF_FORWARD);
            MOTOR_SetRight(inner_on ? MOTOR_HALF_FORWARD : MOTOR_HALF_STOP);
            break;

        case JOYSTICK_DIR_BACKWARD_LEFT:
            /* رجوع لورا مع انحراف لشمال:
             *   - العجلة اليمين (البرّانية) → ورا كامل
             *   - العجلة الشمال (الجوّانية) → ورا بـ 50% فقط
             */
            MOTOR_SetRight(MOTOR_HALF_BACKWARD);
            MOTOR_SetLeft (inner_on ? MOTOR_HALF_BACKWARD : MOTOR_HALF_STOP);
            break;

        case JOYSTICK_DIR_BACKWARD_RIGHT:
            /* رجوع لورا مع انحراف ليمين:
             *   - العجلة الشمال (البرّانية) → ورا كامل
             *   - العجلة اليمين (الجوّانية) → ورا بـ 50% فقط
             */
            MOTOR_SetLeft (MOTOR_HALF_BACKWARD);
            MOTOR_SetRight(inner_on ? MOTOR_HALF_BACKWARD : MOTOR_HALF_STOP);
            break;

        /* ---------- منطقة الراحة → وقوف ---------- */
        case JOYSTICK_DIR_STOP:
        default:
            MOTOR_Stop();
            break;
    }
}
