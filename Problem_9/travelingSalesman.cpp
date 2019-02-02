#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

// 2D Matrix of boolean variables
typedef IloArray<IloBoolVarArray> BoolVarMatrix;

int main() {
    
    IloEnv env;
    IloModel travelingSalesman(env, "Traveling Salesman Problem");
    IloCplex cplex(travelingSalesman);

    // Statement Data:
    // The number of cities (nodes) in the graph
    int n;
    scanf("%d", &n);

    // Generate random distances between n cities (nodes)
    double distance[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                distance[i][j] = 0;
            } else {
                distance[i][j] = static_cast <float> (rand()) / 
                    (static_cast <float> (RAND_MAX / 100));
            }
        }
    }

    // Decision Variables:
    // If there is a path between cities i and j
    IloNumVarArray s(env, n);
    BoolVarMatrix vars(env, n);
    
    for (int i = 0; i < n; i++) {
        s[i] = IloNumVar(env);
        vars[i] = IloBoolVarArray(env, n);

        for (int j = 0; j < n; j++) {
            vars[i][j] = IloBoolVar(env);
        }
    }

    // Constraints:
    // Unique output
    for (int i = 0; i < n; i++) {
        IloExpr sum(env);

        for (int j = 0; j < n; j++) {
            if (i == j) {} else {
                sum += vars[i][j];
            }
        }

        travelingSalesman.add(sum == 1);
    }

    // Constraints:
    // Unique input
    for (int j = 0; j < n; j++) {
        IloExpr sum2(env);

        for (int i = 0; i < n; i++) {
            if (i == j) {} else {
                sum2 += vars[i][j];
            }
        }

        travelingSalesman.add(sum2 == 1);
    }

    // Constraints:
    // Remove subcycles
    for (int i = 1; i < n; i++) {
        for (int j = 1; j < n; j++) {
            if (i == j) {} else {
                IloExpr exp(env);
                exp = s[i] - s[j] + (n - 1) * vars[i][j];
                travelingSalesman.add(exp <= (n - 2));
            }
        }
    }

    // Objective Function:
    // Minimize the cost to go through each city
    IloExpr objective(env);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            objective += distance[i][j] * vars[i][j];
        }
    }
    
    travelingSalesman.add(IloMinimize(env, objective));

    // Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;

    // End
    cplex.end();
    travelingSalesman.end();
    env.end();

}
