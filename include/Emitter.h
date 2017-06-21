//==============================================
//File: Emitter.h
//Last Updated By: Thomas Toye
//Date Created: 10/11/16
//Last Updated 16/12/16
//Brief:the purpose of the class is to hold all the relevant data for each particle
//its position, movement vectors, colour ect... in a struct with unions to take advantage of SIMD
//we also then hold everything we need to create an emitter to spawn those particles
//==============================================

#pragma once

#ifndef __EMITTER_H_
#define __EMITTER_H_

#include <glm/glm.hpp> //needed for vec 3 and what not. All glm stuff
#include <thread> //multithreading
#include <mutex>
#include <immintrin.h> //SIMD
//#include <emmintrin.h> old SIMD

#define MAX_PARTICLES 16384//how many particles we have max
#define MAX_SIMDPARTICLES MAX_PARTICLES / 8 //how many __m256 particles we have (because we can split 8 floats into each)
#define MAX_FLOATINSIMD 8 //how many floats we fit into each __m256

class Emitter
{

public:

	void DrawThread(int startI, int endI, glm::mat4 & a_camera); //multithread function

	//constructor
	Emitter(const glm::vec3& a_position, float a_particleSize, int a_spreadFactor, float a_lifeMin, float a_lifeMax, glm::vec4 a_startColour, const glm::vec4 a_endColour);

	//deconstructor
	virtual ~Emitter();

	//update all particles and emit new ones
	virtual void Update(float a_deltaTime, glm::mat4& a_camera);

	//draw given particle at this position, colour and to face camera
	virtual void Draw(glm::vec3& a_position, glm::vec4& a_colour, glm::mat4& a_camera);

	

	//virtual void EmitParticles(int a_particleCount);
	//virtual void setSIMDPART(float a_max);
	//Particle* GetParticle(const int& a_values);
	//glm::vec4 getRandomDirection();

	

private:

	

	//int m_iemitParticleFreq = 0; //counter for how many particles we emitted in emit loop
	const int m_imaxEmitParticleFreq = 512; //counter for how many particles we emitted in emit loop

	//int m_particleCount;

	std::mutex DrawMute; //if we wanna lock data in multithread


	float m_particleSize = 0.1f; //size of each billboard/particle

	int m_spreadFactor = 128; //how much spread can randomize in normalise. eg if we have 100 then particles can spawn -100 to 100 positions on an axis

	float m_emitLife; //life given to each particle
	//life is decided by gettin linear rand between the following values
	float m_emitLifeMin = 2.f;
	float m_emitLifeMax = 4.f;


	const float m_maxSimDParticles = MAX_SIMDPARTICLES; //the max size that our __m256 goes to

	glm::vec3 m_position; //emitter pos

	glm::vec4 m_endColour = glm::vec4(0.5, 0.5, 0.5, 0.1);

	glm::vec4 m_startColour = glm::vec4(0.1, 0.1, 0.1, 0.5);


	//because we are using floats for each pos in vert and dtime and so on, in simd which is 256 bytes
	//we can only use 256/32 (float is 32bytes) = 8 floats in each __m256

	//this is a union where all 8 values in it will equal delta time, so we can use dtime in simd calculations
	union
	{
		__m256 dTime;

		float arraydTime[8];
	};

	//==============================================
	//as we are now using SIMD and for that we have
	//a particle struct in our emitter class with 
	//all the __m256 data types in unions with arrays 
	//so we can still access individual particles values
	//==============================================
	struct ParticleStruct
	{

		union //xpos
		{ 
			__m256 xPos[MAX_SIMDPARTICLES]; 
			float arrayX[MAX_PARTICLES]; 
		};

		union //xdelta
		{ 
			__m256 xStep[MAX_SIMDPARTICLES]; 
			float arrayXStep[MAX_PARTICLES]; 
		}; 
	
		union //ypos
		{ 
			__m256 yPos[MAX_SIMDPARTICLES]; 
			float arrayY[MAX_PARTICLES]; 
		};

		union //ydelta
		{ 
			__m256 yStep[MAX_SIMDPARTICLES]; 
			float arrayYStep[MAX_PARTICLES]; 
		};
	
		union //zpos union
		{ 
			__m256 zPos[MAX_SIMDPARTICLES]; 
			float arrayZ[MAX_PARTICLES]; 
		}; 

		union //zdelta
		{ 
			__m256 zStep[MAX_SIMDPARTICLES]; 
			float arrayZStep[MAX_PARTICLES]; 
		}; 

		union //life left of particle
		{ 
			__m256 lifeLeft[MAX_SIMDPARTICLES]; 
			float arrayLifeLeft[MAX_PARTICLES]; 
		};

		union //total life of particle
		{ 
			__m256 lifeTotal[MAX_SIMDPARTICLES]; 
			float arrayLifeTotal[MAX_PARTICLES]; 
		}; 

		//for now not using speed
		//union //speed
		//{ 
		//	__m256 speed[MAX_SIMDPARTICLES]; 
		//	float arraySpeed[MAX_PARTICLES]; 
		//};

	} particleStruct;
	
	
};

#endif //__EMITTER_H_
