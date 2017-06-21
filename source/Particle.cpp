//==============================================
//File: Particle.cpp
//Last Updated By: Thomas Toye
//Date Created: 10/11/16
//Last Updated 15/12/16
//Brief:This is the source file for the particle class
//the purpose of the particle class is to hold all the relevant data for each particle
//its position, movement vectors, colour ect...
//we also have an update function in here that can be called for each particle
//where we set its new position and determine its health for colour in draw function
//==============================================


//==============================================
//we are no longer using our particle .h or .cpp
//as we are now using SIMD and for that we have
//a particle struct in our emitter class with 
//all the __m256 data types in unions with arrays 
//so we can still access individual particles values
//i have left them here for reference and to show
//how work has progressed
//==============================================



#include "Particle.h"
//#include <iostream>


//constructor 
Particle::Particle(const float & a_lifeTime, const glm::vec3 & a_position, const glm::vec3 & a_particleDir, const float & a_speed)// , glm::vec4 & a_startColour, glm::vec4 & a_endColour)
{
	m_lifeTime = a_lifeTime; //set total life of particle
	m_lifeLeft = a_lifeTime; //initialise life counter to total life when particle is created
	m_position = a_position; //set position at what will be emitter spawn
	m_direction = a_particleDir; //give direction from possible value of our cone spread
	m_speed = a_speed; //give its speed
}

//deconstructor
Particle::~Particle()
{

}

//update position and colour of billboard
bool Particle::UpdateAlive(const float & a_deltaTime)
{
	if (m_lifeLeft > 0) //if particle still alive
	{
		//set and life pos

		setLifeLeft(m_lifeLeft - a_deltaTime);
		setPosition(m_position + m_direction * m_speed * a_deltaTime);
		
		return true; //tell them this particle still alive
	}
	else //kill it
	{
		setLifeLeft(0); //incase it went negative
		return false; //tell them we dead
	}
}


//get set position
glm::vec3 Particle::GetPosition() const
{
	return m_position;
}
void Particle::setPosition(glm::vec3 a_position)
{
	m_position = a_position;
}

void Particle::setLifeLeft(float a_lifeLeft)
{
	m_lifeLeft = a_lifeLeft;
}

float Particle::GetLifePercentage()
{
	return m_lifeLeft/m_lifeTime;
}
