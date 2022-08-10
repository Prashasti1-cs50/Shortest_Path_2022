#include "Graph.h"
#include "MinHeap.h"
#include <set>

#define DFS_FIRST_PATH

Graph::Graph()
{
	this->m_pVHead = nullptr;
	this->m_vSize = 0;
}
Graph::~Graph()
{
}

void Graph::AddVertex(int vertexKey)
{
	Vertex * pNewVer = new Vertex(vertexKey);	// New vertex object
	if (this->m_pVHead == NULL) {	//if first node
		this->m_pVHead = pNewVer;	//Set vertex Head
		this->m_vSize = 1;			//size = 1
		return;
	}
	Vertex * pCur = this->m_pVHead;			//start from Vertex Head
	while (pCur->GetNext() != NULL) {		//move to tail
		pCur = pCur->GetNext();
	}
	pCur->SetNext(pNewVer);		//add new vertex object
	this->m_vSize++;	//size ++
}

void Graph::AddEdge(int startVertexKey, int endVertexKey, int weight)	//add edge
{
	Vertex * pStartVertex = FindVertex(startVertexKey);
	pStartVertex->AddEdge(endVertexKey, weight);
}

Vertex * Graph::FindVertex(int key)		//find vertex with key
{
	Vertex * pCur = this->m_pVHead;
	while (1) {
		if (pCur->GetKey() == key) return pCur;	//if found return node
		else {
			if (pCur->GetNext() == NULL) {
				pCur = nullptr;
				return pCur;
			}
			pCur = pCur->GetNext();
		}
	}
}

int Graph::Size() const
{
	return this->m_vSize;	//get size
}

void Graph::Clear() //graph de-allocate function
{
	Vertex * pCurVertex = this->m_pVHead;
	Edge * pCurEdge;
	Edge * ptemp;
	while (pCurVertex != NULL) {
		pCurEdge = pCurVertex->GetHeadOfEdge();
		ptemp = pCurEdge->GetNext();
		while (ptemp != NULL) {
			delete pCurEdge;
			pCurEdge = ptemp;			//clear edge
			ptemp = pCurEdge->GetNext();
		}
		delete pCurEdge;
		this->m_pVHead = pCurVertex->GetNext();
		delete pCurVertex;		//clear vertex
		pCurVertex = this->m_pVHead;
	}
}

void Graph::Print(std::ofstream & fout)	//print function
{
	Vertex * pCurVer = this->m_pVHead;
	Edge * pCurEdge;
	for (int i = 0; i < this->m_vSize; ++i) {
		pCurEdge = pCurVer->GetHeadOfEdge();
		for (int j = 0; j < this->m_vSize; ++j) {
			if (pCurEdge->GetKey() == j) {
				fout << pCurEdge->GetWeight() << " ";
				if (pCurEdge->GetNext() != NULL) {
					pCurEdge = pCurEdge->GetNext();		//print graph linked list
				}
			}
			else {
				fout << "0 ";
			}
		}
		fout << endl;
		if (pCurVer->GetNext() != NULL) {
			pCurVer = pCurVer->GetNext();
		}
	}
}

bool Graph::IsNegativeEdge()	//negative edge exist
{
	Vertex * pCurVertex = this->m_pVHead;
	Edge * pCurEdge;

	for (int i = 0; i < this->m_vSize; ++i) {
		if (pCurVertex->GetHeadOfEdge() != NULL) {
			pCurEdge = pCurVertex->GetHeadOfEdge();
			while (pCurEdge != NULL) {
				if (pCurEdge->GetWeight() < 0) return true;	//negative edge exist
				pCurEdge = pCurEdge->GetNext();
			}
		}
		pCurVertex = pCurVertex->GetNext();
	}
	return false;	//doesn't exist
}

