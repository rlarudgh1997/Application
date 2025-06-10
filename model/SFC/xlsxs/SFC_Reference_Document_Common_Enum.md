# Common Enum

## Telltale

### <p id=common_telltale_enum_stat>SFC.XXX.Telltale.XXX.Stat</p>
> State of Telltale
* DataType: HUInt64
* Means of Value
    * NONE: 0x0
    * OFF: 0x1
    * ON: 0x2
    * BLINK1: 0xA1
    * BLINK2: 0xA2
    * BLINK3: 0xA3
    * BLINK4: 0xA4
    * BLINK5: 0xA5
    * RED: 0xC1
    * GREEN: 0xC2
    * BLUE: 0xC3
    * YELLOW: 0xC4
    * AMBER: 0xC5
    * GRAY: 0xC6
    * WHITE: 0xC7
    * OPTIONAL: 0xC8
    * CYAN: 0xC9
    * MAX: 0xCA

<details><summary><b>Implementation</b></summary>
<p><b>enum class Type : HUInt64 {
&nbsp;&nbsp;&nbsp;&nbsp;NONE = 0x0,
&nbsp;&nbsp;&nbsp;&nbsp;OFF = 0x1,
&nbsp;&nbsp;&nbsp;&nbsp;ON = 0x2,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK1 = 0xA1,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK2 = 0xA2,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK3 = 0xA3,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK4 = 0xA4,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK5 = 0xA5,
&nbsp;&nbsp;&nbsp;&nbsp;RED = 0xC1,
&nbsp;&nbsp;&nbsp;&nbsp;GREEN = 0xC2,
&nbsp;&nbsp;&nbsp;&nbsp;BLUE = 0xC3,
&nbsp;&nbsp;&nbsp;&nbsp;YELLOW = 0xC4,
&nbsp;&nbsp;&nbsp;&nbsp;AMBER = 0xC5,
&nbsp;&nbsp;&nbsp;&nbsp;GRAY = 0xC6,
&nbsp;&nbsp;&nbsp;&nbsp;WHITE = 0xC7,
&nbsp;&nbsp;&nbsp;&nbsp;OPTIONAL = 0xC8,
&nbsp;&nbsp;&nbsp;&nbsp;CYAN = 0xC9,
&nbsp;&nbsp;&nbsp;&nbsp;MAX = 0xCA,
};</p></b></details>
---


### <p id=common_telltale_enum_blinkvalue_a>SFC.XXX.Telltale.XXX.BlinkValueA</p>
> Blink ValueA
* DataType: HUInt64
* Means of Value
    * NONE: 0x0
    * OFF: 0x1
    * ON: 0x2
    * ACTIVE: 0x3
    * INACTIVE: 0x4
    * TICK: 0x5
    * TOCK: 0x6
    * ENABLE: 0x7
    * DISABLE: 0x8
    * BLINK1: 0xA1
    * BLINK2: 0xA2
    * BLINK3: 0xA3
    * BLINK4: 0xA4
    * BLINK5: 0xA5
    * RED: 0xC1
    * GREEN: 0xC2
    * BLUE: 0xC3
    * YELLOW: 0xC4
    * AMBER: 0xC5
    * GRAY: 0xC6
    * WHITE: 0xC7
    * OPTIONAL: 0xC8
    * CYAN: 0xC9
    * MAX: 0xCA

