#pragma once
#include "ship.h"
#include <vector>

float degToRad(float a_deg)
{
	float rad = 3.14159f*a_deg / 180.0f;

	return rad;
}

void setRotateZ(Matrix3 p_parent,float a_num)
{
	p_parent.m_floats[0][0] = cosf(a_num);
	p_parent.m_floats[0][1] = sinf(a_num);
	p_parent.m_floats[1][0] = -sinf(a_num);
	p_parent.m_floats[1][1] = cosf(a_num);
}

Vector2 Normalize(Vector2 V)
{
	return V.normalised();
};

ship::ship(SDL_Texture* a_tex,float a_x, float a_y, float a_width, float a_height, int a_frameWidth, int a_frameHeight, int a_frameX, int a_frameY, 
	bool a_pcontrol)
{
	m_tex = a_tex;

	m_x = a_x;
	m_y = a_y;

	m_width		=	a_width;
	m_height	=	a_height;

	m_frameheight	=	a_frameHeight;
	m_framewidth	=	a_frameWidth;

	m_FrameX = a_frameX;
	m_FrameY = a_frameY;

	m_KeyControl = a_pcontrol;
	m_cargo_capacity = 5;
	AtDestination = false;
	
	hullDegrade = 1;

	Hull = 100;
}

ship::~ship()
{
}

void ship::SetShip(float a_topspeed, int a_weapon, int a_ammo, float a_health, int a_attack)
{
#pragma region Stats
	m_TopSpeed = a_topspeed;
	if(a_health > m_health)	m_health = a_health;
	if (a_ammo > m_weapon.ammo) m_weapon.ammo = a_ammo;
	//
	Weapons Unarmed;
	Unarmed.ammo = 0;
	Unarmed.attack = 0;
	Unarmed.Firespeed = 0;
	Unarmed.name.assign("No Weapon");
	Unarmed.Projectile = TM_FIRE;
	Unarmed.ProjectileNo = 0;
	//
	Weapons MachineGun;
	MachineGun.ammo = 100;
	MachineGun.attack = 10;
	MachineGun.Firespeed = 10;
	MachineGun.name.assign("Machine Gun");
	MachineGun.Projectile = TM_FIRE;
	MachineGun.ProjectileNo = 1;
	//
	Weapons Pulser;
	Pulser.ammo = -1;
	Pulser.attack = 25;
	Pulser.Firespeed = 15;
	Pulser.name.assign("Pulser");
	Pulser.Projectile = TM_FIRE;
	Pulser.ProjectileNo = 1;
	//
	Weapons Laser;
	Laser.ammo = -1;
	Laser.attack = 5;
	Laser.Firespeed = 9;
	Laser.name.assign("Laser");
	Laser.Projectile = TM_FIRE;
	Laser.ProjectileNo = 1;
	//
	Weapons missile;
	missile.ammo = 20;
	missile.attack = 75;
	missile.Firespeed = 2;
	missile.name.assign("Missile");
	missile.Projectile = TM_FIRE;
	missile.ProjectileNo = 1;
	//
	switch (a_weapon)
	{
	case MG:
		m_weapon = MachineGun;
		break;
	case Pulse:
		m_weapon = Pulser;
		break;
	case Lazer:
		m_weapon = Laser;
		break;
	case Missile:
		m_weapon = missile;
		break;
	default:
		m_weapon = Unarmed;
		break;
	}
	//
#pragma endregion
}

void ship::Moveship(Vector3 a_Direction)
{
	Vector3 Velocity;
	
	Velocity.x += a_Direction.magnitude();
	Velocity.y += a_Direction.magnitude();

	m_Position.x = Velocity.x;
	m_Position.y = Velocity.y;
}

void ship::KeyPress()
{
	if (HELP_Keypresses(SDL_SCANCODE_A)) m_x -= 5.0f;
	if (HELP_Keypresses(SDL_SCANCODE_D)) m_x += 5.0f;

	if (HELP_Keypresses(SDL_SCANCODE_W)) m_y -= 5.0f;
	if (HELP_Keypresses(SDL_SCANCODE_S)) m_y += 5.0f;

	if (HELP_Keypresses(SDL_SCANCODE_Z)) m_width +=  10.0f;
	if (HELP_Keypresses(SDL_SCANCODE_X)) m_width -=  10.0f;
													  
	if (HELP_Keypresses(SDL_SCANCODE_C)) m_height += 10.0f;
	if (HELP_Keypresses(SDL_SCANCODE_V)) m_height -= 10.0f;

	if (HELP_Keypresses(SDL_SCANCODE_0)) Saveship();
	if (HELP_Keypresses(SDL_SCANCODE_1)) LoadShip();
}

