#pragma once

namespace proyectil
{
	const float g = 9.8;
	struct proyectilP
	{
		Vector3 vReal;
		Vector3 vSim;
		Vector3 a;
		float m ;
		float d ;
	};

	proyectilP bala = { Vector3(300, 0, 0),Vector3(80, 0, 0), Vector3(0,0,0), 0.2, 0.998 };
	
	proyectilP canion = {Vector3(1800, 0, 0),Vector3(200, 0, 0), Vector3(0,0,0), 2500, 0.998};
}