#include <iostream>
#include <fstream>
#include <list>
#include <time.h>
#include <limits.h>
#include "Graph.h"
#include <cstring>
#include <float.h>
//#include <omp.h>
using namespace std;

//Graph Constructor, graph initialized to null graph
Graph::Graph(int nv)
{
    n = nv;
    adjList = new list <int> [n]; 
    radjList = new list <int> [n];
    it = new float[n];
    dt = new float[n];
    weightMat = new float*[n];
    deltavalue = new float*[n];
    for(int i = 0; i < n; ++i)
    {
	    weightMat[i] = new float[n];
	    deltavalue[i] = new float[n];
    }
}

Graph::~Graph()
{
     //cout <<"\nGraph destroyed\n";
     for(int i = 0; i < n; i++)
     {
	    delete [] weightMat[i];
	    delete [] deltavalue[i]; 
	    adjList[i].clear();
	    radjList[i].clear();
     }
     delete[] weightMat;
     delete[] deltavalue;
     delete[] it;
     delete [] dt;
     delete [] adjList;
     delete [] radjList;
}
//Function to add edge (u,v)
void Graph::addEdge(int u, int v, float w, float l)
{
    adjList[u].push_back(v);
    radjList[v].push_back(u);
    weightMat[u][v] = w;
    deltavalue[u][v] = l;
}
//Function to extract degrees of all vertices an integer array
int * Graph::getInDegrees()
{
    int *inDegree = new int[n];
    for(int i = 0; i < n; i++)
    {
	    inDegree[i] = radjList[i].size();
    }
    return inDegree;
}

//Function to extract degrees of all vertices an integer array
int * Graph::getOutDegrees()
{
    int *outDegree = new int[n];
    for(int i = 0; i < n; i++)
    {
	    outDegree[i] = adjList[i].size();
    }
    return outDegree;
}
void Graph::updateWeights()
{
    int *indegree = getInDegrees();
    for(int i = 0; i < n; i++)
    {
	list<int>::iterator iter;
	//#pragma omp schedule(static) nowait
	for(iter = this->adjList[i].begin(); iter != this->adjList[i].end(); iter++)
	{
	    int j = *iter;
	    weightMat[i][j] = ((float)1.0/indegree[j]);
	}
    }
    delete[] indegree;
}

// New changes for signed diffusion

void Graph::deltaValues()
{
	srand((unsigned) time(NULL));
    for(int i = 0; i < n; i++)
    {
    list<int>::iterator iter;
    for(iter = this->adjList[i].begin(); iter != this->adjList[i].end(); iter++)
	{
	    int j = *iter;
	    deltavalue[i][j] = (double)rand()/RAND_MAX;
	}
    }
}

void Graph::setFixedTH(float itv, float dtv)
{
    for(int i = 0; i < n; i++)
    {
	it[i] = itv;
	dt[i] = dtv;
    }
}

void Graph::setRandomTH()
{
    srand((unsigned) time(NULL));    
    for(int i = 0; i < n; i++)
    {
	it[i] = (double)rand()/RAND_MAX;
	dt[i] = (double)rand()/RAND_MAX;
    }
}

list<int> * Graph::getInfluentialNodes(int rsc, int *RS, int tsc, int *TS)
{
    list<int> *infNodes = new list<int>();
    list<int> *tempList = new list<int>();
    float infW[n] = {0.0};
    int status[n] = {0};
    for(int i = 0; i < rsc; i++)
    {
	tempList->push_back(RS[i]);
	status[RS[i]] = 1;	
    }
    for(int i = 0; i < tsc; i++)
    {
	tempList->push_back(TS[i]);
	status[TS[i]] = 1;
    }
    while(!tempList->empty())
    {
	int u = tempList->front();
	tempList->pop_front();
	list<int>::iterator iter;
	for(iter = this->adjList[u].begin(); iter != this->adjList[u].end(); iter++)
	{
	    int v = *iter;
	    infW[v]+=weightMat[u][v];
	    if(status[v] != 1 and infW[v] >= this->it[v])
	    {
		infNodes->push_back(v);
		tempList->push_back(v);
		status[v] = 1;
	    }
	}
    }
    tempList->clear();
    delete tempList;
    return infNodes;
}

//new diffusion function for sigend graph

