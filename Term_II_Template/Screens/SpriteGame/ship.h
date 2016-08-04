#pragma once
#include "string.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "..\..\Game Setup\Helper.h"
#include "..\..\Game Setup\sdlWindow.h"
#include "..\..\Game Setup\sdlResources.h"
#include "../../Game Setup/Helper.h"
#include "../../Node.h"
#include "Include\utility.h"
#include <fstream>
#include <vector>


using namespace Helper;
enum ResourceType
{
	Empty, Home, Metal, Fuel, Depleted
};
class ship
{
public:
	ship();
	ship(SDL_Texture* a_tex,float a_x, float a_y, float a_width, float a_height, int a_frameWidth, int a_frameHeight, int a_frameX, int a_frameY,
		bool a_pcontrol);
	~ship();
	enum Wepname
	{
		MG, Pulse, Lazer, Missile
	};
	struct Weapons
	{
		string name;
		int attack;
		SDL_Texture* Projectile;
		int ProjectileNo;
		float Velocity;
		int Firespeed;
		float X;
		float Y;
		int ammo;
	};

	void SetShip(float a_topspeed, int a_weapon, int a_ammo, float a_health, int a_attack);
	void Moveship(Vector3 a_Direction);
	void DrawShip(); 
	void UpdateShip(Matrix3 p_mat);
	void ShipPos(float a_x, float a_y);
	void KeyPress();
	void Steering();
	void Collect();
	int Deposit();
	void SetHome(int a_home);
	void Saveship();
	bool LoadShip();
	void SetTopSpeed(float a_speed); 
	void SetTarget(vector<Node*>* a_node);

	void SetDestination(int a_dest);

	void SetDeposits(float a_deliverypointA, float a_deliverypointB);

	int ReturnAngle();
	int Destination;
	bool AtDestination;

	bool X; 
	bool Y;


	vector <Node*> Path;
	Node* StartDes;
	Node* EndDes;



	int fuel;
	int m_Home;
	float Hull;
	bool Idle; 
	bool FirstNode;
	Matrix3 ShipMat;
	SDL_Texture* m_tex;
	bool Pathing;
	ResourceType	m_rtype;	
	int			m_cargo;
protected:
	string		 m_name;
	Matrix3		 Translation;
	Matrix3		 Rotation;
	Matrix3		 Scale;
	Matrix3		 LocalSpace = Scale * Rotation * Translation;
	Vector3		 m_Position;
	SDL_Rect	 Dest;
	vector<Node*>*	Target;

	int			m_cargo_capacity;


	int			 hullDegrade;
	bool		 m_save;
	bool		 m_KeyControl;
	float		 m_angle;
	float		 m_width;
	float		 m_height;
	float		 m_x;
	float		 m_y;
	float		 m_TopSpeed;
	float		 m_Decellerate;
	float		 m_velocity;

	float		 m_health;
	float		 m_attack;
	Weapons		 m_weapon;

	int			 m_framewidth;
	int			 m_frameheight;
	int			 m_FrameX;
	int			 m_FrameY;
	float		 m_delay;

	float		 *m_deliveryPointA;
	float		 *m_deliveryPointB;



};

class ChildShip : public ship
{
public:
	ChildShip( float a_x, float a_y, float a_width, float a_height, int a_frameWidth, int a_frameHeight, int a_frameX, int a_frameY);

	void UpdateShip(Matrix3 p_world, Matrix3 p_parent);

private:
	Matrix3 *m_Pmat;
	float MX;
	float MY;
};