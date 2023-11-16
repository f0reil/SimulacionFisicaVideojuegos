#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "Particle.h"
#include "Firework.h"
#include "ParticleSystem.h"
#include "GravityForceGenerator.h"
#include "ParticleDragGenerator.h"
#include "WhirlwindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "ecs.h"
#include <iostream>

std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

// Practica proyectiles
Particle* pParticle = nullptr;
std::vector <Particle*> pParticles;

// Sistema de particulas
ParticleSystem* pSystem = nullptr;
// Generador uniforme
UniformParticleGenerator* uGenerator = nullptr;
// Generador Gaussiano
GaussianParticleGenerator* gausGenerator = nullptr;
// Generador Gaussiano para firework
GaussianParticleGenerator* gausFireworkGenerator = nullptr;

// GENERADORES DE FUERZA -------------------------------------------- 
GravityForceGenerator* gravityForceGenerator = nullptr;
ParticleDragGenerator* particleDragGenerator = nullptr;
WhirlwindForceGenerator* whirlWindGenerator = nullptr;

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	GetCamera()->getTransform().rotate(Vector3(0, 0, 0));

	// SISTEMA DE PARTÍCULAS-------------------------------
	pSystem = new ParticleSystem();

	// GENERADOR UNIFORME-----------------------------------
	/*uGenerator = new UniformParticleGenerator("UniformGenerator", { 0,0,500 }, { 50,1,50 }, { 0,30,0 }, {1,10,1});
	auto modelU = models::modelsUniform[0];
	Particle* a = new Particle({ 0,0,0 }, Vector3(0, 0, 0),
		{ 0,0,0 }, { 0,0,0 }, modelU.damping, 10.0, 9.8, 4.0, modelU.scale, modelU.color);
	uGenerator->setParticle(a, false);
	a->eraseVisualModel();
	pSystem->addGenerator(uGenerator);*/

	// GENERADOR GAUSSIANO--------------------------------------
	gausGenerator = new GaussianParticleGenerator("GaussianGenerator", { 0,0,0 }, { 1,50,1 }, { 10,5,10 });
	auto modelG = models::modelsGuassian[0];
	Particle* g = new Particle({0,0,0}, Vector3(0, 0, 0),
		{0,0,0}, { 0,0,0 }, modelG.damping, 10.0, 9.8, 4.0, modelG.scale, modelG.color);
	g->eraseVisualModel();
	gausGenerator->setParticle(g, false);
	pSystem->addGenerator(gausGenerator);

	// GENERADOR GAUSSIANO PARA FIREWORKS------------------------
	// DESCOMENTAR EL GENERADOR DESEADO Y AÑADIRLO AL SISTEMA DE PARTICULAS ------------------------
	/*gausFireworkGenerator = new GaussianParticleGenerator("GaussianFireworkGenerator", { -100,0,0 }, { 1,50,1 }, { 1,10,1 }, 3, true);
	auto model = models::modelsFirework[0];
	Firework* p = new Firework(5,{ 100,100,100 }, Vector3(0, 0, 0),
		{ 0,0,0 }, { 0,0,0 }, model.damping, 1.0, 9.8, 0.5, model.scale, model.color);
	gausFireworkGenerator->setParticle(p, false);
	p->eraseVisualModel();
	pSystem->addGenerator(gausFireworkGenerator);*/

	// GENERADOR FUERZA GRAVITATORIA
	gravityForceGenerator = new GravityForceGenerator(Vector3(0,-19.8,0));
	pSystem->addForceGenerator(gravityForceGenerator);

	// GENERADOR VIENTO
	//particleDragGenerator = new ParticleDragGenerator(Vector3(0,0,200),Vector3(-20,100,0), Vector3(6000, 500, 1000), 1, 0);
	//pSystem->addForceGenerator(particleDragGenerator);

	// GENERADOR TORBELLINO
	//whirlWindGenerator = new WhirlwindForceGenerator( 100,Vector3(0, 0, 200), Vector3(5, 0, 0), Vector3(6000, 3000, 1000), 1, 0);
	//pSystem->addForceGenerator(whirlWindGenerator);

	// GENERADOR EXPLOSION CON INPUT --> ver en metodo keypress
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	// UPDATE SISTEMA DE PARTICULAS---------------------------------------------------------------
	pSystem->update(t);

	gScene->simulate(t);
	gScene->fetchResults(true);

}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	// DESTRUCTORA SISTEMA DE PARTICULAS (elimina generadores y particulas)-------------------------
	pSystem->~ParticleSystem();

	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	case ' ':
	{
		// Dispara el primer Firework
		if (gausFireworkGenerator != nullptr)
		{
			pSystem->shootFirework();
		}
		break;
	}
	case 'C':
	{
		// Añade un generador de explosiones con input
		pSystem->addExplosion();
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}