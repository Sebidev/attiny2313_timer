ATTINY2313 Timer
================

Ein einfaches Hobbyprojekt aufgebaut auf einen AVR ATTINY2313 mit zwei Shiftregister, drei Taster, zwei Status-LED und zwei Siebensegmentanzeigen.

+ Millisekunden Zähler über den Interrupt TIMER0_COMPA_vect
+ Shiftregister ansteuerung
+ Taster entprellen
+ Programmlogik für den Timer


Schaltplan ist mit dabei.

Kompilieren und Flashen
--------------------------

Ich haben den AVR ATTINY2313 über das Arduino Uno Borad geflasht.
Eine passende Makefile ist mit dabei sie einfach nur ihr Arduino Uno mit dem Beispiel Arduino ISP flashen und danoch in der Konsole "make flash" eingaben.
