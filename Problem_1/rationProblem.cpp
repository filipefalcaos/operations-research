#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
	
	IloEnv env;
	IloModel rationProblem(env, "Ration Problem");
	IloCplex cplex(rationProblem);

	// Decision Variables:
	IloNumVar AMGS(env), RE(env);

	// Restrictions:
    rationProblem.add(1 * AMGS + 4 * RE <= 10000);
    rationProblem.add(5 * AMGS + 2 * RE <= 30000);
    rationProblem.add(AMGS >= 0 && RE >= 0);

    // Objective Function:
	rationProblem.add(IloMaximize(env, 11 * AMGS + 12 * RE));

	// Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;
    cout << "AMGS: " << cplex.getValue(AMGS) << endl;
    cout << "RE: " << cplex.getValue(RE) << endl;
   	
    // End
    cplex.end();
    rationProblem.end();
    env.end();

}
