# تقرير مشروع Robot Car — ATmega328P

> مشروع سيارة روبوت مُتحكَّم بها عبر Joystick سلكي، مع مراقبة جهد البطارية وإشعار LED أحمر عند انخفاض الجهد.
> الشريحة المستعملة هي **ATmega328P** (نفس شريحة Arduino UNO) في علبة DIP-28.

---

## 1. نظرة عامة على المشروع (Project Overview)

- **المتحكم:** ATmega328P — نفس شريحة Arduino UNO، هيك كود AVR-GCC العادي يمشي عليها.
- **تردد العمل:** 16 MHz (crystal خارجي) أو 8 MHz (داخلي)، الاتنين مدعومين من driver الـ ADC.
- **الـ Toolchain:** XC8 for AVR أو AVR-GCC — بدون مكتبات Arduino.
- **المعمارية طبقيّة:**

```
┌─────────────────────────────────────────────────┐
│  APP    :  main.c                                │
├─────────────────────────────────────────────────┤
│  HAL    :  MOTOR  ·  JOYSTICK  ·  BATTERY  · LED │
├─────────────────────────────────────────────────┤
│  MCAL   :  GPIO   ·  ADC                         │
└─────────────────────────────────────────────────┘
```

- **المخرجات الوظيفية المطلوبة:**
  1. تحرك للأمام Forward.
  2. تحرك للخلف Backward.
  3. انعطاف لليسار Left.
  4. انعطاف لليمين Right.
  5. وقوف تام Stop.
  6. قراءة الـ Joystick من خلال الـ ADC.
  7. قراءة جهد البطارية من خلال الـ ADC.
  8. إضاءة LED أحمر عند انخفاض جهد البطارية.

---

## 2. المشاكل التي تم اكتشافها في النسخة الأصلية (Problems Found)

### 2.1 مشاكل حرجة (Critical)

| # | الملف | المشكلة |
|---|------|--------|
| 1 | — | **ميزة مراقبة البطارية غير موجودة نهائياً** في النسخة الأصلية (لا module، لا حسابات جهد، لا LED). |
| 2 | — | **LED الأحمر للبطارية المنخفضة غير موجود** لا في الكود ولا في الـ wiring. |
| 3 | `main.c` | ما كانش بيستدعي `adc_init` صراحةً، بيعتمد على lazy-init مخفي داخل `analogRead`. سلوك غير واضح. |

### 2.2 مشاكل متوسطة (Medium)

| # | الملف | المشكلة |
|---|------|--------|
| 4 | `types.h` + `STD_Types.h` | كلاهما يعرّف `u8/u16/u32/...` — عند تضمين الاتنين يحدث `typedef` مكرر في نفس الـ translation unit. |
| 5 | `interface.h` | واجهة `analogRead(pin channel)` كانت تستخدم enum ينتهي عند `A5` — لا يحمل القناة 2 (البطارية) بشكل نوعي. |
| 6 | `JOYSTICK.h` | dead-zone صغيرة (±100) ممكن تسبب harakat عشوائية عند مركز الـ joystick. |

### 2.3 مشاكل صغيرة (Minor)

| # | الملف | المشكلة |
|---|------|--------|
| 7 | `JOYSTICK.h` | إعادة تعريف `NULL` بدل `<stddef.h>`. |
| 8 | `GPIO_interface.h` | `enum GPIO_Port_t` غير مستعمل ومفاتيحه لا تطابق ماكرو الـ Ports. |
| 9 | `MOTOR.c` | تعليقات غير متسقة؛ لم توضّح بجلاء سبب تعكيس أسلاك الـ Right motor. |
| 10 | `STD_Types.h` | `typedef float f8` اسم مضلّل (`f8` يوحي بـ 8 bytes). |

### 2.4 ملاحظة مهمة (تصحيح مسار)

- النسخة الأولى من هذا التقرير افترضت غلطاً أن الهدف هو **ATmega32** لأن الـ prompt ذكر ذلك، لكن `.cproj` الفعلي كان `ATmega328P` والعناوين الأصلية كانت صحيحة للـ ATmega328P. تم التراجع عن "الإصلاحات" الخاطئة وإعادة الضبط لـ ATmega328P في النسخة الحالية.
- العناوين الأصلية التي ظنّها البعض خاطئة كانت بالفعل صحيحة لـ ATmega328P:
  - `PINB=0x23, DDRB=0x24, PORTB=0x25, PINC=0x26, DDRC=0x27, PORTC=0x28, PIND=0x29, DDRD=0x2A, PORTD=0x2B`
  - `ADCL=0x78, ADCH=0x79, ADCSRA=0x7A, ADMUX=0x7C`

