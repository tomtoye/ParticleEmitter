//==============================================
//File: MyApplication.h
//Last Updated By: Thomas Toye
//Date Created: 10/11/16
//Last Updated 16/12/16
//Brief:base class from which all are called
//==============================================


#ifndef __Application_H_
#define __Application_H_

#include "Application.h"
#include <glm/glm.hpp>
#include "Emitter.h"

// Derived application class that wraps up all globals neatly
class MyApplication : public Application
{
public:

	MyApplication();
	virtual ~MyApplication();

protected:

	virtual bool onCreate(int a_argc, char* a_argv[]); //create all the things

	virtual void onUpdate(float a_deltaTime); //update all the things

	virtual void onDraw(); //draw all the things

	virtual void onDestroy(); //kill all the things


	Emitter* emitter;


	glm::mat4	m_cameraMatrix;
	glm::mat4	m_projectionMatrix;



};

#endif // __Application_H_