void validate(int n1, int n2, int *a, int *b)
{
    for(int i = 0; i < n1; i++)
    {
	for(int j = 0; j < n2; j++)
	{
	    if(a[i] == b[j])
	    {
		cout<<"Duplicate Found\n";
	    }
	}
    }
    for(int i = 0; i < n1; i++)
    {
	for(int j = i+1; j < n1; j++)
	{
	    if(a[i] == a[j])
	    {
		cout<<"Duplicate Found\n";
	    }
	}
    }
    for(int i = 0; i < n2; i++)
    {
	for(int j = i+1; j < n2; j++)
	{
	    if(b[i] == b[j])
	    {
		cout<<"Duplicate Found\n";
	    }
	}
    }

}
DiffusionData * Graph::runDiffussionSigned(int rsc, int *RS, int tsc, int *TS)
{
    DiffusionData *dd = new DiffusionData();
    //list<int> *infNodes = new list<int>();// = getInfluentialNodes(rsc, RS, tsc, TS);	
    list<int> *newInfNodes = new list<int>();
    list<int> *actList = new list<int>();
    //int infFlag[n] = {0};
    int infStat[n] = {0};
    //validate(rsc, tsc, RS, TS);
    for(int i = 0; i < rsc; i++)
    {
	infStat[RS[i]] = 1;
	actList->push_back(RS[i]);
    }

    for(int i = 0; i < tsc; i++)
    {
	infStat[TS[i]] = 2;
	actList->push_back(TS[i]);
    }
    bool cont = true;
    float rinfwPos[n] = {0.0};
    float tinfwNeg[n] = {0.0};
    float rinfwNeg[n] = {0.0};
    float tinfwPos[n] = {0.0};
    int processed[n] = {0};
    while(cont==true)
    {
	cont = false;
	list<int>::iterator iter;
	int inserted[n] = {0};
	for(iter = actList->begin(); iter != actList->end(); iter++)
	{
	    int u = *iter;
	    if(infStat[u] == 0 || processed[u] == 1) continue;

	    list<int>::iterator iter1;
	    for(iter1 = this->adjList[u].begin(); iter1 != this->adjList[u].end(); iter1++)
	    {
		int v = *iter1;
		if(u==v)
		    continue;
		if(infStat[v] == 1 || infStat[v] == 2 || infStat[v] == 4)
		{
		    continue;
		}
		if(infStat[u] == 1 || infStat[u] == 3)
		{
			if(weightMat[u][v] <= 0.0)
			{
				rinfwNeg[v]+= abs(weightMat[u][v]*deltavalue[u][v]);	
			}
			else
			{
				rinfwPos[v]+= weightMat[u][v]*deltavalue[u][v];
			}	
		}
		else
		{
			if(weightMat[u][v] <= 0.0)
			{
				tinfwNeg[v]+= abs(weightMat[u][v]*deltavalue[u][v]);
			}
			else
			{
				tinfwPos[v]+= weightMat[u][v]*deltavalue[u][v];
			}    
		}
		if((inserted[v] == 0) && ((rinfwPos[v] + rinfwNeg[v] + tinfwPos[v] + tinfwNeg[v]) >= it[v]))
		{
		    inserted[v] = 1;
		    newInfNodes->push_back(v);
		}
	    }
	    processed[u] = 1;
	}
	actList->clear();
	list<int> *truthlist = new list<int>();
	list<int> *rumlist = new list<int>();
	list<int> *wupdatelist = new list<int>();
	
	for(iter = newInfNodes->begin(); iter != newInfNodes->end(); iter++)
	{
	    int u = *iter;
	    if(infStat[u] == 1 || infStat[u] == 2 || infStat[u] == 4) continue;

	    float totw = rinfwPos[u] + rinfwNeg[u] + tinfwPos[u] + tinfwNeg[u];
	    if(totw >= it[u])
	    {
		if(totw != 0.0)
		{
		    if(((rinfwPos[u] + tinfwNeg[u])/totw) >= dt[u])
		    {
			if(infStat[u] == 0)
			{
			    rumlist->push_back(u);
			    //infStat[u] = 3;
			    cont = true;
			    actList->push_back(u);
			}
		    }
		    else
		    {
			if(infStat[u] == 0 || infStat[u] == 3)
			{
			    actList->push_back(u);
			    truthlist->push_back(u);
			    cont = true;
			    if(infStat[u] == 3)
			    {
				wupdatelist->push_back(u);
			    }
			}
			//infStat[u] = 4;
		    }
		}
	    }
	}
	newInfNodes->clear();
	for(iter = wupdatelist->begin(); iter != wupdatelist->end(); iter++)
	{
	    int u = *iter;
	    list<int>::iterator iter1;
	    for(iter1 = this->adjList[u].begin(); iter1 != this->adjList[u].end(); iter1++)
	    {
		int v = *iter1;
		if(u == v) continue;
		if(weightMat[u][v] <= 0.0)
		{
		    rinfwNeg[v] -= abs(weightMat[u][v]*deltavalue[u][v]);	
		}
		else
		{
		    rinfwPos[v] -= weightMat[u][v]*deltavalue[u][v];
		}
	    }
	    processed[u] = 0;
	}
	wupdatelist->clear();
	delete wupdatelist;
	for(iter = rumlist->begin(); iter != rumlist->end(); iter++)
	{
	    int u = *iter;
	    infStat[u] = 3;
	}
	rumlist->clear();
	delete rumlist;
	for(iter = truthlist->begin(); iter != truthlist->end(); iter++)
	{
	    int u = *iter;
	    infStat[u] = 4;
	}
	truthlist->clear();
	delete truthlist;

    }
    for(int v = 0; v < n; v++)
    {
		if(infStat[v] == 0 || infStat[v] == 1 || infStat[v] == 2)
	    	continue;
		else if (infStat[v] == 3)
		{
	    	dd->rac++;
	    	dd->RA->push_back(v);
		}
		else
		{
	    	dd->tac++;
	    	dd->TA->push_back(v);
		}
		//cout<<"v:"<<v<<" Rumourwgts:"<<rinfwPos[v]+tinfwNeg[v]<<endl;
    }
    actList->clear();
    delete actList;
    //delete infNodes;
    return dd;
}


//Partitioning of Graph

int * Graph :: getPartitionedGraph()
{
    int * partition = new int[n];
    int * visited = new int[n];
    for(int i = 0; i < n; i++)
    {
	visited[i] = 0;
	partition[i] = 0;
    }
    list<int> *queue = new list<int>();
    queue->push_back(0);
    partition[0] = 1;

    while(!queue->empty())
    {
	int u = queue->front();
	visited[u] = 1;
	//cout<<"u:"<<u<<endl;
	queue->pop_front();
	list<int>::iterator iter;
	for(iter = this->adjList[u].begin(); iter != this->adjList[u].end(); iter++)
	{
	    int v = *iter;
	    //cout<<"v:"<<v<<endl;
	    if(visited[v] == 0)
	    {
		queue->push_back(v);
	    }
	    if(partition[v] == 0)
	    {
			if(weightMat[u][v] >= 0)
		    partition[v] = partition[u];
			else
			{
		    	if(partition[u] == 1)
		    	partition[v] = 2;		
		    	else if(partition[u] == 2)
		    	partition[v] = 1;
			}
	    }
	    else
	    {
			if(weightMat[u][v] < 0)
			{
		    	if(partition[u] == partition[v])
				return NULL;
			}
			else
			{
		    	if(partition[u] != partition[v])
				return NULL;
			}
	    }	    
	}
    }
    return partition;
}

