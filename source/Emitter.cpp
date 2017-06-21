//==============================================
//File: Emitter.cpp
//Last Updated By: Thomas Toye
//Date Created: 10/11/16
//Last Updated 16/12/16
//==============================================

#include "Emitter.h"
#include "Gizmos.h"
#include "glm\ext.hpp"
#include <iostream>

//constructor
Emitter::Emitter(const glm::vec3 & a_position, float a_particleSize, int a_spreadFactor, float a_lifeMin, float a_lifeMax, glm::vec4 a_startColour, glm::vec4 a_endColour)
{
	m_position = a_position;

	m_particleSize = a_particleSize;

	m_spreadFactor = a_spreadFactor;

	m_emitLifeMin = a_lifeMin;

	m_emitLifeMax = a_lifeMax;

	m_startColour = a_startColour;

	m_endColour = a_endColour;
}




//deconstructor
Emitter::~Emitter(){}

void Emitter::Update(float a_deltaTime, glm::mat4& a_camera)
{
	//set all array values to deltatiime, so we have a __m256 of delta time, we can use later
	for (int i = 0; i < MAX_FLOATINSIMD; i++)
	{
		arraydTime[i] = a_deltaTime;
	}


	int iemitParticleFreq = 0; //reset frequency counter //counter for how many particles we emitted in emit loop


	for (int i = 0; i < MAX_PARTICLES; i++) //for each particle (going through array values as we need to check each particle indivdually)
	{
		if (particleStruct.arrayLifeLeft[i] <= 0) //if particle no longer alive
		{
			if (iemitParticleFreq <= m_imaxEmitParticleFreq) //and we have not exceeded our frequency checker
			{

			//reset/repsawn particle

			glm::vec3 spread = glm::vec3(glm::linearRand(-m_spreadFactor, m_spreadFactor), m_spreadFactor, glm::linearRand(-m_spreadFactor, m_spreadFactor)); //get spread vector
			spread = glm::normalize(spread); //get (normalised) x y z step for this particle

			m_emitLife = glm::linearRand(m_emitLifeMin, m_emitLifeMax); //get life for this particle


			//float speed = glm::linearRand(1.f, 1.f); //get speed for this particle


			particleStruct.arrayX[i] = m_position.x; particleStruct.arrayY[i] = m_position.y; particleStruct.arrayZ[i] = m_position.z; //reset position to emitter position
			particleStruct.arrayXStep[i] = spread.x; particleStruct.arrayYStep[i] = spread.y; particleStruct.arrayZStep[i] = spread.z; //set our step values (calculated from spread)
			particleStruct.arrayLifeTotal[i] = m_emitLife; //set life total value
			particleStruct.arrayLifeLeft[i] = m_emitLife; //set life remaing value


			//particleStruct.arraySpeed[i] = speed; //set speed (not using yet)

			iemitParticleFreq++; //increment particlefreq counter only if new particle was created

			}
			
		}
	}






	//actually utilising the benefits of simd here
	//update each particles position and life
	for (int i = 0; i < m_maxSimDParticles; i++)
	{
		__m256 tempXStep = _mm256_mul_ps(particleStruct.xStep[i], dTime); //distance x has moved by
		particleStruct.xPos[i] = _mm256_add_ps(particleStruct.xPos[i], tempXStep); //new x position
	
		__m256 tempYStep = _mm256_mul_ps(particleStruct.yStep[i], dTime); //distance y has moved by
		particleStruct.yPos[i] = _mm256_add_ps(particleStruct.yPos[i], tempYStep); //new y position
	
		__m256 tempZStep = _mm256_mul_ps(particleStruct.zStep[i], dTime); //distance z has moved by
		particleStruct.zPos[i] = _mm256_add_ps(particleStruct.zPos[i], tempZStep); //new z position
	
		particleStruct.lifeLeft[i] = _mm256_sub_ps(particleStruct.lifeLeft[i], dTime); //new life remaining value
	
	}



	//============================================================================================================================================================================================
	//So here is where ideally i wanted to implement multithreading, because drawing all the billboards is the most cpu intensive thing in the update cycle (if we do everything but draw billboards) 
	//our fps was a solid 60, however i have been unable to successfully implement multithreading correctly as i get errors where each thread is trying to read/access the same data
	//i tried mutex to prevent this however i was unsuccesfull
	//
	//So below is the two ways i can do this currently
	// 1 : as a massive loop going through each particle in array
	// 2 : use multithreading however we have to make sure each thread finishes before the next begins (if i find a way to fix error can use this again)
	//============================================================================================================================================================================================



	//============================================================
	//without multithreading
	//============================================================
	//for (int i = 0; i < MAX_PARTICLES; i++) //for each particle (going through array values as we need to check each particle indivdually)
	//{
	//	//with this we are not going to draw the first frame of each particle as it was created in if statement above and now we in else but putting this in seperate loop, was kinda pointless
	//	//and if we really wanted to we could put a draw in the if statement above, but drawing a bunch of particles in same starting point is kinda pointless, wont look different
	//
	//	glm::vec4 tempColour = glm::mix(m_endColour, m_startColour, particleStruct.arrayLifeLeft[i] / particleStruct.arrayLifeTotal[i]); //calculate colour based on life of particle
	//	glm::vec3 tempPosition = glm::vec3(particleStruct.arrayX[i], particleStruct.arrayY[i], particleStruct.arrayZ[i]); //send new position of particle
	//	Draw(tempPosition, tempColour, a_camera); //draw particle
	//	
	//}
	//============================================================


	int tmpMaxParticleForThread = MAX_PARTICLES * 0.25; //divide by 4 for 4 threads, so we can work out what chunk of particles to give to each thread
	//============================================================
	//with broken multithreading (one thread is completeting before doing next)
	//============================================================
	std::thread DrawT(&Emitter::DrawThread, this, 0, tmpMaxParticleForThread, a_camera);
	DrawT.join();
	std::thread DrawT2(&Emitter::DrawThread, this, tmpMaxParticleForThread, tmpMaxParticleForThread*2, a_camera);
	DrawT2.join();
	std::thread DrawT3(&Emitter::DrawThread, this, tmpMaxParticleForThread * 2, tmpMaxParticleForThread * 2 + tmpMaxParticleForThread, a_camera);
	DrawT3.join();
	std::thread DrawT4(&Emitter::DrawThread, this, tmpMaxParticleForThread * 2 + tmpMaxParticleForThread, MAX_PARTICLES, a_camera);
	DrawT4.join();
	//============================================================
	//use these joins and comment the ones above out if u want to see my issue
	//DrawT.join();
	//DrawT2.join();
	//DrawT3.join();
	//DrawT4.join();





	std::cout << " FPS: " << 1/a_deltaTime << std::endl; //fps counter

}


//draw billboard function
void Emitter::Draw(glm::vec3& a_position, glm::vec4& a_colour, glm::mat4& a_camera)
{
			Gizmos::billboard(a_position, a_colour, m_particleSize, a_camera);
}



//multithread function
void Emitter::DrawThread(int startI, int endI, glm::mat4& a_camera)
{
	//DrawMute.try_lock();
	for (int i = startI; i < endI; i++) //for each particle (going through array values as we need to check each particle indivdually)
	{
		glm::vec4 tempColour = glm::mix(m_endColour, m_startColour, particleStruct.arrayLifeLeft[i] / particleStruct.arrayLifeTotal[i]); //calculate colour based on life of particle
		glm::vec3 tempPosition = glm::vec3(particleStruct.arrayX[i], particleStruct.arrayY[i], particleStruct.arrayZ[i]); //send new position of particle
		Draw(tempPosition, tempColour, a_camera);
	}
	//DrawMute.unlock();
}
