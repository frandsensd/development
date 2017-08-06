# Functional description
#  Startup - nothing happens. All LED's are off.
#  Press button first time, red LED will light up for one second, then turn off. Yellow and green LED's will follow same pattern.
#  Press button second time will start lightshow sequence, turning on red, yellow, green then turning off red, yellow, green. Repeats this pattern.
#  Press button third time, will initiate shutdown sequence blinking all three LED's in desired pattern.
#
# Physical description
# Button: GPIO26, button press is active LOW! It's pulled to high through R1+R2.
# Red LED: GPIO16
# Yellow LED: GPIO20
# Green LED: GPIO21
#!/usr/bin/python

LEDredGPIO    = 16
LEDyellowGPIO = 20
LEDgreenGPIO  = 21
ButtonGPIO    = 26
ButtonPressed = False

# Get Raspberry Pi GPIO library
import RPi.GPIO as GPIO

# Get only sleep module from time library
from time import sleep 

# Set up GPIO using BCM numbering 
def ConfigureGPIO():
	# To switch off the "Ports already in use" warnings
	GPIO.setwarnings(False)
	
	GPIO.setmode(GPIO.BCM) 
	GPIO.setup(ButtonGPIO, GPIO.IN)
	GPIO.setup(LEDredGPIO, GPIO.OUT)
	GPIO.setup(LEDyellowGPIO, GPIO.OUT)
	GPIO.setup(LEDgreenGPIO, GPIO.OUT)

	#configure for button events (interrupts) on falling edge
	GPIO.add_event_detect(ButtonGPIO, GPIO.FALLING)
	GPIO.add_event_callback(ButtonGPIO, ButtonEventCallback)
	return

#
# Function to handle button events
#
def ButtonEventCallback(pin):
	global ButtonPressed
	print "Button event"
	ButtonPressed = True
	return

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

ConfigureGPIO()

# Wait here until key is pressed 1st time
print "Press button for testing LEDs..."
ButtonPressed = False
while (ButtonPressed == False):
	sleep(0.0001)

print "Running LED test..."

#
# LED test
#
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

# Wait here until key is pressed 2nd time
print "Press button for running lightshow..."
ButtonPressed = False
while ButtonPressed == False:
	sleep(0.001)

try:
	print "Running lightshow"
	iteration=1
	state = 1
	ButtonPressed = False
	while (ButtonPressed == False):
        	if state == 1:
			LED("red", "on")
			state = 2
        	elif state == 2:
		        LED("yellow", "on")
			state = 3
	        elif state == 3:
			LED("green", "on")
			state = 4
	        elif state == 4:
			LED("red", "off")
			state = 5
		elif state == 5:
		        LED("yellow", "off")
			state = 6
		elif state == 6:
	        	LED("green", "off")
			state = 1
		print "iteration# ", iteration
		sleep(0.5)
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
	GPIO.remove_event_detect(ButtonGPIO)
	GPIO.cleanup()
