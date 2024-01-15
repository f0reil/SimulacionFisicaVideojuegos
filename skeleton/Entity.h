#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>
#include <iostream>
class ParticleGenerator;

enum Geometry { Sphere, Box, Liquid, RectangleBox };

class Entity
{
public:
	Entity(double timeLife, Geometry forma = Sphere,
		float scale = 5, Vector4 color = { 255, 250, 0, 1 }, bool proyect = false, bool isHold = false, bool isFirework = false);
	virtual ~Entity();
	virtual void integrate(double t);
	virtual Entity* clone() const = 0;
	bool getGeneratesOnDeath() { return generatesOnDeath; }
	ParticleGenerator* getGenerator() { return _particle_generator; }
	double getTimeLeft() { return remaining_time; }
	inline void setReminingTime(double t) { remaining_time = t; };
	virtual bool insideLimits(Vector3 limit) = 0;
	virtual bool insideLimits(Vector3 limit, Vector3 origin) = 0;
	virtual inline physx::PxTransform getPos() = 0;
	virtual inline Vector3 getVel() = 0;
	virtual inline void setPos(Vector3 p) = 0;
	virtual inline void setVel(Vector3 v) = 0;
	virtual void death() {};
	virtual std::list<Entity*> getGeneratedParticles() = 0;
	inline void eraseVisualModel()
	{
		if (renderItem != nullptr)
		{
			renderItem->release();
			renderItem = nullptr;
		}
	}

	virtual inline void clearForce() = 0;
	virtual inline void addForce(const Vector3& f) = 0;

	inline double getMass() { return m; };
	inline double getInvMass() { return 1 / m; };

	virtual inline void setMass(double mass) = 0;

	inline int getVolumen()
	{
		int volumen;
		switch (formaP)
		{
		case Box:
			volumen = pow(scaleP, 3);
			break;
		case Sphere:
			volumen = 4 / 3 * std::atan(1) * 4 * scaleP * scaleP * scaleP;
			break;
		case RectangleBox:
			volumen = 4 / 3 * std::atan(1) * 4 * scaleP * (scaleP*2) * scaleP;
			break;
		default:
			break;
		}
		return volumen;
	};

	inline int getHeigth() { return scaleP; };

	inline bool getIsProyectil() { return proyectil; };

	inline Geometry getForma() { return formaP; };

	inline bool isFirework() { return fireworkParticle; };
protected:
	Vector4 colorP;
	RenderItem* renderItem;
	ParticleGenerator* _particle_generator = nullptr;
	Vector3 posI;

	double m;
	double remaining_time, timeI;
	float scaleP;
	bool generatesOnDeath = false, proyectil = false, hold = false;

	Geometry formaP;

	bool fireworkParticle = false;

	physx::PxShape* entShape;
};