---

## 3. الإصلاحات المُطبَّقة (Fixes Applied)

1. **تنظيف `GPIO_private.h`** — نفس عناوين ATmega328P الأصلية مع تعليقات تشرح المصدر وتنسيق أوضح.
2. **تنظيف `GPIO_interface.h`** — إزالة `enum GPIO_Port_t` غير المستعمل، ترقيم منظّم (`GPIO_PORTB=0, PORTC=1, PORTD=2`).
3. **تنظيف `GPIO_program.c`** — formatting منظّم، لا تغيير وظيفي.
4. **تنظيف `private.h` (ADC)** — إضافة أسماء بتّات رمزية (`ADEN`, `ADSC`, `REFS0/1`, `ADLAR`, `MUX0..MUX3`, `ADPS0..ADPS2`, `ADIE`, `ADIF`, `ADATE`) بدل الأرقام السحرية.
5. **تحسين `program.c`** — قناع `ADMUX & 0xF0` (صحيح لـ ATmega328P لأن بت 4 reserved)، استخدم الأسماء الرمزية، تغيير توقيع `analogRead` إلى `u8 channel` بدل enum قصير.
6. **توحيد `types.h`** — يتضمن `STD_Types.h` بدل إعادة تعريف الـ typedefs، مع إضافة `adc_channel` enum.
7. **كتابة `BATTERY.h` + `BATTERY.c` من الصفر** — مع حسابات integer-only، hysteresis، وتعليقات ومعادلات موثّقة.
8. **كتابة `LED.h` + `LED.c`** — LED أحمر على `PB0` (ما أقدرش أحطه على `PC0` لأن `PC0` = ADC0 = Joystick Y).
9. **توسيع dead-zone** الـ joystick من ±100 إلى ±150.
10. **إعادة كتابة `main.c`** — استدعاء `adc_init` صريح، فحص البطارية كل 50 دورة لتوفير CPU.
11. **تحديث `MOTOR.c`** — تعليقات مفصّلة تشرح عكس قطبية الـ right motor.
12. **تحديث `XC8Application2.cproj`** — لإضافة `BATTERY.c/h` و `LED.c/h` لقائمة الـ Compile.
13. **إزالة ملف `MOTOR.c.bak`** من حزمة التسليم (backup قديم).

---

## 4. البنية النهائية (Final Architecture)

### 4.1 قائمة الملفات

| الطبقة | الملف | الغرض |
|--------|------|------|
| MCAL   | `STD_Types.h`        | Primitive typedefs (`u8`, `u16`, …). |
| MCAL   | `BIT_MATH.h`         | ماكروهات `Set_Bit / Clear_Bit / Tog_Bit / Get_Bit`. |
| MCAL   | `GPIO_private.h`     | عناوين Registers GPIO (ATmega328P). |
| MCAL   | `GPIO_interface.h`   | ماكرو Ports/Pins + APIs. |
| MCAL   | `GPIO_config.h`      | للتمديد المستقبلي (فارغ). |
| MCAL   | `GPIO_program.c`     | تنفيذ `GPIO_SetPinDirection / GPIO_WritePin / GPIO_ReadPin`. |
| MCAL   | `private.h`          | عناوين Registers ADC + أسماء البتّات. |
| MCAL   | `types.h`            | Enums الـ ADC (`voltage_ref`, `pre_Scalar`, `adc_channel`). |
| MCAL   | `interface.h`        | APIs الـ ADC (`adc_init`, `analogRead`). |
| MCAL   | `program.c`          | تنفيذ الـ ADC. |
| HAL    | `MOTOR.h/.c`         | تحكم الـ H-bridge (أمام/خلف/يمين/يسار/stop). |
| HAL    | `JOYSTICK.h/.c`      | قراءة محاور XY + الزر مع dead-zone. |
| HAL    | `BATTERY.h/.c`       | حساب جهد البطارية بالمللي-فولت + كشف النقص مع hysteresis. |
| HAL    | `LED.h/.c`           | تشغيل LED أحمر لتنبيه البطارية. |
| APP    | `main.c`             | Loop أساسي: Joystick → Motor، ومراقبة البطارية. |

