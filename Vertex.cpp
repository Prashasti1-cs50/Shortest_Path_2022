#include "Vertex.h"

Vertex::Vertex()	//constructor
{
	this->m_key = 0;
	this->m_pEHead = nullptr;
	this->m_pNext = nullptr;
	this->m_size = 0;
}

Vertex::Vertex(int key)	//constructor overload
{
	this->m_key = key;
	this->m_pEHead = nullptr;
	this->m_pNext = nullptr;
	this->m_size = 0;
}

Vertex::~Vertex()
{
}

void Vertex::SetNext(Vertex * pNext)	//set next node
{
	this->m_pNext = pNext;
}

int Vertex::GetKey() const		//return key
{
	return this->m_key;
}

Vertex * Vertex::GetNext() const	//return next node
{
	return this->m_pNext;
}

int Vertex::Size() const		//return size
{
	return this->m_size;
}

void Vertex::AddEdge(int edgeKey, int weight)	//addedge
{
	Edge * pNEdge = new Edge(edgeKey, weight);	//make new edge

	if (this->m_pEHead == NULL) {	//if edge list is empty
		this->m_pEHead = pNEdge;	//make new edge as head
		return;
	}
	Edge * pCur = m_pEHead;	
	while (pCur->GetNext() != NULL) {
		pCur = pCur->GetNext();
	}
	pCur->SetNext(pNEdge);	//add edge
}

Edge * Vertex::GetHeadOfEdge() const	//return head
{
	return this->m_pEHead;
}

void Vertex::Clear()
{

}
