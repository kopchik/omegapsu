
### Building and Deploying

~~~
OCD="/usr/share/openocd/scripts/"

make
openocd -f $OCD/interface/stlink-v2-1.cfg -f $OCD/board/stm32l4discovery.cfg
arm-none-eabi-gdb -ex gdb.script
~~~

### General properties

1. Uses one stm32f373 MCU for most things (3 12bit DACs, 1 16bit ADC)
1. Remote voltage sense, low-side current sense
1. Two current ranges. Resolution: on 0-5A range: ~1.25mA step, 0.125mA step on 0-500mA.
1. Precise opamps (ADA4077) and reference (ADR4533).

### FAQ

Q: **Why such a big shunt (0.2Ohm)?**  
A: It not just sensenses the current, it also limits it when leads are short-circut.
This is important because BJTs are not that rugged and have quite limited SoA (safe operating area).
"Bigger" shunts also make current limiting faster (because less amplification is needed).