std::vector<int> Graph::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)	//dijkstra algorithms
{
	vector<int> v;
	Vertex * pStartVertex = FindVertex(startVertexKey);
	Vertex * pEndVertex = FindVertex(endVertexKey);

	if (pStartVertex == nullptr || pEndVertex == nullptr) {	//vertex not found
		v.push_back(-1);
		return v;
	}
	Vertex * pCurVertex = pStartVertex;
	Edge * pCurEdge;

	int ver_source, wei_source, ver_dest, wei_dest;

	vector<int> distance, prev;
	distance.resize(this->m_vSize, IN_FINITY);	//initialize with infinite value
	prev.resize(this->m_vSize, startVertexKey);	//initialize with start vertex key
	distance[startVertexKey] = 0;				//start vertex distance = 0

	set<pair<int, int>> s;
	s.insert(make_pair(distance[startVertexKey], startVertexKey));	//<dist, vertex>

	pCurEdge = pCurVertex->GetHeadOfEdge();	//start with head edge
	while (!s.empty()) {
		wei_source = s.begin()->first;
		ver_source = s.begin()->second;
		s.erase(s.begin());				//pop root

		if (distance[ver_source] >= wei_source) {		
			while (1) {
				ver_dest = pCurEdge->GetKey();
				wei_dest = wei_source + pCurEdge->GetWeight();
				if (distance[ver_dest] > wei_dest) {			//update distance
					s.erase(make_pair(distance[ver_dest], ver_dest));
					distance[ver_dest] = wei_dest;
					prev[ver_dest] = ver_source;
					s.insert(make_pair(distance[ver_dest], ver_dest));	//update set
				}
				if (pCurEdge->GetNext() == NULL) {
					break;
				}
				else pCurEdge = pCurEdge->GetNext();
			}
			if (!s.empty()) {
				pCurVertex = FindVertex(s.begin()->second);	//next value
				pCurEdge = pCurVertex->GetHeadOfEdge();
			}
		}
	}

	int key = endVertexKey;
	while (key != startVertexKey) {	//push vector
		v.push_back(key);
		key = prev[key];
	}
	v.push_back(key);
	v.push_back(distance[endVertexKey]);	//push path length
	return v;
}

std::vector<int> Graph::FindShortestPathDijkstraUsingMinHeap(int startVertexKey, int endVertexKey)	//dijkstra with min heap
{
	vector<int> v;

	Vertex * pStartVertex = FindVertex(startVertexKey);
	Vertex * pEndVertex = FindVertex(endVertexKey);
	if (pStartVertex == nullptr || pEndVertex == nullptr) {		// vertex not found
		v.push_back(-1);
		return v;
	}
	Vertex * pCurVertex = pStartVertex;
	Edge * pCurEdge;
	int ver_source, wei_source, ver_dest, wei_dest;
	
	vector<int> distance;
	vector<int> prev;
	distance.resize(this->m_vSize, IN_FINITY);	//initialize with infinite value
	prev.resize(this->m_vSize, startVertexKey);	
	distance[startVertexKey] = 0;	//startvertex 0

	MinHeap<int, int> minheap;
	minheap.Push(distance[startVertexKey], startVertexKey); //push to minheap

	pCurEdge = pCurVertex->GetHeadOfEdge();
	while (!minheap.IsEmpty()) {
		wei_source = minheap.Top().first;
		ver_source = minheap.Top().second;
		minheap.Pop();		//pop root
		if (distance[ver_source] >= wei_source) {
			while (pCurEdge != NULL) {
				ver_dest = pCurEdge->GetKey();
				wei_dest = wei_source + pCurEdge->GetWeight();
				if (distance[ver_dest] > wei_dest) {		//update distance
					distance[ver_dest] = wei_dest;
					prev[ver_dest] = ver_source;
					minheap.DecKey(ver_dest, distance[ver_dest]);	//update minheap
				}
				pCurEdge = pCurEdge->GetNext();
			}
			if (!minheap.IsEmpty()) {
				pCurVertex = FindVertex(minheap.Top().second);	//next value
				pCurEdge = pCurVertex->GetHeadOfEdge();
			}
		}
	}
	int key = endVertexKey;
	while (key != startVertexKey) {			//push to vector
		v.push_back(key);
		key = prev[key];
	}
	v.push_back(key);
	v.push_back(distance[endVertexKey]);	//push path length to vector
	return v;
}

