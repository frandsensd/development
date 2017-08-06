# Functional description
#  Startup - nothing happens. All LED's are off.
#  Press button first time, red LED will light up for one second, then turn off. Yellow and green LED's will follow same pattern.
#  Press button second time will start lightshow sequence, turning on red, yellow, green then turning off red, yellow, green. Repeats this pattern.
#  Press button third time, will initiate shutdown sequence blinking all three LED's in desired pattern.
#
# Issues:
# Problem with detecting button third time. It has to be timed at loop start otherwise it's not detected.
# Preferred solution would be to have a decoupled way to check for keypresses, e.g. a separate thread or interrupt event instead of polling.
#
#  Physical description
#  Button: GPIO26, button press is active LOW! It's pulled to high through R1+R2.
#  RED LED: GPIO16
#  YELLOW LED: GPIO20
#  GREEN LED: GPIO21

# Python code
#!/usr/bin/python

LEDredGPIO    = 16
LEDyellowGPIO = 20
LEDgreenGPIO  = 21
ButtonGPIO    = 26

# Get Raspberry Pi GPIO library
import RPi.GPIO as GPIO

# Get only sleep module from time library
from time import sleep 

# To switch off the "Ports already in use" warnings
GPIO.setwarnings(False)

# Set up GPIO using BCM numbering 
GPIO.setmode(GPIO.BCM) 
GPIO.setup(ButtonGPIO, GPIO.IN)
GPIO.setup(LEDredGPIO, GPIO.OUT)
GPIO.setup(LEDyellowGPIO, GPIO.OUT)
GPIO.setup(LEDgreenGPIO, GPIO.OUT)

#
# Function to control LED's
# color options: "red", "yellow", "green"
# state options: "on", "off"
#
def LED(color, state):
        if color == "red":
                gpio=LEDredGPIO
        elif color == "yellow":
                gpio=LEDyellowGPIO
        elif color == "green":
                gpio=LEDgreenGPIO
        else:
                print "LED color ", color, " invalid!"
                return
        
        if state == "on":
                GPIO.output(gpio, GPIO.HIGH)
        elif state == "off":
                GPIO.output(gpio, GPIO.LOW)
        else:
                print "LED state: ", state, " invalid - ignoring!"
        return

#
# Function to handle sleep(secs) & button press
#
def SleepCheckButtonPress(secs):
	button_press = GPIO.input(ButtonGPIO) == False
	if button_press:
		print "button pressed"
		return button_press
	sleep(secs)
	button_press = GPIO.input(ButtonGPIO) == False
	if button_press:
		print "button pressed"
	return button_press

#
# Function to wait for button press
# Button default not pressed: High.
# Press: Hi->Lo, wait 20ms, Release: Lo->Hi
#
def WaitForButtonPress():
	GPIO.wait_for_edge(ButtonGPIO, GPIO.FALLING)
	sleep(0.020)
	GPIO.wait_for_edge(ButtonGPIO, GPIO.RISING)
	return


#
# Function to handle termination sequence
#
def TerminationSequence():
	count=3
	while count > 0:
		LED("red", "on")
		LED("yellow", "on")
		LED("green", "on")
		sleep(0.5)
		LED("red", "off")
		LED("yellow", "off")
		LED("green", "off")
		sleep(0.5)
		count = count - 1
	count=10
	while count > 0:
		LED("red", "on")
		LED("yellow", "on")
		LED("green", "on")
		sleep(0.1)
		LED("red", "off")
		LED("yellow", "off")
		LED("green", "off")
		sleep(0.1)
		count = count - 1
	return

#
# Program start
#

	
print "Press button for testing LEDs..."
WaitForButtonPress()

# Turn on RED LED, sleep for one sec then turn LED off
LED("red", "on")
sleep(1)
LED("red", "off")

# Turn on YELLOW LED, sleep for two secs then turn LED off
LED("yellow", "on")
sleep(1)
LED("yellow", "off")

# Turn on GREEN LED, sleep for three secs then turn LED off
LED("green", "on")
sleep(1)
LED("green", "off")

print "Press button for entering running lightshow..."
WaitForButtonPress()

try:
	print "entering loop"
	iteration = 1
	while True:
        	LED("red", "on")
		if SleepCheckButtonPress(0.5):
			print "Button on red on"
			break
	        LED("yellow", "on")
		if SleepCheckButtonPress(0.5):
			print "Button on yellow on"
			break
	        LED("green", "on")
		if SleepCheckButtonPress(0.5):
			print "Button on green on"
			break
	        LED("red", "off")
		if SleepCheckButtonPress(0.5):
			print "Button on red off"
			break
	        LED("yellow", "off")
		if SleepCheckButtonPress(0.5):
			print "Button on yellow off"
			break
	        LED("green", "off")
		if SleepCheckButtonPress(0.5):
			print "Button on green off"
			break
		print "iteration# ", iteration
		iteration = iteration + 1

	# Loop terminated - flash all LED's briefly and power them off before leaving program
	print "initiating program termination sequence..."
	TerminationSequence()

except KeyboardInterrupt:
	# Handle CTRL+C graceful exit
	print "Keyboard Interrupt Exception occurred - leaving!"

except:
	# All other exceptions goes here
	print "Exception occurred - leaving!"

finally:
	GPIO.cleanup()
