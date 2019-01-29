#include <stdio.h>
#include <float.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
	
	IloEnv env;
	IloModel dietProblem(env, "Diet Problem");
	IloCplex cplex(dietProblem);

	// Decision Variables:
	IloNumVar x1(env), x2(env), x3(env), x4(env), x5(env), x6(env);

	// Restrictions:
    dietProblem.add(1 * x1 + 0 * x2 + 2 * x3 + 2 * x4 + 1 * x5 + 2 * x6 >= 9);
    dietProblem.add(0 * x1 + 1 * x2 + 3 * x3 + 1 * x4 + 3 * x5 + 2 * x6 >= 19);

    // Objective Function:
	dietProblem.add(IloMinimize(env, 35 * x1 + 30 * x2 + 60 * x3 + 50 * x4 + 
		27 * x5 + 22 * x6));

	// Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;
    cout << "x1: " << cplex.getValue(x1) << endl;
    cout << "x2: " << cplex.getValue(x2) << endl;
    cout << "x3: " << cplex.getValue(x3) << endl;
    cout << "x4: " << cplex.getValue(x4) << endl;
    cout << "x5: " << cplex.getValue(x5) << endl;
    cout << "x6: " << cplex.getValue(x6) << endl;

    // End
    cplex.end();
    dietProblem.end();
    env.end();
}
