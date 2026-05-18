#include<iostream>
#include<list>
#include<fstream>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include "Graph.h"
using namespace std;

static void printList(list<int> *lst)
{
    cout<<""<<lst->size()<<"\n";
    list<int>::iterator iter;	
    for(iter = lst->begin(); iter != lst->end(); iter++)
    {
	int v = *iter;
	cout<<v<<", ";
    }
}

static int adjustTS(int tsc, int *TS)
{
    for(int i = 0; i < tsc; i++)
    {
	if(TS[i] == -1) 
	{
	    return i;
	}
    }
    return tsc;
}

int main(int argc, char ** argv)
{
    int n, m;
    ifstream inFile;
    inFile.open(argv[1]);
    if(!inFile)
    {
        cout << "Not able to open input file: " << argv[1];
        return 0;
    }
    srand(2);
    if(!inFile.eof())
    {
        inFile >> n >> m;
        //cout<<n<<m;
    }
    Graph *g = new Graph(n);
   
    while(!inFile.eof())
    {
        int u, v, s;
        float w;
        inFile >> u >> v >> w >> s;
        //cout<<"uvw"<<u<<":"<<v<<":"<<w;
        if(u == v)
            continue;
        if(!inFile.eof())
		{
            if(s == 1)
            {
                g->addEdge(u, v, w, 1.0);
            }
            if(s == -1)
            {
                g->addEdge(u, v, -w, 1.0);
            }
            
		}
    }
    //g->updateWeights();
    //g->deltaValues();
    int * a = NULL;
    //a = g->getPartitionedGraph();
    //cout<<"a:" <<a;
    if(a!= NULL)
    {
    	cout<<"Balanced Graph\n";
    	// for(int i = 0; i < n; i++)
    	// {
    	// 	cout<<"a["<<i<<"]:"<<a[i]<<endl;
    	// }
    }
    else
    {
    	//cout<<"Unbalanced Graph!"<<endl;
    }
    int k = 5;
    // int *RS,*TS,rsc,tsc;
    // rsc = 5;
    // tsc = 0;
    // TS = NULL;
    // RS = new int[rsc];
    // //g->setRandomTH();
    // g->setFixedTH(0.3, 0.4);
    //int * p = g->multistartRelocationHeuristic(k,120);
    // if(p!= NULL)
    // {
    // 	for(int i = 0; i < n; i++)
    // 	{
    // 		//cout<<"p["<<i<<"]:"<<p[i]<<endl;
    // 	}
    // }
    //int * f = g->getFrequency(p,k);
    // if(f!= NULL)
    // {
    // 	for(int i = 1; i <= k; i++)
    // 	{
    // 		cout<<"f["<<i<<"]:"<<f[i]<<endl;
    // 	}
    // }
    // for(int i = 1; i <= k; i++)
    // {
    // 	for(int j = 0; j < n; j++)
    // 	{
    // 		if(f[j] == i && p[j] == 4)
    // 		{
    // 			int outDeg = g->adjList[j].size();
    // 			cout<<"i:"<<j<<" outDeg:"<<outDeg<<endl;
    // 		}
    // 	}
    // }

    //RS = g->getMaxDegNodes(rsc,p);
    // for(int i = 0; i < rsc; i++)
    // {
    // 	cout<<"RS["<<i<<"]:"<<RS[i]<<endl;
    // }
    //if(p!= NULL)
    //{
    	//float ZP = 0.0; 
    	//ZP = g->calculateZPValues(p);
    	//cout<<"ZP:"<<ZP<<endl;
    //}
    // clock_t time_req;
    // DiffusionData *dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
    // cout<<dd->rac+rsc<<","<<dd->tac+tsc<<endl;
    // cout<<"minGreedy Results:"<<endl;
    // time_req = clock();
    // int *mts = g->minGreedy(rsc, RS, k);
    // time_req = clock() - time_req;
    // tsc = 5;
    // dd = g->runDiffussionSigned(rsc, RS, tsc, mts);
    // cout<<"RC "<<dd->rac<<","<<"TC "<<dd->tac<<" Time "<<(float)time_req/CLOCKS_PER_SEC<<endl;
    // cout<<"modiMinGreedy Results:"<<endl;
    // time_req = clock();
    // int *mmts = g->modiMinGreedy(rsc, RS, k);
    // time_req = clock() - time_req;
    // dd = g->runDiffussionSigned(rsc, RS, tsc, mmts);
    // cout<<"RC "<<dd->rac<<","<<"TC "<<dd->tac<<" Time "<<(float)time_req/CLOCKS_PER_SEC<<endl;
    
  
    int rsc = atoi(argv[2]);
    int tsc = atoi(argv[3]);
    int tsc1 = tsc;
    float it = atof(argv[4]);
    float dt = atof(argv[5]);

    if(!strcmp(argv[4], "randitdt"))
    {
	g->setRandomTH();
	//cout<<"randit,"<<"randdt,";
    }
    else
    {
	g->setFixedTH(it, dt);
	//cout<<it<<","<<dt<<",";
    }
    int *RS;
    if(!strcmp(argv[6], "randseed"))
    {
	RS = g->getRandNodes(rsc);
	//cout<<"randseed,";
    }
    else
    {
	RS = g->getMaxDegNodes(rsc);
    //RS = g->getMaxDegNodes(rsc,p);
	//cout<<"maxdegree,";
    }
    bool runmg = false, runpmg = false, runctrid = false, runpctrid = false, runcen = false, 
    runpcen = false, runecen = false, runga = false, runrandprune = false, runall = false, centrlty = false,rundiff = false;
    
    if(!strcmp(argv[7], "diffusion"))
    {
	rundiff = true;
    }
    else if(!strcmp(argv[7], "mingreedy"))
    {
	runmg = true;
    }
    else if(!strcmp(argv[7], "proxymingreedy"))
    {
	runpmg = true;
    }
    else if(!strcmp(argv[7], "contrid"))
    {
	runctrid = true;
    }
    else if(!strcmp(argv[7], "proxycontrid"))
    {
	runpctrid = true;
    }
    else if(!strcmp(argv[7], "centrality"))
    {
	centrlty = true;
	runcen = true;
    }
    else if(!strcmp(argv[7], "proxycentrality"))
    {
	centrlty = true;
	runpcen = true;
    }
    else if(!strcmp(argv[7], "ecentrality"))
    {
	centrlty = true;
	runecen = true;
    }
    else if(!strcmp(argv[7], "ga"))
    {
	centrlty = true;
	runga = true;
    }
    else if(!strcmp(argv[7], "randpruning"))
    {
	centrlty = true;
	runrandprune = true;
    }
    else if(!strcmp(argv[7], "runall"))
    {
	centrlty = true;
	runall = true;
    }
    clock_t time_req;
    int *TS = NULL;
    if(rundiff)
    {
	time_req = clock();
	DiffusionData *dd = g->runDiffussionSigned(rsc, RS, 0, TS);
	time_req = clock() - time_req;
	cout<<argv[1]<<","<<argv[2]<<","<<argv[3]<<","<<argv[4]<<","<<argv[5]<<","<<argv[6]<<","<<"Diffusion"<<","<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<",";
	delete dd;
    }


    if(runmg || runall)
    {
	//cout<<"Started MinGreedy"<<endl;
	time_req = clock();
	tsc = tsc1;
	TS = g->minGreedy(rsc, RS, tsc);
	time_req = clock() - time_req;
	tsc = adjustTS(tsc, TS);
	DiffusionData *dd = g->runDiffussionSigned(rsc, RS, tsc, TS);

	cout<<argv[1]<<","<<argv[2]<<","<<argv[3]<<","<<argv[4]<<","<<argv[5]<<","<<argv[6]<<","<<"mingreedy"<<","<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<",";
	delete dd;
 
	time_req = clock();
	TS = g->pruning(rsc, RS, tsc, TS);
	time_req = clock() - time_req;
	dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
        cout<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<endl;
	delete dd;
	delete [] TS;
    }
    if(runpmg || runall)
    {
	//cout<<"Started Proxy MinGreedy"<<endl;
	time_req = clock();
	tsc = tsc1;
	TS = g->minGreedyProxy(rsc, RS, tsc);
	time_req = clock() - time_req;
	tsc = adjustTS(tsc, TS);
	DiffusionData *dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
	cout<<argv[1]<<","<<argv[2]<<","<<argv[3]<<","<<argv[4]<<","<<argv[5]<<","<<argv[6]<<","<<"proxymingreedy"<<","<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<",";
	delete dd;

	time_req = clock();
	TS = g->pruning(rsc, RS, tsc, TS);
	time_req = clock() - time_req;
	dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
        cout<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<endl;
	delete dd;
	delete [] TS;
    }

    if(runctrid || runall)
    {
	//cout<<"Started Contr ID"<<endl;
	time_req = clock();
	tsc = tsc1;
	TS = g->contrId(rsc, RS, tsc);
	time_req = clock() - time_req;
	tsc = adjustTS(tsc, TS);
	DiffusionData *dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
	cout<<argv[1]<<","<<argv[2]<<","<<argv[3]<<","<<argv[4]<<","<<argv[5]<<","<<argv[6]<<","<<"contrid"<<","<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<",";
	delete dd;

	time_req = clock();
	TS = g->pruning(rsc, RS, tsc, TS);
	time_req = clock() - time_req;
	dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
    cout<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<endl;
	delete dd;
	delete [] TS;
    }
    if(runpctrid || runall)
    {
	//cout<<"Started Proxy Contr ID"<<endl;
	time_req = clock();
	tsc = tsc1;
	TS = g->contrIdProxy(rsc, RS, tsc);
	time_req = clock() - time_req;
	tsc = adjustTS(tsc, TS);
	DiffusionData *dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
	cout<<argv[1]<<","<<argv[2]<<","<<argv[3]<<","<<argv[4]<<","<<argv[5]<<","<<argv[6]<<","<<"proxycontrid"<<","<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<",";
	delete dd;

	time_req = clock();
	TS = g->pruning(rsc, RS, tsc, TS);
	time_req = clock() - time_req;
	dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
        cout<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<endl;
	delete dd;
	delete [] TS;
    }
    if(runcen || runall)
    {
	//cout<<"Started Centrality"<<endl;
	time_req = clock();
	tsc = tsc1;
	TS = g->topkCentrality(argv[8], tsc, rsc, RS, atof(argv[12]), atof(argv[13]));
	time_req = clock() - time_req;
	tsc = adjustTS(tsc, TS);
	DiffusionData *dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
	cout<<argv[1]<<","<<argv[2]<<","<<argv[3]<<","<<argv[4]<<","<<argv[5]<<","<<argv[6]<<","<<"centrality"<<","<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<",";
	delete dd;

	time_req = clock();
	TS = g->pruning(rsc, RS, tsc, TS);
	time_req = clock() - time_req;
	dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
	cout<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<","<<argv[8]<<endl;
	delete dd;
	delete [] TS;
    }
    if(runpcen || runall)
    {
	//cout<<"Started Proxy Centrality"<<endl;
	time_req = clock();
	tsc = tsc1;
	TS = g->topkCentralityProxy(argv[8], tsc, rsc, RS, atof(argv[12]), atof(argv[13]));
	time_req = clock() - time_req;
	tsc = adjustTS(tsc, TS);
	DiffusionData *dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
	cout<<argv[1]<<","<<argv[2]<<","<<argv[3]<<","<<argv[4]<<","<<argv[5]<<","<<argv[6]<<","<<"proxycentrality"<<","<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<",";
	delete dd;

	time_req = clock();
	TS = g->pruning(rsc, RS, tsc, TS);
	time_req = clock() - time_req;
	dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
	cout<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<","<<argv[8]<<endl;
	delete dd;
	delete [] TS;
    }
    
    if(runga || runall)
    {
	//cout<<"Started GA"<<endl;
	time_req = clock();
	tsc = tsc1;
	TS = g->GA(argv[8], rsc, RS, tsc, atoi(argv[9]), atoi(argv[10]), atoi(argv[11]), atof(argv[12]), atof(argv[13]));
	time_req = clock() - time_req;
	tsc = adjustTS(tsc, TS);
	DiffusionData *dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
	cout<<argv[1]<<","<<argv[2]<<","<<argv[3]<<","<<argv[4]<<","<<argv[5]<<","<<argv[6]<<","<<"ga"<<","<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<",";
	delete dd;

	time_req = clock();
	TS = g->pruning(rsc, RS, tsc, TS);
	time_req = clock() - time_req;
	dd = g->runDiffussionSigned(rsc, RS, tsc, TS);
	cout<<dd->rac+rsc<<","<<dd->tac+tsc<<","<<(float)time_req/CLOCKS_PER_SEC<<","<<argv[9]<<endl;
	delete dd;
	delete [] TS;
    }
}
