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

##
##
##
## Python Key Lines:
##
##
##
# Python start signature:
# #!/usr/bin/python
# Constants used:
# LEDredGPIO    = 16
# LEDyellowGPIO = 20
# LEDgreenGPIO  = 21
# ButtonGPIO    = 26
#
# Get Raspberry Pi GPIO library:
# import RPi.GPIO as GPIO
# Set up GPIO using BCM numbering using a function:
# def ConfigureGPIO():
#  To switch off the "Ports already in use" warnings
#  GPIO.setwarnings(False)
#  GPIO.setmode(GPIO.BCM) 
#  GPIO.setup(ButtonGPIO, GPIO.IN)
#  GPIO.setup(LEDredGPIO, GPIO.OUT)
#  GPIO.setup(LEDyellowGPIO, GPIO.OUT)
#  GPIO.setup(LEDgreenGPIO, GPIO.OUT)
#
# Configure for button events (interrupts). Callback running in separate thread:
#	GPIO.add_event_detect(ButtonGPIO, GPIO.FALLING)
#	GPIO.add_event_callback(ButtonGPIO, ButtonEventCallback)
#	GPIO.remove_event_detect(ButtonGPIO)
#
# Define LEDredGPIO states:
# GPIO.output(LEDredGPIO, GPIO.HIGH)
# GPIO.output(LEDredGPIO, GPIO.LOW)
#
# Reset GPIO states to default:
#	GPIO.cleanup()