<details><summary>Implementation</summary>
<p>enum class Type : HUInt64 {
&nbsp;&nbsp;&nbsp;&nbsp;NONE = 0x0,
&nbsp;&nbsp;&nbsp;&nbsp;OFF = 0x1,
&nbsp;&nbsp;&nbsp;&nbsp;ON = 0x2,
&nbsp;&nbsp;&nbsp;&nbsp;ACTIVE = 0x3,
&nbsp;&nbsp;&nbsp;&nbsp;INACTIVE = 0x4,
&nbsp;&nbsp;&nbsp;&nbsp;TICK = 0x5,
&nbsp;&nbsp;&nbsp;&nbsp;TOCK = 0x6,
&nbsp;&nbsp;&nbsp;&nbsp;ENABLE = 0x7,
&nbsp;&nbsp;&nbsp;&nbsp;DISABLE = 0x8,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK1 = 0xA1,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK2 = 0xA2,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK3 = 0xA3,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK4 = 0xA4,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK5 = 0xA5,
&nbsp;&nbsp;&nbsp;&nbsp;RED = 0xC1,
&nbsp;&nbsp;&nbsp;&nbsp;GREEN = 0xC2,
&nbsp;&nbsp;&nbsp;&nbsp;BLUE = 0xC3,
&nbsp;&nbsp;&nbsp;&nbsp;YELLOW = 0xC4,
&nbsp;&nbsp;&nbsp;&nbsp;AMBER = 0xC5,
&nbsp;&nbsp;&nbsp;&nbsp;GRAY = 0xC6,
&nbsp;&nbsp;&nbsp;&nbsp;WHITE = 0xC7,
&nbsp;&nbsp;&nbsp;&nbsp;OPTIONAL = 0xC8,
&nbsp;&nbsp;&nbsp;&nbsp;CYAN = 0xC9,
&nbsp;&nbsp;&nbsp;&nbsp;MAX = 0xCA,
};</p></details>
---

### <p id=common_telltale_enum_blinkvalue_b>SFC.XXX.Telltale.XXX.BlinkValueB</p>
> DCN: IMG_TelltaleEPB_stat</br>Blink ValueB
* DataType: HUInt64
* Means of Value
    * NONE: 0x0
    * OFF: 0x1
    * ON: 0x2
    * ACTIVE: 0x3
    * INACTIVE: 0x4
    * TICK: 0x5
    * TOCK: 0x6
    * ENABLE: 0x7
    * DISABLE: 0x8
    * BLINK1: 0xA1
    * BLINK2: 0xA2
    * BLINK3: 0xA3
    * BLINK4: 0xA4
    * BLINK5: 0xA5
    * RED: 0xC1
    * GREEN: 0xC2
    * BLUE: 0xC3
    * YELLOW: 0xC4
    * AMBER: 0xC5
    * GRAY: 0xC6
    * WHITE: 0xC7
    * OPTIONAL: 0xC8
    * CYAN: 0xC9
    * MAX: 0xCA

<details><summary>Implementation</summary>
<p>enum class Type : HUInt64 {
&nbsp;&nbsp;&nbsp;&nbsp;nbsp NONE = 0x0,
&nbsp;&nbsp;&nbsp;&nbsp;OFF = 0x1,
&nbsp;&nbsp;&nbsp;&nbsp;ON = 0x2,
&nbsp;&nbsp;&nbsp;&nbsp;ACTIVE = 0x3,
&nbsp;&nbsp;&nbsp;&nbsp;INACTIVE = 0x4,
&nbsp;&nbsp;&nbsp;&nbsp;TICK = 0x5,
&nbsp;&nbsp;&nbsp;&nbsp;TOCK = 0x6,
&nbsp;&nbsp;&nbsp;&nbsp;ENABLE = 0x7,
&nbsp;&nbsp;&nbsp;&nbsp;DISABLE = 0x8,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK1 = 0xA1,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK2 = 0xA2,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK3 = 0xA3,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK4 = 0xA4,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK5 = 0xA5,
&nbsp;&nbsp;&nbsp;&nbsp;RED = 0xC1,
&nbsp;&nbsp;&nbsp;&nbsp;GREEN = 0xC2,
&nbsp;&nbsp;&nbsp;&nbsp;BLUE = 0xC3,
&nbsp;&nbsp;&nbsp;&nbsp;YELLOW = 0xC4,
&nbsp;&nbsp;&nbsp;&nbsp;AMBER = 0xC5,
&nbsp;&nbsp;&nbsp;&nbsp;GRAY = 0xC6,
&nbsp;&nbsp;&nbsp;&nbsp;WHITE = 0xC7,
&nbsp;&nbsp;&nbsp;&nbsp;OPTIONAL = 0xC8,
&nbsp;&nbsp;&nbsp;&nbsp;CYAN = 0xC9,
&nbsp;&nbsp;&nbsp;&nbsp;MAX = 0xCA,
};</p></details>
---