### 4.2 اعتمادات الطبقات (Dependencies)

```
main.c ───► MOTOR.h ───► GPIO_interface.h  (MCAL)
       │           └───► JOYSTICK.h ──► interface.h (ADC)
       │                              └► GPIO_interface.h
       │
       ├──► BATTERY.h ──► interface.h (ADC)
       │
       ├──► LED.h    ──► GPIO_interface.h
       │
       └──► interface.h (adc_init)
```

لا يوجد اعتماد عكسي (لا MCAL يعتمد على HAL).

---

## 5. مخطط التوصيل (Hardware Wiring Diagram)

### 5.1 Pinout ATmega328P DIP-28

```
                        ┌───────┬───────┐
             PC6 /RESET │1      │28  PC5/ADC5/SCL
          PD0 /RXD  ───►│2   A  │27  PC4/ADC4/SDA
          PD1 /TXD  ───►│3   T  │26  PC3/ADC3
          PD2 /INT0 ───►│4   m  │25  PC2/ADC2  ◄─── Battery divider midpoint
               PD3     │5   e  │24  PC1/ADC1  ◄─── Joystick VRX
               PD4     │6   g  │23  PC0/ADC0  ◄─── Joystick VRY
                 VCC   │7   a  │22  GND
                 GND   │8      │21  AREF   ── 100 nF → GND
    XTAL1/PB6          │9   3  │20  AVCC   ── +5V
    XTAL2/PB7          │10  2  │19  PB5/SCK
               PD5     │11  8  │18  PB4    ──► L298N IN2 (Left motor)
               PD6     │12  P  │17  PB3    
               PD7     │13     │16  PB2    
          PB0     ────►│14     │15  PB1    
                        └───────┴───────┘
     PB0 ──► Red LED (low battery)
     PB4 ──► L298N IN1 (Left  motor) — pin 18
     PB5 ──► L298N IN2 (Left  motor) — pin 19
     PD0 ──► L298N IN3 (Right motor) — pin 2
     PD1 ──► L298N IN4 (Right motor) — pin 3
     PD2 ──► Joystick SW             — pin 4  (active-LOW, internal pull-up)
     PC0 ◄── Joystick VRY            — pin 23 (ADC0)
     PC1 ◄── Joystick VRX            — pin 24 (ADC1)
     PC2 ◄── Battery divider         — pin 25 (ADC2)
```

### 5.2 المخطط الكامل للدائرة (ASCII)

```
    +Vbatt (7.4 V)
       │
       │                    ┌──────────────────────────┐
      [R1 10 kΩ]             │       ATmega328P         │
       │                    │        (DIP-28)          │
       ├──► PC2 (pin 25) ──►│ ADC2                     │
       │                    │                          │
      [R2 10 kΩ]             │                          │
       │                    │                          │
      GND                   │ PC0  (pin 23) ◄──────────┼──► Joystick VRY
                            │ PC1  (pin 24) ◄──────────┼──► Joystick VRX
                            │ PD2  (pin 4 ) ◄──────────┼──► Joystick SW
                            │                          │
                            │ AREF (pin 21) ─ 100 nF ──┼─► GND
                            │ AVCC (pin 20) ───────────┼──► +5V (via ferrite/LC)
                            │ VCC  (pin 7 ) ───────────┼──► +5V
                            │ GND  (pin 8,22) ─────────┼──► GND
                            │ RESET(pin 1 ) ─ 10 kΩ ───┼──► +5V, + 100 nF → GND
                            │                          │
                            │ PB4  (pin 18) ───────────┼──► L298N IN1 (Left motor)
                            │ PB5  (pin 19) ───────────┼──► L298N IN2 (Left motor)
                            │ PD0  (pin 2 ) ───────────┼──► L298N IN3 (Right motor)
                            │ PD1  (pin 3 ) ───────────┼──► L298N IN4 (Right motor)
                            │                          │
                            │ PB0  (pin 14) ── 330 Ω ──┼──► Red LED → GND
                            │                          │
                            │ XTAL1(pin 9 ) ─┐         │
                            │ XTAL2(pin 10) ─┤ 16 MHz crystal + 22pF caps
                            │                └─────────┤
                            └──────────────────────────┘

    Joystick module                          L298N H-bridge driver
    ┌──────────────┐                         ┌─────────────────────┐
    │ +5V  ─── VCC │                         │  +12V ─── Battery + │
    │ GND  ─── GND │                         │  GND  ─── Battery − │
    │ VRX  ─── PC1 │                         │  +5V  ─── Logic VCC │
    │ VRY  ─── PC0 │                         │  ENA  ─── jumper +5V│
    │ SW   ─── PD2 │                         │  ENB  ─── jumper +5V│
    └──────────────┘                         │  IN1  ─── PB4       │
                                             │  IN2  ─── PB5       │
                                             │  IN3  ─── PD0       │
                                             │  IN4  ─── PD1       │
                                             │  OUT1/2 → Left M    │
                                             │  OUT3/4 → Right M   │
                                             └─────────────────────┘
```

