# Functional description
#  Startup - all LED's are off.
#  Wait 3 secs. Red LED will light up for one second, then turn off. Yellow and green LED's will follow same pattern.
#  Wait 2 secs. Start lightshow sequence, turning on red, yellow, green then turning off red, yellow, green. 
#  Repeats this pattern for 10 secs.
#  Initiate shutdown sequence blinking all three LED's in desired pattern.
#
# Physical description
# Red LED: GPIO16
# Yellow LED: GPIO20
# Green LED: GPIO21
#!/usr/bin/python

LEDredGPIO    = 16
LEDyellowGPIO = 20
LEDgreenGPIO  = 21

# Get Raspberry Pi GPIO library
import RPi.GPIO as GPIO

# Get only sleep module from time library
from time import sleep 

# Set up GPIO using BCM numbering 
def ConfigureGPIO():
	# To switch off the "Ports already in use" warnings
	GPIO.setwarnings(False)
	
	GPIO.setmode(GPIO.BCM) 
	GPIO.setup(LEDredGPIO, GPIO.OUT)
	GPIO.setup(LEDyellowGPIO, GPIO.OUT)
	GPIO.setup(LEDgreenGPIO, GPIO.OUT)
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

sleep(2)

try:
	print "Running lightshow"
	state = 1
	sleeptime_s = 0.1
	timeleft_s = 10
	
	while (timeleft_s > 0):
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
		print "time left [s] ", timeleft_s
		sleep(sleeptime_s)
		timeleft_s = timeleft_s - sleeptime_s

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
