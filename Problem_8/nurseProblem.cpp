#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
    
    IloEnv env;
    IloModel nurseProblem(env, "Nurse Problem");
    IloCplex cplex(nurseProblem);

    // Statement Data:
    // Number of days
    const int week = 7, working_days = 5;
    int n;
    scanf("%d", &n);
    
    // Generate random demands for each day in a week
    int days_demand[week];
    for (int i = 0; i < n; i++) {
        days_demand[i] = rand() % 10 + 1; 
    }

    // Decision Variables:
    // Number of n_nurses in day i
    IloNumVarArray x(env, week);
    for (int i = 0; i < week; i++) {
        x[i] = IloNumVar(env);
    }

    // Constraints:
    // Handle the demands
    for (int i = 0; i < week; i++) {
        IloExpr n_nurses(env);
        nurseProblem.add(x[i] >= 0); // Non-negativity

        for (int j = 0; j < working_days; j++) {
            n_nurses += x[(i + j) % week];
            nurseProblem.add(n_nurses >= days_demand[(i + working_days - 1) % week]);
        } 
    }

    // Objective Function:
    // Minimize the number of nurses
    IloExpr objective(env);
    for (int i = 0; i < week; i++) {
        objective += x[i];
    }
    nurseProblem.add(IloMinimize(env, objective));

    // Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;

    for (int i = 0; i < week; i++) {
        cout << "x" << i << " -> " << cplex.getValue(x[i]) << endl;
    }

    // End
    cplex.end();
    nurseProblem.end();
    env.end();

}