### 5.3 ملاحظات توصيل هامة

1. **Common ground** — `GND` الـ ATmega328P + `GND` الـ L298N + `GND` البطارية + `GND` الـ Joystick كلهم لازم يتوصلوا مع بعض. بدون هذا، الـ ADC والـ signals الرقمية لن تعمل بشكل صحيح.
2. **AVCC (pin 20)** — يُربط بـ +5V عبر ferrite bead أو مقاومة صغيرة (10Ω) + مكثف 100nF لتقليل الضوضاء الرقمية على الـ ADC. إذا لم يتوفر، اربطه مباشرة بـ +5V.
3. **AREF (pin 21)** — ضع مكثف 100nF بين `AREF` و`GND` (حتى في وضع AVCC reference — الـ datasheet يوصي بهذا).
4. **RESET (pin 1)** — 10kΩ pull-up إلى VCC + مكثف 100nF إلى GND لتفادي reset عشوائي.
5. **Crystal (اختياري)** — للحصول على 16 MHz استخدم crystal خارجي على `XTAL1 (pin 9)` و`XTAL2 (pin 10)` مع مكثفين 22pF لكل منهما. الـ fuses لازم تتضبط `CKSEL` و`SUT` مطابقة. بدون crystal، الشريحة تعمل على 8 MHz (internal RC) — الكود يدعم الاتنين.
6. **L298N logic supply** — +5V من الـ regulator الداخلي للـ L298N يمكن استخدامه لتغذية الـ MCU (لو الـ motor battery 7-12V)، أو regulator منفصل.
7. **ENA / ENB** — إما jumper على البورد لتشغيل دائم (سرعة كاملة كما في الكود الحالي)، أو إخراج PWM من `OC0A (PD6)`/`OC0B (PD5)` و`OC2A (PB3)`/`OC2B (PD3)` لو تحب تضيف تحكم سرعة.
8. **LED** — مقاومة سلسلة 220–470Ω مع LED أحمر قياسي (Vf ≈ 2V، If ≈ 10mA). خرج `HIGH` على `PB0` ⇒ LED مضيء.
9. **Decoupling caps** — 100nF قرب كل من `VCC` و`AVCC`، و10µF إلكتروليتي قرب الشريحة.

---

## 6. جدول Pin Mapping

| Pin رقمي | الاسم | الوظيفة | الاتجاه | ملاحظة |
|:-------:|:-----:|:-------|:-------:|:------|
| 1  | PC6 / RESET | Reset | — | 10kΩ pull-up + 100nF إلى GND |
| 2  | PD0 / RXD   | Right motor IN1 | OUT | لا UART |
| 3  | PD1 / TXD   | Right motor IN2 | OUT | لا UART |
| 4  | PD2 / INT0  | Joystick SW push-button | IN + pull-up | active-LOW |
| 7  | VCC         | +5V | — | decouple 100nF |
| 8  | GND         | 0V  | — | — |
| 9  | PB6 / XTAL1 | Crystal in | — | اختياري 16MHz |
| 10 | PB7 / XTAL2 | Crystal out | — | اختياري 16MHz |
| 14 | PB0         | Red LED (low batt) | OUT | active-HIGH |
| 18 | PB4         | Left motor IN1 | OUT | L298N IN1 |
| 19 | PB5 / SCK   | Left motor IN2 | OUT | L298N IN2 |
| 20 | AVCC        | +5V analog | — | ferrite/filter |
| 21 | AREF        | — | — | 100nF إلى GND |
| 22 | GND         | 0V analog | — | — |
| 23 | PC0 / ADC0  | Joystick Y-axis | Analog-IN | dead-zone ±150 |
| 24 | PC1 / ADC1  | Joystick X-axis | Analog-IN | dead-zone ±150 |
| 25 | PC2 / ADC2  | Battery voltage (بعد divider) | Analog-IN | divider 1:1 → max 10V |

