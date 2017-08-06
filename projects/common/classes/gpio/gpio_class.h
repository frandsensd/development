#pragma once

/* GPIO Class
* Purpose: Each object instatiated from this class will control a GPIO pin
* The GPIO pin number must be passed to the overloaded class constructor
*/

typedef int				GPIO_NUMBER;	// pin#
typedef unsigned int	GPIO_VALUE;		// value for pin#

typedef enum	GPIO_DIRECTION
{
	GPIO_DIRECTION_UNKNOWN,
	GPIO_DIRECTION_IN,					// read
	GPIO_DIRECTION_OUT,					// write
	GPIO_DIRECTION_MAX
}	GPIO_DIRECTION;

typedef enum GPIO_CONFIG_PULL
{
	GPIO_CONFIG_PULL_UNKNOWN,
	GPIO_CONFIG_PULL_UP,
	GPIO_CONFIG_PULL_DOWN,
	GPIO_CONFIG_PULL_MAX
}GPIO_CONFIG_PULL;

typedef enum GPIO_RESULT
{
	GPIO_RESULT_OK,
	GPIO_RESULT_ERROR,
	GPIO_RESULT_MAX
}GPIO_RESULT;



class GPIO
{
public:
	GPIO(GPIO_NUMBER aGpioNumber, GPIO_DIRECTION aDirection, GPIO_CONFIG_PULL aPullConfig);
	~GPIO();
	GPIO_RESULT			GetGpioNumber(GPIO_NUMBER& aGpioNum);
	GPIO_RESULT			Read(GPIO_VALUE& aGpioValue);
	GPIO_RESULT			Write(GPIO_VALUE aGpioValue);
private:
	GPIO_RESULT			InitializeGpio();
	GPIO_RESULT			DeinitializeGpio();
	GPIO_NUMBER			gpioNumber;
	GPIO_DIRECTION		direction;
	GPIO_CONFIG_PULL	pullConfig;
};
