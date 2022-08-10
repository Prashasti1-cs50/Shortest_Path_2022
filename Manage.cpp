#include "Manager.h"
#include <cstring>

Manager::~Manager()
{
    if (fout.is_open())
        fout.close();
}

void Manager::Run(const char* filepath)	//main run function
{
    fout.open(RESULT_LOG_PATH, ios::trunc);

	ifstream fin;
	fin.open(filepath);
	if (!fin) {		//if command file not exist
		fout << "====== SYSTEM ======"
			<< "CommandFileNotExist" << endl
			<< "=====================" << endl;
		PrintError(CommandFileNotExist);
	}
	Result result;
	char * cmd = new char[40];
	char startvertex[40] = {0,};
	char endvertex[40] = {0,};

	while (!fin.eof()) 
	{
		fin.getline(cmd, 40);
		char * one = strtok(cmd, " \n\r");	//tokenize command
		if (strlen(cmd) < 2) break;			//terminate
		if (strcmp(one, "LOAD") == 0) {
			fout << "===== " << one << " =====" << endl;
			one = strtok(NULL, " \n\r");
			if ((one != NULL && Load(one) == Success))	//Load function call
				PrintError(Success);
			else {
				fout << "LoadFileNotExist" << endl
					<< "==================" << endl << endl;	//error
				PrintError(LoadFileNotExist);
			}
		}
		else if (strcmp(one, "PRINT") == 0) {
			fout << "===== PRINT =====" << endl;
			result = Print();	//print function call
			PrintError(result);	//print error code
		}
		else if (strcmp(one, "DIJKSTRA") == 0) {
			fout << "====== DIJKSTRA ======" << endl;
			one = strtok(NULL, " \r\n");
			if (!one) {
				fout << "VertexKeyNotExist" << endl
					<<"======================" << endl << endl;	//startvertex not exist
				PrintError(VertexKeyNotExist);
			}
			else {
				strcpy(startvertex, one);
				one = strtok(NULL, " \n\r");
				if (!one) {
					fout << "VertexKeyNotExist" << endl			//endvertex not exist
						<< "====================" << endl << endl;
					PrintError(VertexKeyNotExist);
				}
				else {
					strcpy(endvertex, one);
					result = FindShortestPathDijkstraUsingSet(atoi(startvertex), atoi(endvertex));	//dijkstra function call
					PrintError(result);
				}
			}
		}
		else if (strcmp(one, "DIJKSTRAMIN") == 0) {
			fout << "====== DIJKSTRAMIN ======" << endl;
			one = strtok(NULL, " \r\n");
			if (!one) {
				fout << "VertexKeyNotExist" << endl
					<< "======================" << endl << endl;	//startvertex not exist
				PrintError(VertexKeyNotExist);
			}
			else {
				strcpy(startvertex, one);
				one = strtok(NULL, " \r\n");
				if (!one) {
					fout << "VertexKeyNotExist" << endl
						<< "====================" << endl << endl;	//endvertex not exist
					PrintError(VertexKeyNotExist);
				}
				else {
					strcpy(endvertex, one);
					result = FindShortestPathDijkstraUsingMinHeap(atoi(startvertex), atoi(endvertex));	//dijkstra with my-minheap
					PrintError(result);
				}
			}
		}
		else if (strcmp(one, "BELLMANFORD") == 0) {
			fout << "====== BELLMANFORD ======" << endl;
			one = strtok(NULL, " \r\n");
			if (!one) {
				fout << "VertexKeyNotExist" << endl
					<< "======================" << endl << endl;	//startvertex not exist
				PrintError(VertexKeyNotExist);
			}
			else {
				strcpy(startvertex, one);
				one = strtok(NULL, " \r\n");
				if (!one) {
					fout << "VertexKeyNotExist" << endl
						<< "====================" << endl << endl;	//endvertex not exist
					PrintError(VertexKeyNotExist);
				}
				else {
					strcpy(endvertex, one);
					result = FindShortestPathBellmanFord(atoi(startvertex), atoi(endvertex));	//bellman-ford function call
					PrintError(result);
				}
			}
		}
		else if (strcmp(one, "FLOYD") == 0) {
			fout << "====== FLYOD =======" << endl;
			result = FindShortestPathFloyd();	//floyd function call
			PrintError(result);
		}
		else {
			fout << "===== " << one << " =====" << endl
				<< "NonDefinedCommand" << endl
				<< "=======================" << endl << endl;	//error command
			PrintError(NonDefinedCommand);
		}
	}
	fin.close();
}