// int * Graph :: getPartitionedGraph()
// {
//     int * partition = new int[n];
//     int * visited = new int[n];
//     ofstream outFile;
//     outFile.open("data.txt");
//     for(int i = 0; i < n; i++)
//     {
// 	visited[i] = 0;
// 	partition[i] = 0;
//     }
//     list<int> *queue = new list<int>();
//     queue->push_back(0);
//     partition[0] = 1;

//     while(!queue->empty())
//     {
// 	int u = queue->front();
// 	visited[u] = 1;
// 	//cout<<"u:"<<u<<endl;
// 	queue->pop_front();
// 	list<int>::iterator iter;
// 	for(iter = this->adjList[u].begin(); iter != this->adjList[u].end(); iter++)
// 	{
// 	    int v = *iter;
// 	    //cout<<"v:"<<v<<endl;
// 	    if(visited[v] == 0)
// 	    {
// 		queue->push_back(v);
// 	    }
// 	    if(partition[v] == 0)
// 	    {
// 			if(weightMat[u][v] >= 0)
// 			{
// 		    	partition[v] = partition[u];
// 		    	outFile<<u<<" "<<v<<" "<<weightMat[u][v]<<endl;
// 			}
// 			else
// 			{
// 		    	if(partition[u] == 1)
// 		    	{
// 		    		partition[v] = 2;
// 					outFile<<u<<" "<<v<<" "<<weightMat[u][v]<<endl;
// 		    	}
// 		    	else if(partition[u] == 2)
// 		    	{
// 		    		partition[v] = 1;
// 					outFile<<u<<" "<<v<<" "<<weightMat[u][v]<<endl;
// 		    	}
// 			}
// 	    }
// 	    else
// 	    {
// 			if(weightMat[u][v] < 0)
// 			{
// 		    	if(partition[u] == partition[v])
// 		    	{
// 		    		adjList[u].remove(v);
// 		    	}
// 				//return NULL;
// 			}
// 			else
// 			{
// 		    	if(partition[u] != partition[v])
// 		    	{
// 		    		adjList[u].remove(v);
// 		    	}
// 				//return NULL;
// 			}
// 	    }	    
// 	}
//     }
//     outFile.close();
//     return partition;
// }

//Calculate Z(P)

float  Graph :: calculateZPValues(int *P)
{	
	float Z = 0.0;
	for(int u = 0; u < n; u++)
	{ 
		list<int> :: iterator iter;
		for(iter = this->adjList[u].begin(); iter != this->adjList[u].end(); iter++)
		{
		    int v = *iter;
		    if(P[u] == P[v] && weightMat[u][v] < 0)
		    {
		    	Z+= abs(weightMat[u][v]);
		    }
		    if(P[u]!=P[v] && weightMat[u][v] > 0)
		    {
		    	Z+= abs(weightMat[u][v]);
		    }   
		}
	}
	return Z;
}

//calculate updated Z(P) values

float Graph :: updateZPValues(int *P, float Z, int u, int j)
{
		int i = P[u];
		list <int> :: iterator iter;
		for(iter = this->adjList[u].begin(); iter != this->adjList[u].end(); iter++)
		{
			int v = *iter;
			if((P[u] == i) && (P[v] == i) && weightMat[u][v] < 0)		
			{
				Z = Z - abs(weightMat[u][v]);
			}
			if((P[u] == i) && (P[v]== j) && weightMat[u][v] > 0)
			{
				Z = Z - abs(weightMat[u][v]);
			}
			if((P[u] == i) && (P[v] == i) && weightMat[u][v] > 0)
			{
				Z = Z + abs(weightMat[u][v]);
			}
			if((P[u] == i) && (P[v]== j) && weightMat[u][v] < 0)
			{
				Z = Z + abs(weightMat[u][v]);
			}
		}
		for(iter = this->radjList[u].begin(); iter != this->radjList[u].end(); iter++)
		{
			int v = *iter;
			if((P[u] == i) && (P[v] == i) && weightMat[v][u] < 0)		
			{
				Z = Z - abs(weightMat[v][u]);
			}
			if((P[u] == i) && (P[v]== j) && weightMat[v][u] > 0)
			{
				Z = Z - abs(weightMat[v][u]);
			}
			if((P[u] == i) && (P[v] == i) && weightMat[v][u] > 0)
			{
				Z = Z + abs(weightMat[v][u]);
			}
			if((P[u] == i) && (P[v]== j) && weightMat[v][u] < 0)
			{
				Z = Z + abs(weightMat[v][u]);
			}
		}
	return Z;
}

//generate random partition
int * Graph :: generateRandpartition(int k)
{
	int *partition = new int[n];
	//srand(time(0));
	for(int i = 0; i < n; i++)
    {
		partition[i] = 0;
    }
	for(int i = 1; i <= k; i++)
	{
		int u;
		do
		{
			u = rand() % n;
		}while(partition[u] != 0);
		partition[u] = i;	
	}
	for(int i = 0; i < n; ++i)
	{
		if(partition[i] == 0)
		{
			partition[i] = (rand() % k) + 1;
		}
	}
	return partition;
}

int * Graph :: getFrequency(int * p, int k)
{
	int *F = new int[k+1];
	for(int i = 0; i < k; ++i)
    {
		F[i] = 0;
    }
	for(int i = 0; i < n; ++i)
	{
		F[p[i]]++;
	}
	return F;
}

