#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
	
	IloEnv env;
	IloModel rationProblem(env, "Ration Problem");
	IloCplex cplex(rationProblem);

	// Statement Data:
	double qtd_amgs[2] = {5, 1};
	double qtd_re[2] = {2, 4};
	double qtd[2] = {30000, 10000};
	double amgs_price = 20, re_price = 30;
	double meat_price = 4, cereal_price = 1;

	// Decision Variables:
	IloNumVar AMGS(env), RE(env);

	// Restrictions:
	rationProblem.add(AMGS >= 0 && RE >= 0);
	for (int i = 0; i < 2; i++) {
		rationProblem.add(qtd_amgs[i] * AMGS + qtd_re[i] * RE <= qtd[i]);
	}

    // Objective Function:
	rationProblem.add(IloMaximize(env, (amgs_price - qtd_amgs[0] * cereal_price - 
		qtd_amgs[1] * meat_price) * AMGS + (re_price - qtd_re[0] * cereal_price - 
		qtd_re[1] * meat_price) * RE));

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
