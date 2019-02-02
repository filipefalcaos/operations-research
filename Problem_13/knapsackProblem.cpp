#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
    
    IloEnv env;
    IloModel knapsackProblem(env, "Knapsack Problem");
    IloCplex cplex(knapsackProblem);

    // Statement Data:
    // Number of items and knapsack weight
    int n;
    double weight;
    scanf("%d %lf", &n, &weight);
    
    // Values and weights of each item
    double weights[n];
    double values[n];

    for (int i = 0; i < n; i ++) {
        scanf("%lf %lf", &weights[i], &values[i]);
    }

    // Decision Variables:
    // If an item is in the knapsack
    IloBoolVarArray vars(env, n);
    for (int i = 0; i < n; i++) {
        vars[i] = IloBoolVar(env);
    }

    // Constraints:
    // The wight of the knapsack cannot be exceeded
    IloExpr sum(env);
    for (int i = 0; i < n; i++) {
        sum += weights[i] * vars[i];
    }
    knapsackProblem.add(sum <= weight);

    // Objective Function:
    // Maximize the value of the itens in the knapsack
    IloExpr objective(env);
    for (int i = 0; i < n; i++) {
        objective += values[i] * vars[i];
    }
    knapsackProblem.add(IloMaximize(env, objective));

    // Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;
    
    for (int i = 0; i < n; i++) {
        cout << "x" << i << " -> " << cplex.getValue(vars[i]) << endl;
    }

    // End
    cplex.end();
    knapsackProblem.end();
    env.end();

}