//Multistart Relocation Heuristic

int * Graph :: multistartRelocationHeuristic(int k, int time_limt)
{
	float ZPstar = FLT_MAX;
	float ZP = 0.0;
	float ZQ = 0.0;
	int * pstar = NULL;
	int * P = NULL;
	int * F = NULL;
	clock_t start_time = clock();
	clock_t stop_time;
	int h = 0;
	do
	{
		P = generateRandpartition(k);
		ZP = calculateZPValues(P);
		//cout<<"ZP"<<ZP<<endl;
		F = getFrequency(P,k);
		int improv = 1;
        while (improv == 1) 
        {
            improv = 0;
            for(int v = 0; v < n; v++)
            {
            	h = P[v];
            	if(F[h] > 1)
            	{
            		for(int j = 1; j <= k; ++j)
            		{
            			if(h == j)
            				continue;
            			ZQ = updateZPValues(P, ZP, v, j);
            			//cout<<"v:"<<v <<"ZQ"<<ZQ<<endl;
            			if(ZQ < ZP)
            			{
            				P[v] = j;
            				ZP = ZQ;
            				improv = 1;
            			}
            		}
            	}
            }  
        }
        if(ZP < ZPstar)
        {
        	if(pstar != NULL)
        	{
        		free(pstar);
        		pstar = NULL;
        	}
        	pstar = P;
        	ZPstar = ZP;
    		//cout<<"ZPstar:"<<ZPstar<<endl;
        }
        else
        {
        	free(P);
        	P = NULL;
        }
        stop_time = clock() - start_time;
	}while ((stop_time/CLOCKS_PER_SEC) < time_limt);
	return pstar;
}

DiffusionData * Graph::runDiffussionCtr(int rsc, int *RS, int *RI)
{
    DiffusionData *dd = new DiffusionData();
    list<int> *infNodes = new list<int>();// = getInfluentialNodes(rsc, RS, tsc, TS);
    list<int> *actList = new list<int>();
    int infFlag[n] = {0};
    int infStat[n] = {0};
    for(int i = 0; i < rsc; i++)
    {
	infStat[RS[i]] = 1;
	infFlag[RS[i]] = 1;
	RI[RS[i]] = 1;
	actList->push_back(RS[i]);
    }
    int rival = 1;
    bool cont = true;
    while(cont==true)
    {
	cont = false;	
	list<int>::iterator iter;
	float rinfw[n] = {0.0};
	float tinfw[n] = {0.0};
	list<int> *newInfNodes = new list<int>();
	rival++;
	for(iter = actList->begin(); iter != actList->end(); iter++)
	{
	    int u = *iter;
	    list<int>::iterator iter1;
	    for(iter1 = this->adjList[u].begin(); iter1 != this->adjList[u].end(); iter1++)
	    {
		int v = *iter1;
		if(u==v)
		    continue;
		if(infStat[v] == 1 || infStat[v] == 2 || infStat[v] == 4)
		{
		    continue;
		}
		if (infStat[u] == 1 || infStat[u] == 3)
		{
		    rinfw[v]+=weightMat[u][v];
		}
		else
		{
		    tinfw[v]+=weightMat[u][v];
		}
		if(infFlag[v] == 0 && ((rinfw[v] + tinfw[v]) >= it[v]))
		{
		    infFlag[v] = 1;
		    newInfNodes->push_back(v);
		}
	    }
	}
	for(iter = newInfNodes->begin(); iter != newInfNodes->end(); iter++)
	{
	    int v = *iter;
	    infNodes->push_back(v);
	    actList->push_back(v);
	}
	newInfNodes->clear();
	delete newInfNodes;
	

	for(iter = infNodes->begin(); iter != infNodes->end(); iter++)
	{
	    int u = *iter;
	    if(infStat[u] == 4)
		continue;
	    float totw = rinfw[u] + tinfw[u];
	    if(totw >= it[u])
	    {
		if(totw != 0.0)
		{
		    if((rinfw[u]/totw) >= dt[u])
		    {
			if(infStat[u] == 0)
			{
			    infStat[u] = 3;
			    RI[u] = rival;
			    cont = true;
			}
		    }
		    else
		    {
			if(infStat[u] == 0 || infStat[u] == 3)
			{
			    cont = true;
			}
			infStat[u] = 4;
		    }
		}
	    }
	}
	
    }
    list<int>::iterator iter;
    for(iter = infNodes->begin(); iter != infNodes->end(); iter++)
    {
	int v = *iter;
	if(infStat[v] == 0)
	    continue;
	else if (infStat[v] == 3)
	{
	    dd->rac++;
	    dd->RA->push_back(v);
	}
	else
	{
	    dd->tac++;
	    dd->TA->push_back(v);
	}
    }
    infNodes->clear();
    actList->clear();
    delete actList;
    delete infNodes;
    return dd;
}

int * Graph::pruning(int rsc, int *RS, int tsc, int *TS)
{
    DiffusionData *mind = runDiffussionSigned(rsc, RS, tsc, TS);
    bool cont = true;    
    while(cont == true)
    {
	cont = false;
	int flag[n] = {0};
	for(int i = 0; i < rsc; i++)
	{
	    flag[RS[i]] = 1;
	}
	for(int i = 0; i < tsc; i++)
	{
	    flag[TS[i]] = 1;
	}
	for(int i = 0; i < n; i++)
	{
	    if(flag[i] == 1) continue;
	    for(int j = 0; j < tsc; j++)
	    {
		int tnode = TS[j];
		TS[j] = i;
		DiffusionData *dd = runDiffussionSigned(rsc, RS, tsc, TS);
		if(dd->rac < mind->rac)
		{
		    delete mind;
		    mind=NULL;
		    mind = dd;
		    flag[i] = 1;
		    flag[tnode] = 0;
		    cont = true;
		    break;
		}
		else
		{
		    delete dd;
		    TS[j] = tnode;
		}
	    }
	    if(cont == true)
		break;
	}
    }
    return TS;
}

