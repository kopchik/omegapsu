
## Building and Deploying

~~~
OCD="/usr/share/openocd/scripts/"

make
openocd -f $OCD/interface/stlink-v2-1.cfg -f $OCD/board/stm32l4discovery.cfg
arm-none-eabi-gdb -ex gdb.script
~~~

## General properties

1. Uses one stm32f373 MCU for most things (3 12bit DACs, 1 16bit ADC)
1. Remote voltage sense, low-side current sense
1. Two current ranges. Resolution: on 0-5A range: ~1.25mA step, 0.125mA step on 0-500mA.
1. Precise opamps (ADA4077) and reference (ADR4533).

## FAQ

Q: **Why such a big shunt (0.2Ohm)?**  
A: It not just sensenses the current, it also limits it when leads are short-circut.
This is important because BJTs are not that rugged and have quite limited SoA (safe operating area).
"Bigger" shunts also make current limiting faster (because less amplification is needed).

## Main blocks

1. Mains filter, soft start and aux supply
2. Two power channels
3. Control board
4. Enclosure


## AUX supply design

### current consumption

Per channel:

1. 1mA current set
2. 6mA pass bjt bias
3. 10mA stm32f373
4. 10mA NE5532
4. 10mA digital isolator (1.5mA@1mbits per chanel + some quiscent)
4. 10mA spare just in case

~ 50mA total, not including head (display) unit. Plus 100% overprovisioning due to bridge rectifier: 100mA.

Or 200mA total for two channels

Plus 6mA relay (coil resistance 83 Ohm@5V)

### AUX transformer minimum voltage

~5V or more expected after aux bridge rectifier.
Bridge rectifier dropout: ~2V.
Allow up to -6% mains variation: `(5+2)*1.06~=7.4V DC or 7/1.4~=5.3V AC`


### Sizing

2x100mA@5.3V >= 1VA.

Suggested transformer: 6V 1.5VA
