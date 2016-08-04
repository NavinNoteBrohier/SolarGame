#pragma once
#include "Node.h"
#include "Entity.h"

Entity::Entity()
{
	m_nodes = 0;
	m_ship = 0;

	rMetal = 100;
	rFuel = 100;
	Alive = true;
	BuildCost = 110;

	bool HomeSet = false;
	bool ShipSet = false;

	int i = 0;
	turn = 100;
}

void Entity::Construct(vector<Node*>* a_nodes, vector<ship*>* a_ship, vector<int*>* a_idle)
{
	m_nodes = a_nodes;
	m_ship = a_ship;

	rMetal = 100;
	rFuel = 100;
	Alive = true;
	BuildCost = 110;

	bool HomeSet = false;
	bool ShipSet = false;

	int i = 0;
	turn = 100;

	while (HomeSet == false)
	{
		if ((*m_nodes)[i]->rType == Home) { m_Home = i; HomeSet = true; }
		else { HomeSet = false; };
		i++;
	}

	for (int j = 0; j < m_ship->size(); j++)
	{
		(*m_ship)[j]->SetHome(m_Home);
		(*m_ship)[j]->SetDeposits(rMetal, rFuel);
		MetalThreshold += 10;
		TotalShips++;
	}
}

void Entity::updateEntity(vector <int*>* IdleShips)
{
	if (rFuel <= 0) { Alive = false; };
	if (Alive == false)
	{
		//delete[] m_nodes->at(m_Home);
		Alive = true;
	}
	m_x = (*m_nodes)[m_Home]->m_x;
	m_y = (*m_nodes)[m_Home]->m_y;

	if ((*m_nodes)[m_Home]->ReqOne > 0)
	{
		cout << "Bump" << endl;
	}

	rMetal += (*m_nodes)[m_Home]->ReqOne;
	rFuel += (*m_nodes)[m_Home]->ReqTwo;

	(*m_nodes)[m_Home]->ReqOne = 0;
	(*m_nodes)[m_Home]->ReqTwo = 0;
	//static int Cleanup = 100;
	//Cleanup--;
	//if (Cleanup <= 0)
	//{
	//	for (int i = 0; i < m_nodes->size(); i++)
	//	{
	//		if ((*m_nodes)[i]->Alive == false) 
	//		{
	//			delete (*m_nodes)[i];
	//			m_nodes->erase(m_nodes->begin() + i);
	//		};
	//	}
	//	Cleanup = 100;
	//}

	turn -= 1;
	if (turn == 0)
	{
	

		CollectIdleShips(IdleShips, m_ship);

		for (int i = 0; i < IdleShips->size(); i++)
		{
			if ((*m_ship)[*(*IdleShips)[i]]->m_rtype == Metal)
			{
 				rMetal += 15;
			}
			if ((*m_ship)[*(*IdleShips)[i]]->m_rtype == Fuel)
			{
				rFuel += 15;
			}
			(*m_ship)[*(*IdleShips)[i]]->m_cargo = 0;
			

			if ((*m_ship)[*(*IdleShips)[i]]->Hull == 0)
			{
				cout << "Hull breach" << endl;
			}

			while (	(*m_ship)[ *(*IdleShips)[i] ]->Hull != 100 && rMetal > 0) 
			{
				(*m_ship)[ *(*IdleShips)[i] ]->Hull++;
				rMetal--;
			}
		}

		FindRec();
		CalculatePriotity(IdleShips);



		CalculateFuelUse(IdleShips);
		turn = 1;
		rFuel -= FuelUse;

		
		
		cout << "Fuel  " << rFuel << endl;
		cout << "Fuel Use " << FuelUse << endl;
		cout << "Total Ships " << TotalShips << endl;
		cout << "Metal " << rMetal << endl;
		cout << "Metal Threshold " << MetalThreshold  << endl << endl;
	}

	//cout << turn << endl;


}

float Entity::CalculateFuelUse(vector <int*>* IdleShips)
{
	float j = IdleShips->size() * 0.05f;		
	FuelUse = j;
	return j;
}

