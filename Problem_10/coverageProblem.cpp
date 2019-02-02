#include <stdlib.h>
#include <ilcplex/ilocplex.h>
#include <math.h>
#include <set>
using namespace std;

// Adjacency list of a graph
typedef set<int> adjlist;

int main() {
    
    IloEnv env;
    IloModel coverageProblem(env, "Coverage Problem");
    IloCplex cplex(coverageProblem);

    // Statement Data:
    // Set the graph size
    int n, m;
    scanf("%d %d", &n, &m);
    adjlist graph[n];
    int constructions_costs[n];

    // Generate random costs
    for (int i = 0; i < n; i++){
        constructions_costs[i] = rand() % 100;
    }

    // Set the graph
    for (int i = 0; i < m; i++) {
        int u, v;
        u = rand() % n;
        v = rand() % n;

        if (u != v) {
            graph[u].insert(v);
            graph[v].insert(u);
        }
    }

    // Decision Variables:
    // If a school will be contructed in j
    IloBoolVarArray x(env, n);
        
    // Constraints:
    // Each neighborhood needs to have a school
    for (int i = 0; i < n; i++) {
        IloExpr service(env);
        service += x[i];

        for (int j : graph[i]) {
            service += x[j];
        }

        coverageProblem.add(service >= 1);
    }

    // Objective Function:
    // Minimize the construction costs
    IloExpr cost(env);
    for (int i = 0; i < n; i++) {
        cost += x[i] * constructions_costs[i];
    }
    coverageProblem.add(IloMinimize(env, cost));

    // Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;

    for (int i = 0; i < n; i++) {
        cout << "x" << i << " -> " << cplex.getValue(x[i]) << endl;
    }

    // End
    cplex.end();
    coverageProblem.end();
    env.end();

}