void ship::Steering()
{
	//static int i = 0;

	if ((*Target).size()-1 == 0)return;

	if (Destination > (int)((*Target).size() - 1))
	{
		Destination = 0;
	}
//	m_TopSpeed = 2.0f;
	float Speed = 0;
	 X = false;
	 Y = false;

	Vector2 TargetVector((*Target)[Destination]->m_x, (*Target)[Destination]->m_y);
	Vector2 V;
	Vector2 Pos(m_x, m_y);
	Vector2 m_force;
	Vector2 Velocity;


	V = Normalize(TargetVector - Pos) * m_TopSpeed ;

	m_force = V - Velocity;
	Velocity = Velocity + m_force;
	Pos = Pos + Velocity;

	m_x = Pos.x;
	m_y = Pos.y;

	m_angle = atan2(Velocity.x,-Velocity.y);
	m_angle *= 180 / (float)M_PI;
	m_angle -= 90;

	if (Pathing == false)
	{
		if (m_x <= (*Target)[Destination]->m_x + 10 && m_x >= (*Target)[Destination]->m_x - 10)
		{
			X = true;
		};

		if (m_y <= (*Target)[Destination]->m_y + 10 && m_y >= (*Target)[Destination]->m_y - 10)
		{
			Y = true;
		};
	}
	else
	{
		if (m_x <= (*Target)[Destination]->m_x + 5 && m_x >= (*Target)[Destination]->m_x - 5)
		{
			X = true;
		};

		if (m_y <= (*Target)[Destination]->m_y + 5 && m_y >= (*Target)[Destination]->m_y - 5)
		{
			Y = true;
		};
	};

	if (Pathing == false)
	{
		if (X && Y && Destination == m_Home)
		{
			Idle = true; AtDestination = true; Deposit();
		}
		else if (X && Y)
		{
			Collect(); AtDestination = true; Hull -= hullDegrade;
		};
	}
	else if (Pathing == true)
	{
		while (true)
		{
			if (Path.size() <= 0)
			{

				return;
			}
			
			Destination = Path[0]->Index;

			if (X == true && Y == true)
			{
				Path.erase(Path.begin());
				Destination = Path[0]->Index;
				X = false;
				Y = false;
			}
			else
			{
				break;
			}

		}
	};
}

void ship::Collect()
{
	if (Destination == m_Home) {	Destination = m_Home;return; }
	int i = (*Target)[Destination]->rType;
	m_rtype = (ResourceType)i;
	m_cargo = m_cargo_capacity;
	(*Target)[Destination]->m_ResourceAmount -= m_cargo_capacity;
	Destination = m_Home;
}

int ship::Deposit()
{
	int i = m_cargo;
	if (m_rtype == Metal)
	{
		(*Target)[m_Home]->ReqOne += m_cargo;
	}
	else 
	{
		(*Target)[m_Home]->ReqTwo += m_cargo;
	}
	m_cargo = 0;
	return i;
}

void ship::SetHome(int a_home)
{
	m_Home = a_home;
}

void ship::Saveship()
{
	ofstream outfile;
	outfile.open("ShipSave.txt", ios_base::out);
	if (outfile.is_open())
	{
		outfile << ShipMat.m_floats[0][0] << endl;
		outfile << ShipMat.m_floats[0][1] << endl;
		outfile << ShipMat.m_floats[0][2] << endl;
		outfile << ShipMat.m_floats[1][0] << endl;
		outfile << ShipMat.m_floats[1][1] << endl;
		outfile << ShipMat.m_floats[1][2] << endl;
		outfile << ShipMat.m_floats[2][0] << endl;
		outfile << ShipMat.m_floats[2][1] << endl;
		outfile << ShipMat.m_floats[2][2] << endl;
		outfile.close();
	}
}
		
