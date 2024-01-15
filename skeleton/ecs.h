#pragma once
#include <vector>
namespace proyectil
{
	const float g = -9.8;
	struct proyectilP
	{
		Vector3 vReal;
		Vector3 vSim;
		Vector3 a;
		float m ;
		float d ;
	};

	static proyectilP generico = { Vector3(0, 0, 0), Vector3(-10,10,-10), Vector3(0,10,0), 0, 1};

	static proyectilP bala = { Vector3(0, 0, -10),Vector3(0, 0, -3), Vector3(0,0,0), 0.2, 0.998 };
	
	static proyectilP canion = {Vector3(1800, 0, 0),Vector3(200, 0, 0), Vector3(0,0,0), 2500, 0.998};
}

namespace models
{
	struct modelInfo
	{
		double damping;
		Vector4 color;
		int scale;
	};

	static modelInfo uniform1 = { 0.98, {1,0,1,1} , 5};
	static modelInfo uniform2 = { 0.5, {0.4, 0.4, 0.8, 1} };
	static modelInfo gaus1 = { 0.98, {0, 0.2, 1, 1}, 1 };
	static modelInfo gaus2 = { 0.7, {0, 0.1, 0.8, 1}, 1 };
	static modelInfo firework1 = { 0.98, {1, 0.2, 0.2, 1}, 2 };
	static modelInfo firework2 = { 0.98, {1, 0.1, 0.5, 1}, 1 };

	static std::vector<modelInfo> modelsUniform = { uniform1, uniform2 };
	static std::vector<modelInfo> modelsGuassian = { gaus1, gaus2 };
	static std::vector<modelInfo> modelsFirework = { firework1, firework2 };
}