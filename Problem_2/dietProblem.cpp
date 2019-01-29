#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
	
	IloEnv env;
	IloModel dietProblem(env, "Diet Problem");
	IloCplex cplex(dietProblem);

	// Statement Data:
	const int n = 6, m = 2;
	double data[m][n] = {{1, 0, 2, 2, 1, 2}, {0, 1, 3, 1, 3, 2}};
	double price[n] = {35, 30, 60, 50, 27, 22};
	double qtd_min[2] = {9, 19};

	// Decision Variables:
	IloNumVarArray vars(env, n);
	for (int i = 0; i < n; i++) {
		vars[i] = IloNumVar(env);
	}

	// Restrictions:
	for (int i = 0; i < m; i++) {
		IloExpr constraint(env);

		for (int j = 0; j < n; j++) {
			constraint += data[i][j] * vars[i];
			dietProblem.add(vars[i] >= 0);
		}

		dietProblem.add(constraint >= qtd_min[i]);
	}

    // Objective Function:
    IloExpr objective(env);
    for (int i = 0; i < n; i++) {
		objective += price[i] * vars[i];
	}
	dietProblem.add(IloMinimize(env, objective));

	// Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;

    for (int i = 0; i < n; i++) {
		cout << "x" << i << ": " << cplex.getValue(vars[i]) << endl;
	}

    // End
    cplex.end();
    dietProblem.end();
    env.end();
}