/*
int * Graph::minGreedy(int rsc, int *RS, int k)
{
    int *TS = new int[k];
    int used[n] = {0};
    for(int i = 0; i < rsc; i++)
    {
	used[RS[i]] = 1;
    }
    for(int i = 0; i < k; i++)
    {
	TS[i] = -1;
    }
    //DiffusionData *dinit = runDiffussion(rsc, RS, 0, TS);
    DiffusionData *dinit = runDiffussionSigned(rsc, RS, 0, TS);
    int prevmin = dinit->rac;
    int i = 0;
    while(i < k)
    {
	int min = n+1;
	int minNode;
	for(int j = 0; j < n; j++)
	{
	    if(used[j] == 1)
	    {
		continue;
	    }
	    TS[i] = j;
	    //DiffusionData *dnew = runDiffussion(rsc, RS, i+1, TS);
	    DiffusionData *dnew = runDiffussionSigned(rsc, RS, i+1, TS);
	    if(dnew->rac < min)
	    {
		min = dnew->rac;
		minNode = j;
	    }
	    delete dnew;
	}
	int delta = min - prevmin;
	if (delta < 0)
	{
	    prevmin = min;
	    TS[i] = minNode;
	    used[minNode] = 1;
	}
	else
	{
	    break;
	}
	i++;
    }
    delete dinit;
    return TS;
}
*/
// Mingreedy for signed graph

int * Graph::minGreedy(int rsc, int *RS, int k)
{
    int *TS = new int[k];
    int used[n] = {0};
    DiffusionData *dinit;
    for(int i = 0; i < rsc; i++)
    {
	used[RS[i]] = 1;
    }
    for(int i = 0; i < k; i++)
    {
	TS[i] = -1;
    }
    int i = 0;
    while(i < k)
    {
	int marked[n] = {0};
	dinit = runDiffussionSigned(rsc, RS, i, TS);
	int prevmin = dinit->rac;
	list <int> *RA = dinit->RA;
	list <int> *RAIN =new list<int>();
	list<int>::iterator iter,iter1;
	int min = n+1;
	int minNode;
	for(iter = RA->begin(); iter != RA->end(); iter++)
	{
	    int j = *iter;
	    marked[j] = 1;
	    RAIN->push_back(j);
	} 	
	for(iter = RA->begin(); iter != RA->end(); iter++)
	{
	    int i = *iter;
	    for(iter1 = radjList[i].begin(); iter1 != radjList[i].end(); iter1++)
	    {
		int j = *iter1;
		if(marked[j] == 1)
		{
		    continue;
		}
		RAIN->push_back(j);
		marked[j] = 1;
	    }
	}
	for(iter = RAIN->begin(); iter != RAIN->end(); iter++)
	    //for(iter = RA->begin(); iter != RA->end(); iter++)
	{
	    int j = *iter;
	    if(used[j] == 1)
	    {
		continue;
	    }
	    TS[i] = j;	    

	    DiffusionData *dnew = runDiffussionSigned(rsc, RS, i+1, TS);
	    
	    if(dnew->rac < min)
	    {
		min = dnew->rac;
		minNode = j;
	    }
	    delete dnew;
	}
	int delta = min - prevmin;
	if (delta < 0)
	{
	    prevmin = min;
	    TS[i] = minNode;
	    used[minNode] = 1;
	}
	else
	{
	    break;
	}
	i++;
    }
    delete dinit;
    return TS;
}


int * Graph::minGreedyProxy(int rsc, int *RS, int k)
{
    int *TS = new int[k];
    int used[n] = {0};
    for(int i = 0; i < rsc; i++)
    {
		used[RS[i]] = 1;		
    }
    for(int i = 0; i < k; i++)
    {
		TS[i] = -1;
    }
    int RSnFlag[n] = {0};
    for(int i = 0; i < rsc; i++)
    {
		list<int>::iterator iter;
		for(iter = this->adjList[RS[i]].begin(); iter != this->adjList[RS[i]].end(); iter++)
		{
		    int j = *iter;
		    //cout<<"j:"<<j;
		    if(used[j]==1) continue;
		    RSnFlag[j] = 1;
		}
    }
    DiffusionData *dinit = runDiffussionSigned(rsc, RS, 0, TS);
    int prevmin = dinit->rac;
    int i = 0;
    while(i < k)
    {
	int min = n+1;
	int minNode;
	for(int j = 0; j < n; j++)
	{
	    if(used[j] == 1)
	    {
		continue;
	    }
	    if(RSnFlag[j] == 0) continue;
	    TS[i] = j;
	    DiffusionData *dnew = runDiffussionSigned(rsc, RS, i+1, TS);
	    if(dnew->rac < min)
	    {
		min = dnew->rac;
		minNode = j;
	    }
	    delete dnew;
	}
	int delta = min - prevmin;
	if (delta < 0)
	{
	    prevmin = min;
	    TS[i] = minNode;
	    used[minNode] = 1;
	}
	else
	{
	    break;
	}
	i++;
    }
    delete dinit;
    return TS;
}
//Function to get max degree
int * Graph::getMaxDegNodes(int k)
{
    //int *degNodes = getInDegrees();
    int *degNodes = getOutDegrees();
    //cout<<"degNodes:"<<degNodes[1]<<endl;
    int *knodes = new int[k];
    for(int i = 0; i < k; i++)
    {
	int max = degNodes[0];
	int maxv = 0;
	knodes[i] = -1;
		for(int j = 1; j < n; j++)
		{ 
		    //if(degNodes[j] > max && p[j] == 4)
		    if(degNodes[j] > max)
		    {
			max = degNodes[j];
			maxv = j;
		    }
		}
	knodes[i] = maxv;
	degNodes[maxv] = -1;
    }
    return knodes;
}

