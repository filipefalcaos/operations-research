#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

// 2D Matrix of numerical variables
typedef IloArray<IloNumVarArray> NumVarMatrix;

int main() {
    
    IloEnv env;
    IloModel farmProblem(env, "Farm Problem");
    IloCplex cplex(farmProblem);

    // Statement Data:
    // Data provided by the problem
    const int n_farms = 3, n_plantations = 3;
    double area[n_farms] = {400, 650, 350};
    double max_area[n_farms] = {660, 880, 400};
    double water[n_farms] = {1800, 2200, 950};
    double water_prop[n_farms] = {5.5, 4, 3.5};
    double profit[n_farms] = {5000, 4000, 1800};

    // Decision Variables:
    NumVarMatrix farms(env, n_farms);
    for (int i = 0; i < n_farms; i++) {
        farms[i] = IloNumVarArray(env, n_farms);

        for (int j = 0; j < n_farms; j++) {
            farms[i][j] = IloNumVar(env);
        }
    }

    // Constraints:
    // Area/Plantation
    for (int i = 0; i < n_plantations; i++) {
        IloExpr constraint_area(env);

        for (int j = 0; j < n_farms; j++) {
            constraint_area += farms[i][j];
        }

        farmProblem.add(constraint_area <= max_area[i]);
    }

    // Constraints:
    // Water/Farm
    for (int i = 0; i < n_farms; i++) {
        IloExpr constraint_water(env);

        for (int j = 0; j < n_plantations; j++) {
            constraint_water += farms[i][j] * water_prop[i];
        }

        farmProblem.add(constraint_water <= water[i]);
    }

    // Constraints:
    // Area/Farm
    for (int i = 0; i < n_farms; i++) {
        IloExpr constraint_area2(env);

        for (int j = 0; j < n_plantations; j++) {
            constraint_area2 += farms[i][j];
        }

        farmProblem.add(constraint_area2 <= area[i]);
    }

    // Constraints:
    // Proportion:
    IloExprArray prop_constraints(env, n_farms);
    
    for (int i = 0; i < n_farms; i++) {
        IloExpr constraint_prop(env);

        for (int j = 0; j < n_plantations; j++) {
            constraint_prop += farms[i][j];
        }

        prop_constraints[i] = constraint_prop / area[i];
    }
    
    // Constraints:
    // Proportion equality
    farmProblem.add(prop_constraints[0] == prop_constraints[1] == prop_constraints[2]);

    // Objective Function:
    // Maximize the profit
    IloExpr objective(env);
        
    for (int i = 0; i < n_plantations; i++) {
        IloExpr objective_part(env);

        for (int j = 0; j < n_farms; j++) {
            objective_part += farms[i][j];
        }

        objective += (objective_part * profit[i]);
    }

    farmProblem.add(IloMaximize(env, objective));

    // Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;

    for (int i = 0; i < n_farms; i++) {
        for (int j = 0; j < n_farms; j++) {
            cout << "x" << i << j << ": " << cplex.getValue(farms[i][j]) << endl;
        }
    }

    // End
    cplex.end();
    farmProblem.end();
    env.end();

}