---

## 7. جدول حقيقة التحكم بالموتور (Motor Control Truth Table)

| Motor_Direction_t | PB4 (LM-IN1) | PB5 (LM-IN2) | PD0 (RM-IN1) | PD1 (RM-IN2) | النتيجة الفيزيائية |
|:-----------------:|:-------------:|:-------------:|:-------------:|:-------------:|:-----------------|
| `MOTOR_STOP`      | 0 | 0 | 0 | 0 | السيارة واقفة |
| `MOTOR_FORWARD`   | 1 | 0 | 0 | 1 | أمام — كلا الموتورين يدفع |
| `MOTOR_BACKWARD`  | 0 | 1 | 1 | 0 | خلف — كلا الموتورين يسحب |
| `MOTOR_LEFT`      | 0 | 0 | 0 | 1 | الأيسر واقف، الأيمن قدام ⇒ قوس لليسار |
| `MOTOR_RIGHT`     | 1 | 0 | 0 | 0 | الأيسر قدام، الأيمن واقف ⇒ قوس لليمين |

> **ملاحظة:** القطبية على الـ right motor معكوسة عن الـ left motor. السبب: الموتور الأيمن مُركَّب ميكانيكياً في الاتجاه المعاكس للأيسر. لو السيارة عكست الاتجاه عند أول تشغيل — **بدّل فقط طرفي الموتور** في الـ screw terminal من L298N ولا تعدّل الكود.

### 7.1 Truth table بسيطة لكل موتور (H-bridge standard)

| IN1 | IN2 | السلوك |
|:---:|:---:|:-----|
|  0  |  0  | Stop / Coast |
|  1  |  0  | Rotate one way |
|  0  |  1  | Rotate opposite way |
|  1  |  1  | Brake (تجنّب الاستخدام) |

---

## 8. شرح منطق الـ Joystick

### 8.1 مبدأ العمل

الـ Joystick XY عبارة عن مقاومتين متغيرتين (potentiometers) مركّبتين بزاوية 90°.
كل محور يعطي جهد من 0V إلى VCC:
- مركز الـ joystick = ≈ VCC/2 ≈ 2.5V → ADC ≈ 511.
- دفع كامل في اتجاه = ≈ VCC → ADC ≈ 1023.
- دفع كامل في العكس = ≈ 0V → ADC ≈ 0.

### 8.2 الـ Dead-zone

الـ potentiometers الرخيصة نادراً ما تعطي 511 بالضبط في مركزها (عادة 480-540)، لذلك نعرّف dead-zone:

```
JOYSTICK_CENTER       = 511
JOYSTICK_DEAD_ZONE    = 150
JOYSTICK_HIGH_THRESH  = 661   ( 511 + 150 )
JOYSTICK_LOW_THRESH   = 361   ( 511 − 150 )
```

خارج [361, 661] = دفع مقصود. داخل هذا المجال = STOP.

### 8.3 خوارزمية تحديد الاتجاه

الأولوية لمحور Y (الحركة قدام/ورا أكثر استخداماً):

```c
if (raw.y >= HIGH_THRESH)       → FORWARD
else if (raw.y <= LOW_THRESH)   → BACKWARD
else if (raw.x <= LOW_THRESH)   → LEFT
else if (raw.x >= HIGH_THRESH)  → RIGHT
else                             → STOP
```

⚠ ملاحظة: هذا المنطق لا يدعم الحركات القطرية (أمام-يمين مثلاً). لو محتاج ذلك لاحقاً، ضيف حالات combined، أو طبّق PWM differential drive.

### 8.4 توجيه الـ X axis

- دفع الـ joystick يساراً عادةً يعطي قيمة ADC منخفضة → `LEFT`.
- دفعه يميناً يعطي قيمة عالية → `RIGHT`.
- لو وجدت العكس: إما اعكس اتجاه تركيب الـ joystick فيزيائياً، أو بدّل `LEFT`/`RIGHT` في الكود.