//Function to get random nodes
int * Graph::getRandNodes(int k)
{
	int *knodes = new int[k];
	int dFlag[n] = {0};
	int j = 0;
	while(j<k)
	{
		int rnum  = rand()%n;		
		if(dFlag[rnum]==1) continue;
		dFlag[rnum]=1;
		knodes[j] = rnum;
		j++;
	}
	//cout<<"rnum"<<rnum<<endl;
	return knodes;
}

int * Graph::getRandTS(int k)
{
	int *TS = getRandNodes(k);
	
	return TS;
}


int * Graph::contrId(int rsc, int *RS, int k)
{
    int *knodes = new int[k];
    int RI[n] = {0};
    DiffusionData *dd = runDiffussionCtr(rsc, RS, RI); 
    int ctr[n] = {0};
    int ctrCopy[n] = {0};
    for(int i = 0; i < n; i++)
    {
	if(RI[i] == 1 || RI[i] == 0) continue;
	list<int>::iterator iter;
	for(iter = this->adjList[i].begin(); iter != this->adjList[i].end(); iter++)
	{
	    int j = *iter;
	    if(RI[j] > RI[i])
	    {
		ctr[i]++;
	    }
	}
    }
    for(int i = 0; i < k; i++)
    {
	int max = ctr[0];
	int maxv = 0;
	for(int j = 1; j < n; j++)
	{
	    if(ctr[j] > max)
	    {
		max = ctr[j];
		maxv = j;
	    }
	}
	knodes[i] = maxv;
	ctrCopy[maxv] = max;
	ctr[maxv] = -1;
    }
    return knodes;
}

int * Graph::contrIdProxy(int rsc, int *RS, int k)
{
    int *knodes = new int[k];
    int RI[n] = {0};
    DiffusionData *dd = runDiffussionCtr(rsc, RS, RI); 
    int ctr[n] = {0};
    int ctrCopy[n] = {0};
    for(int i = 0; i < n; i++)
    {
	if(RI[i] == 1 || RI[i] == 0) continue;
	list<int>::iterator iter;
	for(iter = this->adjList[i].begin(); iter != this->adjList[i].end(); iter++)
	{
	    int j = *iter;
	    if(RI[j] > RI[i])
	    {
		ctr[i]++;
	    }
	}
    }
    int RSnFlag[n] = {0};
    for(int i = 0; i < rsc; i++)
    {
	list<int>::iterator iter;
	for(iter = this->adjList[RS[i]].begin(); iter != this->adjList[RS[i]].end(); iter++)
	{
	    int j = *iter;
	    if(RI[j] == 1 || RI[j] == 0) continue;
	    RSnFlag[j] = 1;
	}
    }
    for(int i = 0; i < k; i++)
    {
	int max = ctr[0];
	int maxv = 0;
	knodes[i] = -1;
	for(int j = 1; j < n; j++)
	{
	    if(RSnFlag[j] == 0) continue;
	    if(ctr[j] > max)
	    {
		max = ctr[j];
		maxv = j;
	    }
	}
	knodes[i] = maxv;
	ctrCopy[maxv] = max;
	ctr[maxv] = -1;
    }
    return knodes;
}

int * Graph::topkCentralityProxy(char *file, int k, int rsc, int *RS, double f1, double f2)
{
    int *knodes = new int[k];
    //int n,m;
    char cenname[20];
    ifstream infile;
    infile.open(file);
    if(!infile)
    {
	cout << "Not able to open input file: " <<file;
    }
    /*if(!infile.eof())
    {
	infile >>n>>m;
	infile >> cenname;
	//cout<<cenname<<",";
    }*/
    double cen[n] = {0.0};
    while(!infile.eof())
    {
	int u;
	double eta, mu;
	infile >> u >> eta >> mu;
	cen[u] = (f1 * abs(eta)) + (f2 * abs(mu));	
    }
    /*while(!infile.eof())
    {   int u;
	double c;
	infile >>u>>c;
	cen[u] = c;
    }*/
    int rsFlag[n] = {0};
    for(int i = 0; i < rsc; i++)
    {
	rsFlag[RS[i]] = 1;
	cen[RS[i]] = -1;
    }

    int RSnFlag[n] = {0};
    for(int i = 0; i < rsc; i++)
    {
	list<int>::iterator iter;
	for(iter = this->adjList[RS[i]].begin(); iter != this->adjList[RS[i]].end(); iter++)
	{
	    int j = *iter;
	    if(rsFlag[j] == 1) continue;
	    RSnFlag[j] = 1;
	}
    }
    for(int i = 0; i < k; i++)
    {
	double max = -1;
	int maxv = -1;
	knodes[i] = -1;
	for(int j = 0; j < n; j++)
	{
	    if(RSnFlag[j] == 0) continue;
	    if(cen[j] > max)
	    {
		max = cen[j];
		maxv = j;
	    }
	}
	knodes[i] = maxv;
	cen[maxv] = -1;
    }
    return knodes; 
}

