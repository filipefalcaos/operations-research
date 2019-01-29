#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
	
	IloEnv env;
	IloModel facilitiesProblem(env, "Facilities Problem");
	IloCplex cplex(facilitiesProblem);

  	// Statement Data:
    int deposits, clients; 
    scanf("%d %d", &deposits, &clients);
    
    double installationCosts[deposits];
    double serviceCosts[deposits][clients];

    for (int i = 0; i < deposits; i ++) {
    	scanf("%lf", &installationCosts[i]);
    }

    for (int i = 0; i < deposits; i++) {
    	for (int j = 0; j < clients; j++) {
    		scanf("%lf", &serviceCosts[i][j]);
    	}
    }

  	// Decision Variables:
    IloBoolVarArray installDeposits(env, deposits);
    IloArray<IloBoolVarArray> serveClient(env, deposits);
    for (int i = 0; i < deposits; i++) {
    	serveClient[i] = IloBoolVarArray(env, clients);
    }
  
  	// Restrictions:
    // Service:
    for (int j = 0; j < clients; j++) {
        IloExpr serve(env);

        for (int i = 0; i < deposits; i++) {
        	serve += serveClient[i][j];
        }
        
        facilitiesProblem.add(serve == 1);
    }
    
    // Restrictions:
    // Installation:
    for (int i = 0; i < deposits; i++) {
        IloExpr install(env);
        
        for (int j = 0; j < clients; j++) {
        	install += serveClient[i][j];
        }
        
        facilitiesProblem.add(install <= installDeposits[i] * clients);
    }

  	// Objective Function:
    IloExpr cost(env);
    for (int i = 0; i < deposits; i++) {
    	cost += installationCosts[i] * installDeposits[i];
      	for (int j = 0; j < clients; j++) {
        	cost += serviceCosts[i][j] * serveClient[i][j];
      	}
    }
    facilitiesProblem.add(IloMinimize(env, cost));

  	// Get Solution:
    cplex.solve();
    printf("minCost: %.3lf\n", cplex.getObjValue());
    IloNumArray solutionInstall(env, deposits); cplex.getValues(solutionInstall, installDeposits);
    IloArray<IloNumArray> solutionServe(env, deposits);
    
    for (int i = 0; i < deposits; i++) {
      	solutionServe[i] = IloNumArray(env, clients);
      	cplex.getValues(solutionServe[i], serveClient[i]);
      
      	if (solutionInstall[i]) {
        	printf("Install deposit %d:\n", i + 1);
        	for (int j = 0; j < clients; j ++) {
        		if (solutionServe[i][j]) {
        			printf("\tServe client %d\n", j + 1);
        		}
        	}
      	}
    }

  	env.end();
  	return(0);
}
