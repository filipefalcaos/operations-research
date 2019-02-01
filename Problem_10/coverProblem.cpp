#include <stdlib.h>
#include <ilcplex/ilocplex.h>
#include <math.h>
#include <set>

using namespace std;

#define MAX_SIZE 5

typedef set<int> adjlist;

adjlist graph[MAX_SIZE];
int installation_cost[MAX_SIZE];

int main(){
    IloEnv env;
    IloModel coverProblem(env, "Cover Problem");
    IloCplex cplex(coverProblem);

    // Statement Data:
    srand((unsigned) time(NULL));

    const int MAX_EDGES = 10;

    for (int i = 0; i < MAX_SIZE; ++i){
        installation_cost[i] = rand() % 100;
    }

    for(int i = 0; i < MAX_EDGES; ++i){
        int u, v;
        u = rand() % MAX_SIZE;
        v = rand() % MAX_SIZE;

        if (u != v){
            graph[u].insert(v);
            graph[v].insert(u);
        }
    }

    // Decision Variables:
    IloBoolVarArray x(env, MAX_SIZE);
        
    // Restrictions:
    for(int i = 0; i < MAX_SIZE; ++i){
        IloExpr service(env);
        service += x[i];

        for (int j : graph[i]){
            service += x[j];
        }

        coverProblem.add(service >= 1);
    }

    // Objective Function:
    IloExpr cost(env);
    for(int i = 0; i < MAX_SIZE; ++i){
        cost += x[i] * installation_cost[i];
    }

    coverProblem.add(IloMinimize(env, cost));

    if (cplex.solve()){
        cout << "Min Cost: " << cplex.getObjValue() << endl;
    }

    // Get Solution:
    IloNumArray sol(env, MAX_SIZE);
    cplex.getValues(sol, x);

    cout << endl;

    cout << "Cost: " << endl;
    for(int i = 0; i < MAX_SIZE; ++i){
        cout << i << " " << installation_cost[i] << endl;
    }

    cout << endl; 
    cout << "Graph:" << endl;
    for(int i = 0; i < MAX_SIZE; ++i){
        cout << i << " -> ";
        for (int node : graph[i]){
                cout << " " << node << " ";
        }

        cout << endl;
    }

    cout << endl;
    
    for(int i = 0; i < MAX_SIZE; ++i){
        if (sol[i]){
            cout << i << endl;
        }
    }

    // End
    cplex.end();
    coverProblem.end();
    env.end();
}