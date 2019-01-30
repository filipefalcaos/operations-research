#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
    
    IloEnv env;
    IloModel nurseProblem(env, "Nurse Problem");
    IloCplex cplex(nurseProblem);

    // Statement Data:
    int n;
    scanf("%d", &n);
    
    int days_demand[n];
    for (int i = 0; i < n; i++) {
        days_demand[i] = rand() % 10 + 1; 
    }

    // Decision Variables:
    IloNumVarArray x(env, n);
    for (int i = 0; i < n; i++) {
        x[i] = IloNumVar(env);
    }

    // Restrictions:
    for (int i = 0; i < n; i++) {
        nurseProblem.add(x[i] >= 0);

        if (i >= 5) {
            nurseProblem.add(x[i] + x[i - 1] + x[i - 2] + x[i - 3] + x[i - 4] 
                >= days_demand[i]);
        }
    }

    // Objective Function:
    IloExpr objective(env);
    for (int i = 0; i < n; i++) {
        objective += x[i];
    }
    nurseProblem.add(IloMinimize(env, objective));

    // Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;

    for (int i = 0; i < n; i++) {
        cout << "x" << i << " -> " << cplex.getValue(x[i]) << endl;
    }

    // End
    cplex.end();
    nurseProblem.end();
    env.end();

}