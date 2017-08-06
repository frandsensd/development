/* Functional description
**  Startup - nothing happens. All LED's are off.
**  Press button first time, red LED will light up for one second, then turn off. Yellow and green LED's will follow same pattern.
**  Press button second time will start lightshow sequence, turning on red, yellow, green then turning off red, yellow, green. Repeats this pattern.
**  Press button third time, will initiate shutdown sequence blinking all three LED's in desired pattern.
**
** Physical description
** Button: GPIO26, button press is active LOW! It's pulled to high through R1+R2.
** Red LED: GPIO16
** Yellow LED: GPIO20
** Green LED: GPIO21
*/

/*
**
**
**
** C Key Lines:
**
**
**
*/

