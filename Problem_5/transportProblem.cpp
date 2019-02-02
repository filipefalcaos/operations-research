#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

// 2D Matrix of numerical variables
typedef IloArray<IloNumVarArray> NumVarMatrix;

int main() {
    
    IloEnv env;
    IloModel transportProblem(env, "Transport Problem");
    IloCplex cplex(transportProblem);

    // Statement Data:
    // Data provided by the problem
    const int n = 3, m = 3;
    double amount_n[n] = {120, 80, 80};
    double amount_m[m] = {150, 70, 60};
    double costs[n][m] = {{8, 5, 6}, {15, 10, 12}, {3, 9, 10}};

    // Decision Variables:
    // Amount of products from warehouse i to factory j
    NumVarMatrix qtd_ij(env, n);
    for (int i = 0; i < n; i++) {
        qtd_ij[i] = IloNumVarArray(env, m);

        for (int j = 0; j < m; j++) {
            qtd_ij[i][j] = IloNumVar(env);
        }
    }

    // Constraints:
    // Deal with max possible amount of products in warehouses
    for (int i = 0; i < n; i++) {
        IloExpr constraint(env);

        for (int j = 0; j < m; j++) {
            constraint += qtd_ij[i][j];
            transportProblem.add(qtd_ij[i][j] >= 0); // Non-negativity
        }
        
        transportProblem.add(constraint <= amount_n[i]);
    }

    // Constraints:
    // Deal with max possible amount of products in factories
    for (int j = 0; j < n; j++) {
        IloExpr constraint(env);

        for (int i = 0; i < m; i++) {
            constraint += qtd_ij[i][j];
            transportProblem.add(qtd_ij[i][j] >= 0); // Non-negativity
        }
        
        transportProblem.add(constraint == amount_m[j]);
    }

    // Objective Function:
    // Minimize the cost
    IloExpr objective(env);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            objective += qtd_ij[i][j] * costs[i][j];
        }
    }
    transportProblem.add(IloMinimize(env, objective));

    // Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << "x" << i << j << ": " << cplex.getValue(qtd_ij[i][j]) << endl;
        }
    }

    // End
    cplex.end();
    transportProblem.end();
    env.end();

}