std::vector<int> Graph::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)	//bellman-ford algorithms
{
	vector<int> v;

	if (this->m_pVHead == nullptr)	return v;	//if graph not exist

	Vertex * pStartVertex = FindVertex(startVertexKey);
	Vertex * pEndVertex = FindVertex(endVertexKey);

	if (pStartVertex == nullptr || pEndVertex == nullptr) {		//vertex key not found
		v.push_back(IN_FINITY + 1);
		return v;
	}
	Vertex * pCurVertex;
	Edge * pCurEdge;

	int ver_source, ver_dest, wei_source, wei_dest;

	vector<int> distance, prev;

	distance.resize(this->m_vSize, IN_FINITY);	//initialize with infinity value
	prev.resize(this->m_vSize, startVertexKey);
	distance[startVertexKey] = 0;

	for (int i = 0; i < this->m_vSize; ++i) {		//vertex n cycle
		pCurVertex = pStartVertex;
		pCurEdge = pCurVertex->GetHeadOfEdge();
		ver_source = pCurVertex->GetKey();
		wei_source = distance[ver_source];
		if (wei_source != IN_FINITY) {
			while (1) {								//edge e cycle
				ver_dest = pCurEdge->GetKey();
				wei_dest = wei_source + pCurEdge->GetWeight();
				if (distance[ver_dest] > wei_dest) {
					distance[ver_dest] = wei_dest;	//update distance
					prev[ver_dest] = ver_source;
					if (i == this->m_vSize - 1) {
						v.push_back(-IN_FINITY);	//negative cycle detected
						return v;		//return
					}
				}
				if (pCurEdge->GetNext() == NULL) {
					if (pCurVertex->GetNext() == NULL) {
						break;
					}
					else {
						pCurVertex = pCurVertex->GetNext();
						pCurEdge = pCurVertex->GetHeadOfEdge();
						ver_source = pCurVertex->GetKey();		//next value
						wei_source = distance[ver_source];
					}
				}
				else {
					pCurEdge = pCurEdge->GetNext();
				}
			}
		}
	}

	int key = endVertexKey;
	while (key != startVertexKey) {	//push to vector
		v.push_back(key);
		key = prev[key];
	}
	v.push_back(key);
	v.push_back(distance[endVertexKey]);	//push path length
	return v;
}

std::vector<vector<int>> Graph::FindShortestPathFloyd()	//Floyd algorithms
{
	vector<vector<int>> distance;

	if (this->m_pVHead == nullptr) return distance;	//if graph not exist

	Vertex * pCurVertex = this->m_pVHead;
	Edge * pCurEdge = pCurVertex->GetHeadOfEdge();

	distance.resize(this->m_vSize);

	for (int i = 0; i < this->m_vSize; ++i) {
		for (int j = 0; j < this->m_vSize; ++j) {
			distance[i].push_back(IN_FINITY);		//initialize with Infinity value
			if (i == j)
				distance[i][j] = 0;	//diagonal with 0
		}
	}

	while (1) {
		while (pCurEdge != NULL) {
			distance[pCurVertex->GetKey()][pCurEdge->GetKey()] = pCurEdge->GetWeight();	//first floyd algorthms
			pCurEdge = pCurEdge->GetNext();
		}
		pCurVertex = pCurVertex->GetNext();
		if (pCurVertex == NULL) break;
		else pCurEdge = pCurVertex->GetHeadOfEdge();
	}

	//Floyd main algorithms
	for (int i = 0; i < this->m_vSize; ++i) {
		for (int j = 0; j < this->m_vSize; ++j) {
			for (int k = 0; k < this->m_vSize; ++k) {
				if (distance[k][j] > distance[k][i] + distance[i][j]) {
					distance[k][j] = distance[k][i] + distance[i][j];
				}
			}
		}
	}

	//negative cycle detected
	for (int i = 0; i < this->m_vSize; ++i) {
		for (int j = 0; j < this->m_vSize; ++j) {
			for (int k = 0; k < this->m_vSize; ++k) {
				if (distance[k][j] > distance[k][i] + distance[i][j]) {
					distance[0][0] = -IN_FINITY;
					break;
				}
			}
		}
	}

	return distance;
}
