#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "Particle.h"
#include "RigidSolid.h"
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
#include "estados.h"

std::string display_text = "Campo de Tiro - Proyecto Final, Paula Lopez";

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


// SOLIDO RIGIDO ------------------------------------------------------
// Suelo
physx::PxRigidStatic* _staticFloor = nullptr;
RenderItem* floorRenderItem;

// PROYECTO FINAL ---------------------------------------------------
// Variables flujo de juego
Estados estadoJuego = Intro;
int score = 0;
// Sistema de particulas
ParticleSystem* pSystem = nullptr;
// Generadores de fuerza -------------------------------------------- 
GravityForceGenerator* gravityForceGenerator = nullptr;
// Lista de balas
std::list<Entity*> bulletList;
// Cooldown balas
bool canShoot = false;
float cooldownShoot = 0.25f;
float timer = 0.0f;
// Tiempo de juego
const double tiempoPartida = 10;
double tiempoRestante = tiempoPartida;

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

}

void setupGaussianGenerator()
{
	// Generador uniforme
	UniformParticleGenerator* uGenerator = nullptr;
	uGenerator = new UniformParticleGenerator("UniformGenerator", { 0,-20,-100 }, { 150,1,30 }, { 0,35,0 }, { 1,100,1 }, 800, false);
	auto modelU = models::modelsUniform[0];
	RigidSolid* u = new RigidSolid(Vector3(0, 10, 0), Dynamic, 10, gPhysics, gScene, 10, Box, modelU.scale, modelU.color);
	uGenerator->setParticle(u, false);
	u->eraseVisualModel();
	pSystem->addGenerator(uGenerator);
}

void createBouyancyObstacles()
{
	// Creamos los obstaculos flotantes del escenario
	pSystem->generateBuoyancyForce(gPhysics, gScene);
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	// UPDATE SISTEMA DE PARTICULAS---------------------------------------------------------------
	pSystem->update(t, bulletList, score);
	
	// Si estamos en el estado de la partida, vamos disminuyendo el tiempo
	if (estadoJuego == Juego)
	{
		tiempoRestante -= t;
		// COOLDOWN BALAS -----------------------------------------
		if (canShoot == false) {
			timer += t;
			// Cuando el timer iguale al tiempo del cooldown, reseteamos timer y activamos disparo
			if (timer >= cooldownShoot) {
				timer = 0;
				canShoot = true;
			}
		}
	}
	// Cuando la partida termina, reseteamos el tiempo, borramos las instancias y lanzamos fuegos artificales
	if (tiempoRestante <= 0)
	{
		tiempoRestante = tiempoPartida;
		estadoJuego = Final;
		pSystem->deleteParticles();
		pSystem->Fireworks();
	}
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
			break;
		default:
			break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);

	// Comprobamos las colisiones de las balas con las entidades del juego
	pSystem->checkColissionsWithBullets(score, actor1, actor2, bulletList);
}

void shoot()
{
	// Desactivamos disparo
	canShoot = false;
	// Creamos bala
	RigidSolid* bullet = new RigidSolid({ 0,0,0 }, Dynamic, 1, gPhysics, gScene, 5, Sphere, 1, {200,200,0,1});
	bullet->setVel({ (GetCamera()->getMousePos().x / 5) * 500,
		(GetCamera()->getMousePos().y / 5) * 500, -1 * 500 });
	bulletList.push_back(bullet);
	// Añadimos la bala al sistema de particulas
	pSystem->addParticleToList(bullet);
}

void mouseInput(int button, int state, int x, int y)
{
	// Si pulsamos click izquierdo
	if (button == 0)
	{
		switch (estadoJuego)
		{
		case Intro:
			// Al iniciar, para pasar al siguiente estado habrá que pulsar el click izq
			estadoJuego = Juego;
			// Creamos el escenario del juego
			setupGaussianGenerator();
			createBouyancyObstacles();
			break;
		case Juego:
			// Activamos el sistema de particulas si está desactivado
			if (!pSystem->getActive())
			{
				pSystem->setActive(true);
				setupGaussianGenerator();
			}
			// Si podemos disparar, añadimos una bala a la lista de balas
			if (canShoot) 
			{
				shoot();
			}
			break;
		case Final:
			// Reiniciamos juego
			estadoJuego = Juego;
			canShoot = false;
			break;
		default:
			break;
		}
	}
	PX_UNUSED(state);
	PX_UNUSED(button);
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