#include <Singleton.h>

//The instance which never changes - accessible by all.
Singleton* Singleton::_instance = 0;


Singleton* Singleton::Instance()
{
	if (_instance == 0) {
		_instance = new Singleton;
	}
	return _instance;
}


Singleton::~Singleton()
{
	if (_instance) {
		delete _instance;
		_instance = NULL;
	}
}