---

## 9. شرح مراقبة البطارية

### 9.1 الـ Voltage Divider

بطاريات الـ Robot cars الشائعة (7.4V 2S Li-ion، أو 9V alkaline، أو 2S lipo) أعلى من 5V المسموح على مدخل ADC.
الحل: مقسم جهد بمقاومتين.

```
V_batt ──[R1 10kΩ]──┬──► PC2 (ADC2)
                     │
                   [R2 10kΩ]
                     │
                    GND
```

**الصيغة الأساسية:**

```
V_adc = V_batt × R2 / (R1 + R2)
```

مع `R1 = R2 = 10kΩ`: `V_adc = V_batt / 2`.

يعني بطارية 8.4V → ADC يشوف 4.2V → ضمن نطاق الـ 5V بمقدار أمان كافٍ.

### 9.2 تحويل ADC إلى جهد (integer math)

```
V_adc_mV  = ADC × V_ref_mV / 1023
V_batt_mV = V_adc_mV × (R1 + R2) / R2
```

مثال عددي: AVCC = 5000mV، ADC = 750، R1=R2=10kΩ:
```
V_adc_mV  = 750 × 5000 / 1023      = 3666
V_batt_mV = 3666 × (10+10) / 10     = 7332 mV ≈ 7.33 V
```

### 9.3 عتبات انخفاض الجهد (Hysteresis)

```
BATTERY_LOW_THRESHOLD_MV     = 6500   // نزول تحت هذا ⇒ فعّل LED
BATTERY_RECOVER_THRESHOLD_MV = 6800   // يجب الصعود فوق هذا للإطفاء
```

بدون hysteresis، الـ LED يومض بسرعة عندما يتذبذب الجهد عند العتبة بسبب الحمل المتغير. الفجوة (300mV) تمنع هذا.

### 9.4 كيف تُعدّل الأرقام لبطاريتك

- **9V alkaline:** `LOW=7200, RECOVER=7600` (9V battery عمرها ينتهي ~7V).
- **6V (4× AA):** `LOW=5000, RECOVER=5300`.
- **2S Li-ion (الافتراضي):** `LOW=6500, RECOVER=6800`.
- **للبطاريات > 10V (3S Li-ion = 11.1V):** غيّر نسبة الـ divider، مثلاً R1=20kΩ, R2=10kΩ (divider 1:3) → يتحمل حتى 15V. عدّل `BATTERY_R1_OHMS` في الـ header.

### 9.5 سلوك الـ LED

- جهد طبيعي → LED OFF.
- جهد ≤ 6.5V → LED ON (مستمر، ليس وامضاً).
- الـ LED يبقى مضاءً حتى يعود الجهد ≥ 6.8V.
- الـ main loop يفحص البطارية مرة كل 50 دورة فقط لتجنب تحميل CPU.

---

## 10. قائمة الاختبار (Testing Checklist)

### 10.1 اختبارات hardware (قبل تحميل الـ firmware)

- [ ] قياس VCC على الشريحة = 5.0V ± 0.2V.
- [ ] قياس AVCC (pin 20) = 5.0V.
- [ ] قياس الجهد على `PC2` (pin 25) مباشرة بالـ multimeter = نصف جهد البطارية.
- [ ] التأكد من common ground (< 1Ω بين كل الـ GND points).
- [ ] ENA/ENB على L298N jumpered to +5V.
- [ ] RESET pin مربوط بـ pull-up (قراءة الجهد عليه = 5V).

### 10.2 اختبارات unit-level

- [ ] **GPIO:** كود بسيط يومض LED على `PB0` ← تأكد أن الـ pin شغالة.
- [ ] **ADC:** اقرأ `ADC0` — يقرأ 0 عند GND و ~1023 عند +5V.
- [ ] **Joystick:** راحة الـ joystick → `JOYSTICK_GET_DIR()` يرجع `STOP`. دفع كل اتجاه يرجع الاتجاه المقابل.
- [ ] **Battery:** غذِّ 7.4V على الـ input → `BATTERY_ReadVoltage_mV()` يرجع قيمة بين 7200-7600.
- [ ] **Motor:** `MOTOR_Move(MOTOR_FORWARD)` → كلا الموتورين يدوران أماماً.

### 10.3 اختبارات integration