bool ship::LoadShip()
{
	ifstream infile;
	infile.open("ShipSave.txt", ios_base::in);
		if (infile.is_open())
		{
			infile >> ShipMat.m_floats[0][0];
			infile >> ShipMat.m_floats[0][1];
			infile >> ShipMat.m_floats[0][2];

			infile >> ShipMat.m_floats[1][0];
			infile >> ShipMat.m_floats[1][1];
			infile >> ShipMat.m_floats[1][2];

			infile >> ShipMat.m_floats[2][0];
			infile >> ShipMat.m_floats[2][1];
			infile >> ShipMat.m_floats[2][2];

			infile.close();

			m_x = ShipMat.m_floats[2][0];
			m_y = ShipMat.m_floats[2][1];
		}
	return false;
}

void ship::SetTopSpeed(float a_speed)
{
	m_TopSpeed = a_speed;
}

void ship::SetTarget(vector<Node*>* a_node)
{
	Target = a_node;
}

void ship::SetDestination(int a_dest)
{
	Destination = a_dest;
	Idle = false;
}

void ship::SetDeposits(float a_deliverypointA, float a_deliverypointB)
{
	m_deliveryPointA = &a_deliverypointA;
	m_deliveryPointB = &a_deliverypointB;
}

int ship::ReturnAngle()
{
	return (int)m_angle;
}

void ship::DrawShip()
{
	HELP_DrawSprite(m_tex, m_FrameX, m_FrameY, m_framewidth, m_frameheight, ShipMat.m_floats[2][0], ShipMat.m_floats[2][1],
		Dest.w, Dest.h, (int)m_delay, (int)m_angle);
}

void ship::UpdateShip(Matrix3 p_world)
{
	ShipMat = p_world * ShipMat;
	ShipMat.m_floats[1][1] = 1;
	ShipMat.m_floats[2][0] = m_x;
	ShipMat.m_floats[2][1] = m_y;

	//m_angle += 5;
	if (m_angle > 359) m_angle = 0;
	setRotateZ(ShipMat,m_angle);

	Dest.w = (int)(m_width * p_world.m_floats[0][0]);
	Dest.h = (int)(m_height * p_world.m_floats[1][1]);

	if (m_KeyControl == true) { KeyPress(); }
	else
	{
		if (Hull > 0)
		{
			Steering();
		}
	}
	DrawShip();
}

void ship::ShipPos(float a_x, float a_y)
{
	m_x = a_x;
	m_y = a_y;
}

ship::ship()
{
	m_tex = TM_WAKE;

	m_width = 0;
	m_height = 0;

	m_frameheight = 0;
	m_framewidth = 0;

	m_FrameX= 0;
	m_FrameY = 0;
}

ChildShip::ChildShip( float a_x, float a_y, float a_width, float a_height, 
	int a_frameWidth, int a_frameHeight, int a_frameX, int a_frameY)
{


	m_x = a_x;
	m_y = a_y;

	m_width = a_width;
	m_height = a_height;

	m_frameheight = a_frameHeight;
	m_framewidth = a_frameWidth;

	m_FrameX = a_frameX;
	m_FrameY = a_frameY;

	ShipMat.m_floats[2][0] = 0;
	ShipMat.m_floats[2][1] = 0;

	MX = m_x;
	MY = m_y;
}

void ChildShip::UpdateShip(Matrix3 p_world, Matrix3 p_parent)
{
	if (m_KeyControl == true) { KeyPress(); }
	else { Steering(); }

	//ShipMat.setRotateZ(m_angle);

	//m_x = MX;
	//m_y = MY;

	Vector3 VecChild(m_x, m_y, 0);
	VecChild = p_parent * ShipMat * VecChild;

	ShipMat.m_floats[2][0] = VecChild.x;
	ShipMat.m_floats[2][1] = VecChild.y;

	m_x = VecChild.x;
	m_y = VecChild.y;

	Dest.w = (int)(m_width*p_world.m_floats[0][0]);
	Dest.h = (int)(m_height*p_world.m_floats[1][1]);

	ShipMat.m_floats[2][0] -= p_parent.m_floats[2][0];
	ShipMat.m_floats[2][1] -= p_parent.m_floats[2][1];

	DrawShip();
}