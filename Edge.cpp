#include "Edge.h"

Edge::Edge()		//constructor
{
	this->m_key = 0;
	this->m_pNext = nullptr;
	this->m_weight = 0;
}

Edge::Edge(int key, int weight)	//constructor overload
{
	this->m_key = key;
	this->m_weight = weight;
	this->m_pNext = nullptr;
}

void Edge::SetNext(Edge * pNext)	//set next node
{
	this->m_pNext = pNext;
}

int Edge::GetKey() const	//get key
{
	return this->m_key;
}

int Edge::GetWeight() const	//get weight
{
	return this->m_weight;
}

Edge * Edge::GetNext() const	//get next node
{
	return this->m_pNext;
}