void Entity::CalculatePriotity(vector <int*>* IdleShips)
{
	int p_Metal = 100;
	int p_Fuel = 92;
	int BuildQ = 99;

	FuelThreshold = FuelUse * 100;

	if (rMetal > MetalThreshold + BuildCost)
	{
		BuildQ += 1;
	};

	if (rMetal > MetalThreshold)
	{
		p_Metal -= 1;
	}
	else { p_Metal += 1; }

	if (rFuel > FuelThreshold)
	{
		p_Fuel -= 1;
	}
	else { p_Fuel += 1; }


	for (int j = 0; j < m_ship->size(); j++)
	{
		if ((*m_ship)[j]->m_rtype == Fuel)
		{
			p_Fuel -= 1;
		};
		if ((*m_ship)[j]->m_rtype == Metal)
		{
			p_Metal -= 1; BuildQ += 1;
		};
	}
	int i = 0;
	while (IdleShips->size() > i)
	{

		if (p_Fuel >= p_Metal)
		{
			(*m_ship)[*(*IdleShips)[i]]->SetDestination(C_Fuel);
			(*m_ship)[*(*IdleShips)[i]]->m_rtype = Fuel;
			p_Fuel--;
		}
		else if (p_Metal > p_Fuel)
		{
			(*m_ship)[*(*IdleShips)[i]]->SetDestination(C_Metal);
			(*m_ship)[*(*IdleShips)[i]]->m_rtype = Metal;
			p_Metal--;
		}
		else { p_Metal += 1; p_Fuel += 1; }
		i++;
	}


	if (BuildQ >= p_Metal && rMetal >= BuildCost)
	{
		TotalShips++;
		(*m_ship).push_back(new ship(TM_PODSHIP, m_x, m_y, 25, 25, 25, 25, 0, 0, false));
		(*m_ship)[TotalShips - 1]->Idle = true;
		(*m_ship)[TotalShips - 1]->SetTarget(m_nodes);
		(*m_ship)[TotalShips - 1]->Destination = m_Home;
		(*m_ship)[TotalShips - 1]->m_tex = TM_PODSHIP;
		(*m_ship)[TotalShips - 1]->SetTopSpeed(HELP_Random(1, 11));
		(*m_ship)[TotalShips - 1]->SetHome(m_Home);
		(*m_ship)[TotalShips - 1]->SetDeposits(rMetal, rFuel);
		MetalThreshold += 10;
		rMetal -= BuildCost;
	}
	cout << "Ships Sorted " << endl;
	cout << "Metal Q " << p_Metal << endl;
	cout << "Fuel Q " << p_Fuel << endl;
	cout << "Build Q " << BuildQ << endl;
}

void Entity::CollectIdleShips(vector<int*>* IdleShips, vector <ship*>* Ships)
{
	for (int i = 0; i < IdleShips->size(); i++)
	{
		delete (*IdleShips)[i];
		IdleShips->erase(IdleShips->begin() + i);
	}

	for (int i = 0; i < Ships->size(); i++)
	{
		if ((*Ships)[i]->Idle == true) { IdleShips->push_back(new int(i)); 	(*Ships)[i]->m_rtype = Home;};
	}
}

void Entity::CollectNodes()
{
	if (m_MetalNodes.size() != NULL)
	{
		for (int i = 0; i < m_MetalNodes.size(); i++)
		{
			delete m_MetalNodes[i];
			m_MetalNodes.erase(m_MetalNodes.begin() + i);
		}
	}

	if (m_FuelNodes.size() != NULL)
	{
		for (int i = 0; i < m_FuelNodes.size(); i++)
		{
			delete m_FuelNodes[i];
			m_FuelNodes.erase(m_FuelNodes.begin() + i);
		}
	}

	for (int i = 0; i < m_nodes->size(); i++)
	{
		if ((*m_nodes)[i]->rType == Metal && (*m_nodes)[i]->Alive == true)
		{
			m_MetalNodes.push_back(new int(i));
		}
		else if ((*m_nodes)[i]->rType == Fuel && (*m_nodes)[i]->Alive == true)
		{
			m_FuelNodes.push_back(new int(i));
		}
	}

}

