#pragma once

// Singleton Pattern Example
// Uses: Places where one and only one instance is created. Ex. Abstract Factory pattern..
// Notice only Instance() method is accessible to all
// Constructor is only accessible to the members of the class inheriting Singleton.
// The constructor here is simply default constructor. Instance() is the creating method.
class Singleton
{
public:
	static Singleton* Instance();
protected:
	Singleton();
	~Singleton();
private:
	static Singleton* _instance;
};