## Event

### <p id=common_event_enum_stat>SFC.XXX.Event.XXX.Stat</p>
> State of Event
* DataType: HUInt64
* Means of Value
    * NONE: 0x0
    * OFF: 0x1
    * ON: 0x2
    * MAX: 0x3

<details><summary>Implementation</summary>
<p>enum class Type : HUInt64 {
&nbsp;&nbsp;&nbsp;&nbsp;NONE = 0x0,
&nbsp;&nbsp;&nbsp;&nbsp;OFF = 0x1,
&nbsp;&nbsp;&nbsp;&nbsp;ON = 0x2,
&nbsp;&nbsp;&nbsp;&nbsp;MAX = 0x3,
};</p></details>
---

### <p id=common_event_enum_type>SFC.XXX.Event.XXX.LinkedSound.Type</p>
> Linked Sound Type
* DataType: HUInt64
* Means of Value
    * NONE: 0x0
    * REPEAT_COUNT: 0x1
    * REPEAT_MINMAX: 0x2
    * DURATION: 0x3
    * DURATION_MINMAX: 0x4
    * INFINITE: 0x5
    * BLINK: 0x6
    * MAX: 0x7

<details><summary>Implementation</summary>
<p>enum class Type : HUInt64 {
&nbsp;&nbsp;&nbsp;&nbsp;NONE = 0x0,
&nbsp;&nbsp;&nbsp;&nbsp;REPEAT_COUNT = 0x1,
&nbsp;&nbsp;&nbsp;&nbsp;REPEAT_MINMAX = 0x2,
&nbsp;&nbsp;&nbsp;&nbsp;DURATION = 0x3,
&nbsp;&nbsp;&nbsp;&nbsp;DURATION_MINMAX = 0x4,
&nbsp;&nbsp;&nbsp;&nbsp;INFINITE = 0x5,
&nbsp;&nbsp;&nbsp;&nbsp;BLINK = 0x6,
&nbsp;&nbsp;&nbsp;&nbsp;MAX = 0x7,
};</p></details>
---

## Sound

### <p id=common_sound_enum_stat>SFC.XXX.Sound.XXX.Stat</p>
> State of Sound
* DataType: HUInt64
* Means of Value
    * NONE: 0x0
    * OFF: 0x1
    * ON: 0x2
    * MAX: 0x3

<details><summary>Implementation</summary>
<p>enum class Type : HUInt64 {
&nbsp;&nbsp;&nbsp;&nbsp;NONE = 0x0,
&nbsp;&nbsp;&nbsp;&nbsp;OFF = 0x1,
&nbsp;&nbsp;&nbsp;&nbsp;ON = 0x2,
&nbsp;&nbsp;&nbsp;&nbsp;MAX = 0x3,
};</p></details>
---

### <p id=common_sound_enum_type>SFC.XXX.Sound.XXX.Type</p>
> Sound Type
* DataType: HUInt64
* Means of Value
    * NONE: 0x0
    * INFINITE: 0x1
    * DURATION_MINMAX: 0x2
    * REPEAT_COUNT: 0x3
    * INTERVAL: 0x4
    * INTERVAL_SKIP_FIRST: 0x5
    * REPEAT_MINMAX: 0x6
    * MAX: 0x7

<details><summary>Implementation</summary>
<p>enum class Type : HUInt64 {
&nbsp;&nbsp;&nbsp;&nbsp;NONE = 0x0,
&nbsp;&nbsp;&nbsp;&nbsp;INFINITE = 0x1,
&nbsp;&nbsp;&nbsp;&nbsp;DURATION_MINMAX = 0x2,
&nbsp;&nbsp;&nbsp;&nbsp;REPEAT_COUNT = 0x3,
&nbsp;&nbsp;&nbsp;&nbsp;INTERVAL = 0x4,
&nbsp;&nbsp;&nbsp;&nbsp;INTERVAL_SKIP_FIRST = 0x5,
&nbsp;&nbsp;&nbsp;&nbsp;REPEAT_MINMAX = 0x6,
&nbsp;&nbsp;&nbsp;&nbsp;MAX = 0x7,
};</p></details>
---


