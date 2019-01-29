#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
    
    IloEnv env;
    IloModel paintProblem(env, "Paint Problem");
    IloCplex cplex(paintProblem);

    // Statement Data:
    const int n = 2, m = 4;
    double proportions[m] = {0.25, 0.5, 0.2, 0.5};
    double solutions[m] = {0.3, 0.6, 0.7, 0.4};
    double liters_sr = 1000, liters_sn = 250;
    double cost_sec = 4.0, cost_cor = 6.0;
    double cost_a = 1.5, cost_b = 1.0;

    // Decision Variables:
    IloNumVarArray sr_paint(env, m);
    IloNumVarArray sn_paint(env, m);
    
    for (int i = 0; i < m; i++) {
        sr_paint[i] = IloNumVar(env);
        sn_paint[i] = IloNumVar(env);
    }

    // Restrictions:
    // Non-negativity:
    paintProblem.add(sr_paint[0] >= 0 && sr_paint[1] >= 0 && sr_paint[2] >= 0 
        && sr_paint[3] >= 0);
    paintProblem.add(sn_paint[0] >= 0 && sn_paint[1] >= 0 && sn_paint[2] >= 0 
        && sn_paint[3] >= 0);
    
    // Restrictions:
    // Production:
    paintProblem.add(sr_paint[0] + sr_paint[1] + sr_paint[2] + sr_paint[3] 
        == liters_sr);
    paintProblem.add(sn_paint[0] + sn_paint[1] + sn_paint[2] + sn_paint[3] 
        == liters_sn);

    // Restrictions:
    // Proportion:
    paintProblem.add(((sr_paint[2] + sr_paint[0] * solutions[0] + sr_paint[1] 
        * solutions[2]) / 1000) >= proportions[0]);
    paintProblem.add(((sr_paint[3] + sr_paint[0] * solutions[2] + sr_paint[1] 
        * solutions[3]) / 1000) >= proportions[1]);
    paintProblem.add(((sn_paint[2] + sn_paint[0] * solutions[0] + sn_paint[1] 
        * solutions[2]) / 1000) >= proportions[0]);
    paintProblem.add(((sn_paint[3] + sn_paint[0] * solutions[2] + sn_paint[1] 
        * solutions[3]) / 1000) >= proportions[1]);

    // Objective Function:
    paintProblem.add(IloMinimize(env, (sr_paint[0] * cost_a + sr_paint[1] * cost_b + 
        sr_paint[2] * cost_sec + sr_paint[3] * cost_cor) + (sn_paint[0] * cost_a + 
        sn_paint[1] * cost_b + sn_paint[2] * cost_sec + sn_paint[3] * cost_cor)));

    // Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;

    cout << "SR paint: " << endl;
    for (int i = 0; i < m; i++) {
        cout << "x" << i << ": " << cplex.getValue(sr_paint[i]) << endl;
    }

    cout << "SN paint: " << endl;
    for (int i = 0; i < m; i++) {
        cout << "x" << i << ": " << cplex.getValue(sn_paint[i]) << endl;
    }

    // End
    cplex.end();
    paintProblem.end();
    env.end();

}
