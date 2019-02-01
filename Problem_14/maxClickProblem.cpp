#include <stdio.h>
#include <set>
#include <time.h>
#include <math.h>
#include <ilcplex/ilocplex.h>

using namespace std;

#define MAX_SIZE 10

typedef set<int> adjlist;

adjlist graph[MAX_SIZE];
adjlist revGraph[MAX_SIZE];

int main(){
    IloEnv env;
    IloModel maxClickProblem(env, "Max Click Problem");
    IloCplex cplex(maxClickProblem);

    srand((unsigned) time(NULL));

    const int MAX_EDGES = 5;

    for(int i = 0; i < MAX_EDGES; ++i){
        int u, v;
        u = rand() % MAX_SIZE;
        v = rand() % MAX_SIZE;

        if (u != v) {
            graph[u].insert(v);
            graph[v].insert(u);
        }
    }

    for (int i = 0; i < MAX_SIZE; ++i){
        for (int j = i + 1; j < MAX_SIZE; ++j){
            if (!graph[i].count(j)){
                revGraph[i].insert(j);
                revGraph[j].insert(i);
            }
        }  
    }

    // Decision Variables:
    IloBoolVarArray x(env, MAX_SIZE);


    // Restrictions:
    for (int i = 0; i < MAX_SIZE; ++i){
        for (int j: revGraph[i]){
            maxClickProblem.add(x[i] + x[j] <= 1);
        }
    }
        
    // Objective Function:
    IloExpr totalVertices(env);
    for (int i = 0; i < MAX_SIZE; ++i){
        totalVertices += x[i];
    }
    maxClickProblem.add(IloMaximize(env, totalVertices));

    // Get Solution:
    cplex.solve();
    IloNumArray solution(env); 
    cplex.getValues(solution, x);
    int clickSize = 0;

    for (int i = 0; i < MAX_SIZE; ++i) {
        clickSize += solution[i];
    }

    printf("Max Click : %d\n", clickSize);
    
    for (int i = 0; i < MAX_SIZE; ++i){
        if (solution[i]){
            printf("Vertex %d\n", i + 1);
        }
    }

    // End
    cplex.end();
    maxClickProblem.end();
    env.end();   

}