float CalcDis(float numA, float numB)
{
	float numC = (numA - numB);
	//if (numC < 0) { numC * -1; }
	return numC;
};


void Entity::FindRec()
{
	(*m_nodes)[m_Home]->m_x;
	(*m_nodes)[m_Home]->m_y;

	static float TX = 50000;
	static float TY = 50000;

	static float TXa = 50000;
	static float TYa = 50000;
	int FuelAddress = 0;

	CollectNodes();

	for (int i = 0; i < m_nodes->size(); i++)
	{
		if (m_FuelNodes.size() > 0)
		{
			if (CalcDis((*m_nodes)[m_Home]->m_x, (*m_nodes)[ *m_FuelNodes[i] ]->m_x) < TX &&
				CalcDis((*m_nodes)[m_Home]->m_y, (*m_nodes)[ *m_FuelNodes[i] ]->m_y) < TY)
			{
				TX = CalcDis((*m_nodes)[i]->m_x , (*m_nodes)[m_Home]->m_x);
				TY = CalcDis((*m_nodes)[i]->m_y , (*m_nodes)[m_Home]->m_y);
				C_Fuel = i;
			}
		} 
		else { C_Fuel = m_Home; };
	}


	for (int i = 0; i < m_nodes->size(); i++)
	{
		if (m_FuelNodes.size() > 0)
		{
			if (CalcDis((*m_nodes)[m_Home]->m_x, (*m_nodes)[ *m_MetalNodes[i] ]->m_x) < TX &&
				CalcDis((*m_nodes)[m_Home]->m_y, (*m_nodes)[ *m_MetalNodes[i] ]->m_y) < TY)
			{
				TXa = CalcDis((*m_nodes)[i]->m_x , (*m_nodes)[m_Home]->m_x);
				TYa = CalcDis((*m_nodes)[i]->m_y , (*m_nodes)[m_Home]->m_y);
				C_Metal = i;
			}
		}
		else { C_Metal = m_Home; }
	}

}

vector<Node*> Pathing::FindPath(Node * start, Node * end, ship * a_ship)
{
	if (start == nullptr || end == nullptr)
	{
		return vector<Node*>();
	}

	vector<Node*> OpenList;
	vector<Node*> ClosedList;

	OpenList.push_back(start);
	Node* CurrentNode = nullptr;

	while (OpenList.size() > 0)
	{
		std::sort(OpenList.begin(), OpenList.end(),CompareG);
		CurrentNode = OpenList[0];
		if (CurrentNode == end)
		{
			break;
		}
		for (int i = 0; i < 4; i++)
		{
			Node* connection = CurrentNode->Connections[i];
			if (connection == nullptr || FindNode(ClosedList, connection))
			{
				continue;
			}
		
			int TempG = CurrentNode->g + connection->m_cost;
			if (FindNode(OpenList, connection) == false)
			{
				OpenList.push_back(connection);
			}
			else if (TempG > connection->g)
			{
				continue;
			}

			connection->g = TempG;
			connection->FromNode = CurrentNode;

		}
		OpenList.erase(OpenList.begin());
		ClosedList.push_back(CurrentNode);
	}

	if (CurrentNode != end)
	{
		return vector<Node*>();
	}

	vector<Node*> Path;

	while (CurrentNode != start)
	{
		Path.push_back(CurrentNode);
		CurrentNode = CurrentNode->FromNode;
	}

	std::reverse(Path.begin(), Path.end());

	


	return Path;
}

bool Pathing::CompareG(Node * a, Node * b)
{
	return a->g < b->g;
}

bool Pathing::FindNode(vector<Node*> Vector, Node * node)
{
	bool Result = false;
	
	for (int i = 0; i < Vector.size(); i++)
	{
		if (Vector[i] == node)
		{
			Result = true;
		}
	}
	
	return Result;
}
