#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
    
    IloEnv env;
    IloModel facilitiesProblem(env, "Facilities Problem");
    IloCplex cplex(facilitiesProblem);

    // Statement Data:
    // Number of deposits and clients
    int deposits, clients; 
    scanf("%d %d", &deposits, &clients);
    
    // Costs
    double costs_1[deposits];
    double costs_2[deposits][clients];

    for (int i = 0; i < deposits; i ++) {
        scanf("%lf", &costs_1[i]);
    }

    for (int i = 0; i < deposits; i++) {
        for (int j = 0; j < clients; j++) {
            scanf("%lf", &costs_2[i][j]);
        }
    }

    // Decision Variables:
    // xi: If a deposit i will be will be built
    // yi: If a deposit i answers to a client j
    IloBoolVarArray xi(env, deposits);
    IloArray<IloBoolVarArray> yi(env, deposits);
    
    for (int i = 0; i < deposits; i++) {
        yi[i] = IloBoolVarArray(env, clients);
    }
  
    // Constraints:
    // Answering to a client
    for (int j = 0; j < clients; j++) {
        IloExpr serve(env);

        for (int i = 0; i < deposits; i++) {
            serve += yi[i][j];
        }
        
        facilitiesProblem.add(serve == 1);
    }
    
    // Constraints:
    // Building a deposit
    for (int i = 0; i < deposits; i++) {
        IloExpr install(env);
        
        for (int j = 0; j < clients; j++) {
            install += yi[i][j];
        }
        
        facilitiesProblem.add(install <= xi[i] * clients);
    }

    // Objective Function:
    // Minimizing both the costs
    IloExpr cost(env);
    
    for (int i = 0; i < deposits; i++) {
        cost += costs_1[i] * xi[i];
        
        for (int j = 0; j < clients; j++) {
            cost += costs_2[i][j] * yi[i][j];
        }
    }
    
    facilitiesProblem.add(IloMinimize(env, cost));

    // Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;
    
    IloNumArray solutionInstall(env, deposits); 
    cplex.getValues(solutionInstall, xi);
    IloArray<IloNumArray> solutionServe(env, deposits);

    cout << "Deposits:" << endl;
    for (int i = 0; i < deposits; i++) {
        cout << "x" << i << " -> " << cplex.getValue(xi[i]) << endl;
    }

    cout << "Clients:" << endl;
    for (int i = 0; i < deposits; i++) {
        for (int j = 0; j < clients; j++) {
            cout << "x" << i << j << " -> " << cplex.getValue(yi[i][j]) << endl;
        }
    }

    // End
    cplex.end();
    facilitiesProblem.end();
    env.end();

}