int * Graph::topkCentrality(char *file, int k, int rsc, int *RS, double f1, double f2)
{
    int *knodes = new int[k];
    //int n,m;
    char cenname[20];
    ifstream infile;
    infile.open(file);
    if(!infile)
    {
	cout << "Not able to open input file: " <<file;
    }
    /*if(!infile.eof())
    {
	infile >>n>>m;
	infile >> cenname;
	//cout<<cenname<<",";
    }*/
    double cen[n] = {0.0};
    while(!infile.eof())
    {
	int u;
	double eta, mu;
	infile >> u >> eta >> mu;
	cen[u] = (f1 * abs(eta)) + (f2 * abs(mu));	
    }
    /*while(!infile.eof())
    {   
    int u;
	double c;
	infile >>u>>c;
	cen[u] = c;
    }*/
    int rsFlag[n] = {0};
    for(int i = 0; i < rsc; i++)
    {
	rsFlag[RS[i]] = 1;
	cen[RS[i]] = -1;
    }
    for(int i = 0; i < k; i++)
    {
	double max = cen[0];
	int maxv = 0;
	knodes[i] = -1;
	for(int j = 1; j < n; j++)
	{
	    if(cen[j] > max)
	    {
		max = cen[j];
		maxv = j;
	    }
	}
	knodes[i] = maxv;
	cen[maxv] = -1;
    }
    return knodes; 
}

int Graph::generateRandNode(float cumprob[])
{
    int first = 0, last = n-1, mid;
    float randnum = (float) rand()/RAND_MAX;
    if(randnum <= cumprob[0])
    {
	return 0;
    }
    if(randnum >= cumprob[n-1])
    {
	return n-1;
    }
    while(first <= last)
    {
	mid = (first + last) / 2;
	if(cumprob[mid-1] < randnum && cumprob[mid] >= randnum)
	{
	    return mid;
	}
	else if(cumprob[mid] < randnum)
	{
	    first = mid + 1;
	}
	else
	{
	    last = mid - 1;
	}
    }
    return -1;
}


void insertSort(popwrc *population[], int q)
{
    while(q > 0 && population[q]->rc < population[q-1]->rc)
    {
	popwrc *temp = population[q-1];
	population[q-1] = population[q];
	population[q] = temp;
	q--;
    }
}
void Graph::generatePopulation(int k, int initPSize, int rsc, int *RS, popwrc *population[], float cumprob[], int rsFlag[])
{
    for(int i = 0; i < initPSize; i++)
    {
	int flag[n] = {0};
	int j = 0;
	while(j < k)
	{
	    int rnum = this->generateRandNode(cumprob);
	    if(flag[rnum] == 1) continue;
	    if(rsFlag[rnum] == 1) continue;
	    population[i]->pop[j] = rnum;
	    flag[rnum] = 1;
	    j++;
	}
	DiffusionData *mind = runDiffussionSigned(rsc, RS, k, population[i]->pop);
	population[i]->rc = mind->rac;
	delete mind;
	insertSort(population, i);
    }
}

void printArray(int l, int arr[])
{
    for(int i = 0; i < l; i++)
    {
	cout<<arr[i]<<" ";
    }
}

void Graph::Mutate(int k, int *pop, float cumprob[], int *flag)
{
    int mop = rand() % (k);
    int ele = pop[mop];
    bool cont = true;
    while(cont)
    {
	int rnum = this->generateRandNode(cumprob);
	if(flag[rnum] == 1) continue;
	pop[mop] = rnum;
	cont = false;
    }
}



void Graph::increasePopolationwithCM(int k, int initPSize, int totPSize, int rsc, int *RS, popwrc *population[], float cumprob[], int rsFlag[])
{
    for(int i = initPSize; i < totPSize; i = i + 2)
    {
	int ri1 = -1, ri2 = -1;
	do
	{
	    ri1 = rand() % initPSize;
	}while(rsFlag[ri1] == 1);
	do
	{
	    ri2 = rand() % initPSize;
	}while(ri2 == ri1 || rsFlag[ri2] == 1);

	int cop = rand() % (k - 1);
	int flag1[n] = {0}, flag2[n] = {0};
	for(int j = 0; j <= cop; j++)
	{
	    int node = population[ri1]->pop[j];
	    population[i]->pop[j] = node;
	    flag1[node] = 1;
	}
	int q = cop + 1;
	for(int j = cop + 1; j < k; j++)
	{
	    int node = population[ri2]->pop[j];
	    if(flag1[node] == 1) continue;
	    population[i]->pop[q] = node;
	    flag1[node] = 1;
	    q++;
	}
	while(q < k)
	{
	    int rnum = this->generateRandNode(cumprob);
	    if(flag1[rnum] == 1) continue;
	    population[i]->pop[q] = rnum;
	    flag1[rnum] = 1;
	    q++;
	}
	Mutate(k, population[i]->pop, cumprob, flag1);
	DiffusionData *mind = runDiffussionSigned(rsc, RS, k, population[i]->pop);
	population[i]->rc = mind->rac;
	delete mind;
	for(int j = 0; j <= cop; j++)
	{
	    int node = population[ri2]->pop[j];
	    population[i+1]->pop[j] = node;
	    flag2[node] = 1;
	}
	q = cop + 1;
	for(int j = cop + 1; j < k; j++)
	{
	    int node = population[ri1]->pop[j];
	    if(flag2[node] == 1) continue;
	    population[i+1]->pop[q] = node;
	    flag2[node] = 1;
	    q++;
	}
	while(q < k)
	{
	    int rnum = this->generateRandNode(cumprob);
	    if(flag2[rnum] == 1) continue;
	    population[i+1]->pop[q] = rnum;
	    flag2[rnum] = 1;
	    q++;
	}
	Mutate(k, population[i+1]->pop, cumprob, flag2);
	mind = runDiffussionSigned(rsc, RS, k, population[i+1]->pop);
	population[i+1]->rc = mind->rac;
	delete mind;
	insertSort(population, i);
	insertSort(population, i+1);
    }
}