void Manager::PrintError(Result result)	//print error code
{
	fout << "=====================" << endl
		<< "Error code: " << result << endl
		<< "=====================" << endl << endl;
}

Result Manager::Load(const char* filepath)		//Load mapdata
{
    // TODO: implement
	ifstream fin;
	fin.open(filepath);		//file open

	if (!fin) {
		return CommandFileNotExist;	//file not exist
	}

	int size, row = 0;
	char * str = NULL;
	char * temp = new char[100];
	int** mapData;

	fin.getline(temp, 100);	//get size
	size = atoi(temp);		//size of map
	
	mapData = new int*[size];
	for (int i = 0; i < size; ++i) {
		mapData[i] = new int[size];
		memset(mapData[i], 0, sizeof(int)*size);	//initialize 2D-array
	}

	for (int i = 0; i < size; ++i) {
		fin.getline(temp, 100);
		str = strtok(temp, " \n\r");
		mapData[row][0] = atoi(str);
		for (int col = 1; col < size; ++col) {	
			str = strtok(NULL, " \n\r");
			mapData[row][col] = atoi(str);	//copy to 2D-array
		}
		row++;
	}

	for (row = 0; row < size; ++row) {
		this->m_graph.AddVertex(row);	//make graph vertex
		for (int col = 0; col < size; ++col) {
			if (mapData[row][col] != 0) {
				this->m_graph.AddEdge(row, col, mapData[row][col]);	//make graph edge
			}
		}
	}

	for (int i = 0; i < size; ++i)	delete[] mapData[i];	//de-allocate
	fout << "Success" << endl;
	fout << "=================" << endl << endl;
	return Success;
}

Result Manager::Print()		//print graph
{
	if (this->m_graph.Size() == 0) {		//if graph not exist
		fout << "GraphNotExist" << endl;
		fout << "=====================" << endl << endl;
		return GraphNotExist;
	}
	this->m_graph.Print(fout);		//print function call
	fout << "==================" << endl << endl;
	return Success;
}

Result Manager::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)	//Dijkstra Algorithms
{
	vector<int> v;
	int i = 0;
	if (this->m_graph.Size() == 0) {		//if graph not exist
		fout << "GraphNotExist" << endl
			<< "=====================" << endl << endl;
		return GraphNotExist;
	}
	if (this->m_graph.IsNegativeEdge()) {		//if negative edge detected
		fout << "InvalidAlgorithm" << endl
			<< "======================" << endl << endl;
		return InvalidAlgorithm;
	}
	v = this->m_graph.FindShortestPathDijkstraUsingSet(startVertexKey, endVertexKey);	//Dijkstra Algorithms
	int path_length = v.back();
	v.pop_back();
	if (path_length == -1) {					//vertex not exist
		fout << "InvalidVertexKey" << endl
			<< "=========================" << endl << endl;
		return InvalidVertexKey;
	}
	fout << "shortest path: ";
	for (int i = v.size() - 1; i >= 0; --i)	//print result	
		fout << v[i] << " ";
	fout << endl;
	fout << "sorted nodes: ";
	QuickSort(v, 0, v.size() - 1);
	for (int i = 0; i < v.size(); ++i)		//print sorted result
		fout << v[i] << " ";
	fout << endl;
	fout << "path length: " << path_length << endl		//print path length
		<< "=======================" << endl << endl;
	return Success;
}

