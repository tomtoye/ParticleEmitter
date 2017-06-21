//==============================================
//File: Particle.h
//Last Updated By: Thomas Toye
//Date Created: 10/11/16
//Last Updated 15/12/16
//Brief:This is the header file for the particle class
//the purpose of the particle class is to hold all the relevant data for each particle
//its position, movement vectors, colour ect...
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

#pragma once

#ifndef __PARTICLE_H_
#define __PARTICLE_H_


#include <glm/glm.hpp>


class Particle
{

public:
	
	//constructor for spawning new particle at emitter
	Particle(const float& a_lifeTime, const glm::vec3& a_position, const glm::vec3& a_particleDir, const float& a_speed);// , glm::vec4& a_startColour, glm::vec4& a_endColour);

	//deconstructor
	virtual ~Particle();

	//Update particle, bool for if its alive
	bool UpdateAlive(const float& a_deltaTime);

	//get set pos
	glm::vec3 GetPosition() const;
	void setPosition(glm::vec3 a_position);
	
	//set lifeleft
	void setLifeLeft(float a_lifeLeft);

	float GetLifePercentage(); //lifeleft/lifetotal


private:

	float m_lifeLeft; //current life
	float m_lifeTime; //total life

	glm::vec3 m_position; //current position
	glm::vec3 m_direction; //current direction

	glm::vec2 m_size; //size of billboard

	float m_speed; //speed of particle

};



#endif	//__PARTICLE_H_