float * Graph::getEcentrality(char *cFileName, int rsc, int *RS, double f1, double f2)
{
    double centVal[n] = {0.0};    
    ifstream inFile;
    inFile.open(cFileName);
    if(!inFile)
    {
	cout << "Not able to open input file: " <<cFileName;
	return 0;
    }
    while(!inFile.eof())
    {
	int u;
	double eta, mu;
	inFile >> u >> eta >> mu;
	centVal[u] = (f1 * abs(eta)) + (f2 * abs(mu));	
    } 
    int signCount[n];
    for(int i = 0; i < n; i++)
    {
	signCount[i] = -1;
    }

    int level[n] = {0};
    int maxLevel = 0;
    list<int> *queue = new list<int>();
    for(int i = 0; i < rsc; i++)
    {
	signCount[RS[i]] = 0;
	level[RS[i]] = 1;
	queue->push_back(RS[i]);
	centVal[RS[i]] = 0.0;
    }
    while(!queue->empty())
    {
	int u = queue->front();
	queue->pop_front();
	list<int>::iterator iter;
	for(iter = this->adjList[u].begin(); iter != this->adjList[u].end(); iter++)
	{
	    int v = *iter;
	    if(level[v] > 0)continue;
	    if(weightMat[u][v] < 0)
	    {
		signCount[v] = signCount[u] + 1;
	    }
	    else
	    {
		signCount[v] = signCount[u];
	    }
	    level[v] = level[u] + 1;
	    queue->push_back(v);
	    if(level[v] > maxLevel)
	    {
		maxLevel = level[v];
	    }
	}
	if(level[u] > 1)
	{
	    for(iter = this->radjList[u].begin(); iter != this->radjList[u].end(); iter++)
	    {
		int v = *iter;
		if(level[v] > 0)continue;
		level[v] = level[u] + 1;
		queue->push_back(v);
		if(level[v] > maxLevel)
		{
		    maxLevel = level[v];
		}
	    }
	}
    }
    float centTot = 0.0;
    for(int i = 0; i < n; i++)
    {
	centVal[i] = centVal[i] * (maxLevel - level[i] + 1);
	if(signCount[i] % 2 == 1)
	{
	    centVal[i] = centVal[i] / 2;
	}
	centTot += centVal[i];
    }
    float *cumprob = new float[n];
    memset(cumprob, 0.0, n * sizeof(float));
    for(int i = 0; i < n; i++)
    {
	centVal[i] /= centTot;
	if(i == 0)
	{
	    cumprob[0] = centVal[0];
	}
	else
	{
	    cumprob[i] = cumprob[i-1] + centVal[i]; 
	}
    }
    return cumprob;
}

int * Graph::GA(char *cFileName, int rsc, int *RS, int k, int initPSize, int totPSize, int noIters, double f1, double f2)
{
    int *knodes = new int[k];
    float *cumprob = getEcentrality(cFileName, rsc, RS, f1, f2);
    popwrc *population[totPSize];
    for(int i = 0; i < totPSize; i++)
    {
	population[i] = new popwrc(k);
    }
    int rsFlag[n] = {0};
    for(int i = 0; i < rsc; i++)
    {
	rsFlag[RS[i]] = 1;
    }
    
    generatePopulation(k, initPSize, rsc, RS, population, cumprob, rsFlag);
    for(int i = 0; i < noIters; i++)
    {
	//cout<<"Iteration-"<<i+1<<": "<<population[0]->rc<<endl;
	increasePopolationwithCM(k, initPSize, totPSize, rsc, RS, population, cumprob, rsFlag);
    }
    delete [] cumprob;
    return population[0]->pop;
}

int * Graph::topkECentrality(char *cFileName, int rsc, int * RS, int k, double f1, double f2)
{
    int *knodes = new int[k];
    //int n,m;
    char cenname[20];
    ifstream infile;
    infile.open(cFileName);
    if(!infile)
    {
	cout << "Not able to open input file: " <<cFileName;
    }
    /*if(!infile.eof())
    {
	infile >>n>>m;
	infile >> cenname;
	//cout<<"E-"<<cenname<<",";
    }*/
    float *ecen = getEcentrality(cFileName, rsc, RS, f1, f2);    
    for(int i = 0; i < k; i++)
    {
	double max = ecen[0];
	int maxv = 0;
	knodes[i] = -1;
	for(int j = 1; j < n; j++)
	{
	    if(ecen[j] > max)
	    {
		max = ecen[j];
		maxv = j;
	    }
	}
	knodes[i] = maxv;
	ecen[maxv] = -1;
    }
    delete [] ecen;
    return knodes; 
}

int * Graph::randPruneSigned(char * cFileName, int rsc, int * RS, int tsc, int Iter, double f1, double f2)
{
    float *ecen = getEcentrality(cFileName, rsc, RS, f1, f2);
    int flag[n] = {0};
    int minrcount = n+1;
    int *minTS = NULL;
    for(int i = 0; i < Iter; i++)
    {
	int *TS = new int[tsc];
	memset(TS, 0.0, tsc * sizeof(int));
	int j = 0;
	while(j < tsc)
	{
	    int rnum = this->generateRandNode(ecen);
	    if(flag[rnum] == 1) continue;	    
	    TS[j] = rnum;
	    flag[rnum] = 1;
	    j++;
	}
	TS = pruning(rsc, RS, tsc, TS);
	DiffusionData *dd = runDiffussionSigned(rsc, RS, tsc, TS);
	if(dd->rac < minrcount)
	{
	    minrcount = dd->rac;
	    if(minTS != NULL)
	    {
		delete [] minTS;
	    }
	    minTS = TS;
	}
	else
	{
	    delete [] TS;
	}
	delete dd;
    }
    delete [] ecen;
    return minTS;
}

