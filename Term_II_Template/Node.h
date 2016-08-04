#pragma once
#include <iostream>
#include "string.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "Game Setup\sdlWindow.h"
#include "Game Setup\sdlResources.h"
#include "Game Setup\Helper.h"
#include "Include\utility.h"
#include <vector>

using std::vector;
using namespace Helper;



class Node
{
public:

	Node(float a_x, float a_y, int a_cost)
	{
		m_x = a_x;
		m_y = a_y;
		m_cost = a_cost;
		MX = m_x;
		MY = m_y;
		m_ResourceAmount = 1;
		Alive = true;
		Immortal = false;
			for (int i = 0; i < 4; i++)
			{
				Connections[i] = nullptr;
			}
			pathing = false;
	}

	bool pathing = false;
	
	void SetXY(float a_x, float a_y)
	{
		m_x += a_x;
		m_y += a_y;
	}

	void SetParent(int a_parent)
	{
		m_parent = a_parent;
	}

	void SetNodeReq(int a_resource, int a_type)
	{
		m_ResourceAmount = a_resource;
		rType = (ResourceType)a_type;
	}

	bool CheckReq()
	{
		if (!m_ResourceAmount)return false;
		return true;
	}

	void DrawNode()
	{
		
		HELP_DrawSprite(m_sprite,(float)m_FrameX, (float)m_FrameY, (float)m_frameWidth, (float)m_frameHeight, NodeMat.m_floats[2][0], NodeMat.m_floats[2][1], (float)Dest.w, (float)Dest.h,(int)m_delay,m_angle);

		for (int i = 0; i < 4; i++)
		{
			if (Connections[i] != nullptr && pathing == true)
			{
				Window::setDrawColor(255, 255, 255, 255);
				Window::drawLine(NodeMat.m_floats[2][0], NodeMat.m_floats[2][1], Connections[i]->m_x, Connections[i]->m_y);
			}
		}
	}; 

	void UpdateNode(Matrix3 p_world)
	{
		if (m_ResourceAmount <= 0 && Immortal == false) { Alive = false; m_sprite = TM_DEPLETED;}
		
			NodeMat = p_world * NodeMat;
			NodeMat.m_floats[1][1] = 1;
			NodeMat.m_floats[2][0] = m_x;
			NodeMat.m_floats[2][1] = m_y;

			m_angle += m_RotSpeed;
			if (m_angle > 359) m_angle = 0;
			setRotateZ(NodeMat, m_angle);

			Dest.w = (int)(m_width * p_world.m_floats[0][0]);
			Dest.h = (int)(m_height * p_world.m_floats[1][1]);

			DrawNode();
		
	};

	void UpdateNode(Matrix3 p_world, Matrix3 p_parent)
	{
		if (m_ResourceAmount <= 0 && Immortal == false) { Alive = false; m_sprite = TM_DEPLETED; }
		
			NodeMat.m_floats[1][1] = 1;
			NodeMat.m_floats[2][0] = 0;
			NodeMat.m_floats[2][1] = 0;

			m_angle += m_RotSpeed;
			NodeMat.setRotateZ(m_angle);
			m_x = MX;
			m_y = MY;
			Vector3 VecChild(m_x, m_y, 1);
			VecChild = p_parent * NodeMat * VecChild;

			NodeMat.m_floats[2][0] = VecChild.x;
			NodeMat.m_floats[2][1] = VecChild.y;

			Dest.w = (int)(m_width*p_world.m_floats[0][0]);
			Dest.h = (int)(m_height*p_world.m_floats[1][1]);

			m_x = NodeMat.m_floats[2][0];
			m_y = NodeMat.m_floats[2][1];

			DrawNode();
		
	};

	void SetSprite(int a_frameWidth, int a_frameheight,int a_frameX, int a_frameY, int a_Width, int a_height, float a_delay, float a_RotSpeed)
	{
		m_width			= a_Width ;
		m_height		= a_height ;
		m_frameHeight	= a_frameheight ;
		m_frameWidth	= a_frameWidth ;
		m_FrameX		= a_frameX ;
		m_FrameY		= a_frameY ;
		m_delay			= a_delay ;
		m_RotSpeed		= a_RotSpeed;
	}

	void ChangeSprite(SDL_Texture * a_sprite)
	{
		m_sprite = a_sprite;
	}

	void setQueued(bool a) { Queued = a; };

	void SetVisited(bool a) { Visited = a; };

	Node* Connections[4];



	enum ResourceType
	{
		Empty, Home, Metal, Fuel, Depleted
	};

	float m_x;
	float m_y;
	int m_cost; 
	Matrix3 NodeMat;
	ResourceType rType;
	int m_ResourceAmount;
	int m_parent;

	bool Alive;
	bool Immortal;

	int g;
	int Index;

	int ReqOne;
	int ReqTwo;

	Node* FromNode;

private:
	SDL_Rect Dest;
	int m_width;
	int m_height;
	int m_frameHeight;
	int m_frameWidth;
	int	m_FrameX;
	int	m_FrameY;
	float m_delay;
	float m_angle;
	float m_RotSpeed;
	float MX ;
	float MY ;
	bool Visited;
	bool Queued;

	int Position[1][1];

	SDL_Texture* m_sprite;
	void setRotateZ(Matrix3 p_parent, float a_num)
	{
		p_parent.m_floats[0][0] = cosf(a_num);
		p_parent.m_floats[0][1] = sinf(a_num);
		p_parent.m_floats[1][0] = -sinf(a_num);
		p_parent.m_floats[1][1] = cosf(a_num);
	}

	float degToRad(float a_deg)
	{
		float rad = 3.14159f*a_deg / 180.0f;

		return rad;
	}
};

class Map
{
public:


private:
};