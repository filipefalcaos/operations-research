#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
    
    IloEnv env;
    IloModel maxFlowProblem(env, "Max Flow Problem");
    IloCplex cplex(maxFlowProblem);

    // Decision Variables:
    // Sample data (nodes)
    IloNumVar x01(env), x02(env), x03(env);
    IloNumVar x14(env), x15(env);
    IloNumVar x24(env), x25(env), x26(env);
    IloNumVar x35(env), x47(env), x57(env), x67(env);

    // Restrictions:
    // Capacity
    maxFlowProblem.add(x01 <= 3);    maxFlowProblem.add(x02 <= 2);
    maxFlowProblem.add(x03 <= 2);    maxFlowProblem.add(x14 <= 5);
    maxFlowProblem.add(x15 <= 1);    maxFlowProblem.add(x24 <= 1);
    maxFlowProblem.add(x25 <= 3);    maxFlowProblem.add(x26 <= 1);
    maxFlowProblem.add(x35 <= 1);    maxFlowProblem.add(x47 <= 4);
    maxFlowProblem.add(x57 <= 2);    maxFlowProblem.add(x67 <= 4);

    // Constraints:
    // Flow Conservation
    maxFlowProblem.add(x01 == (x14 + x15));          maxFlowProblem.add(x02 == (x24 + x25 + x26));
    maxFlowProblem.add(x03 == x35);                  maxFlowProblem.add((x14 + x24) == x47);
    maxFlowProblem.add((x15 + x25 + x35) == x57);    maxFlowProblem.add(x26 == x67);

    // Objective Function:
    // Maximize the flow starting from node 0
    maxFlowProblem.add(IloMaximize(env, x01 + x02 + x03));

    // Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;
    cout << "x01: " << cplex.getValue(x01) << endl;
    cout << "x02: " << cplex.getValue(x02) << endl;
    cout << "x03: " << cplex.getValue(x03) << endl;
    cout << "x14: " << cplex.getValue(x14) << endl;
    cout << "x15: " << cplex.getValue(x15) << endl;
    cout << "x24: " << cplex.getValue(x24) << endl;
    cout << "x25: " << cplex.getValue(x25) << endl;
    cout << "x26: " << cplex.getValue(x26) << endl;
    cout << "x35: " << cplex.getValue(x35) << endl;
    cout << "x47: " << cplex.getValue(x47) << endl;
    cout << "x57: " << cplex.getValue(x57) << endl;
    cout << "x67: " << cplex.getValue(x67) << endl;

    // End
    cplex.end();
    maxFlowProblem.end();
    env.end();

}
