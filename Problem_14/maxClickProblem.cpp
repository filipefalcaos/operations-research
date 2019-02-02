#include <stdio.h>
#include <set>
#include <math.h>
#include <ilcplex/ilocplex.h>
using namespace std;

// Adjacency list of a graph
typedef set<int> adjlist;

int main() {
    
    IloEnv env;
    IloModel maxClickProblem(env, "Max Click Problem");
    IloCplex cplex(maxClickProblem);

    // Statement Data:
    // Set the graph size
    int n, m;
    scanf("%d %d", &n, &m);
    adjlist graph[n];
    adjlist revGraph[n];

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

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (!graph[i].count(j)) {
                revGraph[i].insert(j);
                revGraph[j].insert(i);
            }
        }  
    }

    // Decision Variables:
    // If the node i belongs to the max clique
    IloBoolVarArray x(env, n);

    // Constraints:
    // Check if a click exists
    for (int i = 0; i < n; i++) {
        for (int j: revGraph[i]) {
            maxClickProblem.add(x[i] + x[j] <= 1);
        }
    }
        
    // Objective Function:
    // Maximize the number of node in a clique
    IloExpr sum(env);
    for (int i = 0; i < n; i++) {
        sum += x[i];
    }
    maxClickProblem.add(IloMaximize(env, sum));

    // Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;

    for (int i = 0; i < n; i++) {
        cout << "x" << i << " -> " << cplex.getValue(x[i]) << endl;
    }

    // End
    cplex.end();
    maxClickProblem.end();
    env.end();   

}