- [ ] دفع الـ joystick للأمام → السيارة تتحرك للأمام (≤ 100ms تأخير).
- [ ] العودة للوسط → السيارة تقف بسرعة.
- [ ] كل الاتجاهات الأربعة تتصرف بشكل صحيح.
- [ ] بطارية 6V (محاكاة) → LED الأحمر يُضيء خلال ثانية.
- [ ] رفع الجهد تدريجياً فوق 6.8V → LED يُطفأ.
- [ ] تذبذب الـ joystick السريع لا يُوقف السيارة (dead-zone).

### 10.4 اختبارات طويلة (Soak)

- [ ] تشغيل 30 دقيقة → لا سخونة على الشريحة أو L298N.
- [ ] الجهد يُقرأ بثبات (لا قفزات مفاجئة).

---

## 11. ملاحظات نهائية واقتراحات (Final Notes & Suggestions)

### 11.1 تحسينات قادمة (اختيارية)

1. **PWM speed control** — اربط `ENA` بـ `OC0A (PD6)` و`ENB` بـ `OC2A (PB3)` وولّد إشارة PWM بنسبة متناسبة مع دفع الـ joystick (بدل bang-bang).
2. **Diagonal movement** — أضف `FORWARD_LEFT`, `FORWARD_RIGHT`, `BACKWARD_LEFT`, `BACKWARD_RIGHT` باستخدام تركيب X و Y معاً + PWM للموتور الأبطأ.
3. **Interrupt-driven ADC** — استخدم ADC free-running mode مع ISR بدل polling، لتقليل CPU load.
4. **Watchdog timer** — فعّل `WDT` لحماية السيارة من hang.
5. **Failsafe على فقدان الـ joystick** — لو كل القراءات = 0 أو = 1023 لأكثر من X ثانية (دليل فصل سلك)، أوقف الموتورات.
6. **Buzzer إضافي** — مع LED البطارية، ضيف تنبيه صوتي.
7. **EEPROM calibration** — احفظ قيمة مركز الـ joystick الفعلية عند أول تشغيل في EEPROM بدل الاعتماد على 511 النظري.

### 11.2 أشياء موصى بها في الـ hardware

- كابل twisted pair للـ joystick ← يقلل الضوضاء.
- مكثفات 100nF من `PC0` إلى `GND` ومن `PC1` إلى `GND` (low-pass filter على إشارات الـ joystick).
- diodes flyback على مداخل الموتور (L298N يحويها لكن الأمان مطلوب).
- fuse 2A في سلسلة +Vbatt.

### 11.3 افتراضات مُصرَّحة (Assumptions)

- البطارية الافتراضية = **2S Li-ion** بجهد اسمي 7.4V، عدّل الثوابت في `BATTERY.h` لأي بطارية أخرى.
- L298N في full-speed (ENA/ENB jumpers).
- F_CPU = 16 MHz (crystal) أو 8 MHz (internal). الاتنين مدعومين.
- الـ joystick module نوع KY-023 الشائع (VCC/GND/VRX/VRY/SW).
- الموتور الأيمن مُركَّب معكوساً فيزيائياً؛ لو غير ذلك، بدّل أسلاك الموتور بدل الكود.
- المشروع يشتغل على Atmel Studio / Microchip Studio بـ XC8 toolchain أو AVR-GCC.

### 11.4 خلاصة

المشروع في نسخته الأصلية كان يحمل أساساً معمارياً سليماً (طبقات MCAL/HAL/APP) وإعداد ATmega328P صحيح، لكنه كان **ينقص كلّياً ميزة مراقبة البطارية ولمبة تنبيه الـ low-battery**، وفيه مشاكل متوسطة (duplicate typedefs، dead-zone صغيرة، lazy ADC init). بعد الإصلاحات، الكود الآن:

- يبني بنجاح على ATmega328P (نفس شريحة Arduino UNO).
- يقرأ الـ Joystick بثبات مع dead-zone معقولة.
- يدير موتور H-bridge بشكل صحيح.
- يراقب البطارية بحسابات integer دقيقة مع hysteresis لمنع وميض الـ LED.
- يُضيء LED أحمر واضح عند انخفاض الجهد.
- قابل للتوسّع (PWM, WDT, diagonal moves, …).

**المشروع جاهز للبرمجة على الـ hardware والاختبار.**