Result Manager::FindShortestPathDijkstraUsingMinHeap(int startVertexKey, int endVertexKey)	//Dijkstra with My-MINHEAP
{
	if (this->m_graph.Size() == 0) {				//graph not exist
		fout << "GraphNotExist" << endl
			<< "=================" << endl << endl;
		return GraphNotExist;
	}
	if (this->m_graph.IsNegativeEdge()) {			//negative edge detected
		fout << "InvalidAlgorithm" << endl
			<< "=========================" << endl << endl;
		return InvalidAlgorithm;
	}
	vector<int> v;
	int i = 0;
	v = this->m_graph.FindShortestPathDijkstraUsingMinHeap(startVertexKey, endVertexKey);	//Dijkstra with My-MINHEAP
	int path_length = v.back();
	v.pop_back();
	if (path_length == -1) {				//vertex not exist
		fout << "InvalidVertexKey" << endl
			<< "=======================" << endl << endl;
		return InvalidVertexKey;
	}
	fout << "shortest path: ";
	for (int i = v.size() - 1; i >= 0; --i)		//print result
		fout << v[i] << " ";
	fout << endl;
	fout << "sorted nodes: ";
	QuickSort(v, 0, v.size() - 1);
	for (int i = 0; i < v.size(); ++i)			//print sorted result
		fout << v[i] << " ";
	fout << endl;
	fout << "path length: " << path_length << endl;	//print path length
	fout << "==================" << endl << endl;
	return Success;
}

Result Manager::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)	//Bellman-Ford Algorithms
{
	if (this->m_graph.Size() == 0) {				//if graph not exist
		fout << "GraphNotExist" << endl
			<< "==================" << endl << endl;
		return GraphNotExist;
	}
	vector<int> v;
	int i = 0;
	v = m_graph.FindShortestPathBellmanFord(startVertexKey, endVertexKey);	//Bellman-Ford Algorithms
	int path_length = v.back();
	v.pop_back();
	if (path_length == IN_FINITY + 1) {						//vertex doesn't exist
		fout << "InvalidVertexKey" << endl
			<< "====================" << endl << endl;
		return InvalidVertexKey;
	}
	if (path_length == -IN_FINITY) {						//negative cycle exist
		fout << "NegativeCycleDetected" << endl
			<< "====================" << endl << endl;
		return NegativeCycleDetected;
	}
	fout << "shortest path: ";
	for (int i = v.size() - 1; i >= 0; --i) {		//print result
		fout << v[i] << " ";
	}
	fout << endl;
	fout << "sorted nodes: ";
	QuickSort(v, 0, v.size() - 1);			//My-STL-sort
	for (int i = 0; i < v.size(); ++i) {	//print sorted result
		fout << v[i] << " ";
	}
	fout << endl;
	fout << "path length: " << path_length << endl;		//path length
	fout << "=====================" << endl << endl;
	return Success;
}

Result Manager::FindShortestPathFloyd()			//Floyd Algorithms
{
	if (this->m_graph.Size() == 0) {		//if graph not exist
		fout << "GraphNotExist" << endl
			<< "====================" << endl << endl;
		return GraphNotExist;
	}
	vector<vector<int>> v;
	v = this->m_graph.FindShortestPathFloyd();	//Floyd algorithms
	if (v[0][0] == -IN_FINITY) {				//if negative cycle detected
		fout << "NegativeCycleDetected" << endl
			<< "====================" << endl << endl;
		return NegativeCycleDetected;
	}

	for (int i = 0; i < v.size(); ++i) {
		for (int j = 0; j < v.size(); ++j) {	//print result
			fout << v[i][j] << " ";
		}
		fout << endl;
	}
	fout << "===========================" << endl << endl;
	return Success;
}

void Manager::QuickSort(vector<int>& v, int left, int right)	//QuickSort
{
	if (left < right) {
		if (right - left + 1 <= 6)			//if Segment size is smaller than 6
			InsertionSort(v, left, right);	//do Insertion sort
		else {								//partition
			int i = left, j = right + 1, pivot = v[left];
			do {
				do ++i; while (v[i] < pivot);
				do --j; while (v[j] > pivot);
				if (i < j) {
					int temp = v[i];	//swap
					v[i] = v[j];
					v[j] = temp;
				}
			} while (i < j);

			int temp = v[left];	//swap
			v[left] = v[j];
			v[j] = temp;

			QuickSort(v, left, j - 1);	//Recursion
			QuickSort(v, j + 1, right);
		}
	}
}

void Manager::InsertionSort(vector<int>& v, int left, int right)
{
	for (int i = left+1; i <= right; i++) {		//start with second node
		int temp = v[i];
		Insert(v, temp, i - 1);	//find spot & insert
	}
}

void Manager::Insert(vector<int>& v, int e, int i)	//Insertion insert func
{
	while (i >= 0) {			
		if (e < v[i]) {
			v[i + 1] = v[i];		//shift right
			i--;
		}
		else break;
	}
	v[i + 1] = e;	//insert

}


