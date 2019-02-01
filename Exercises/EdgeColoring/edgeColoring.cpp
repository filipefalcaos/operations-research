#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

// 3D Matrix of boolean variables
typedef IloArray<IloArray<IloBoolVarArray> > Bool3DMatrix;

// 2D Matrix of boolean variables
typedef IloArray<IloBoolVarArray> BoolVarMatrix;

int main() {
    
    IloEnv env;
    IloModel edgeColoringProblem(env, "Edge Coloring Problem");
    IloCplex cplex(edgeColoringProblem);

    // Statement Data:
    // Number of vertices in the graph
    int n;
    scanf("%d", &n);
    
    int graph[n][n];
    int vertex_1, vertex_2;
    int costs[4] = {1, 2, 3, 4};

    // Initialize the adjacency matrix with 0s
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph[i][j] = 0;
        }
    }
    
    // Read edges (pair of vertices) until two -1 are given
    // The first vertex should be 1, the second should be 2, ...
    // Then, assign 1 to the given edge in the ajacency matrix
    scanf("%d %d", &vertex_1, &vertex_2);
    while (vertex_1 != -1 && vertex_2 != -1) {
        graph[vertex_1 - 1][vertex_2 - 1] = 1;
        graph[vertex_2 - 1][vertex_1 - 1] = 1;
        scanf("%d %d", &vertex_1, &vertex_2);
    }

    // Decision Variables:
    // Create a 3D boolean matrix with coefficients i, j, k
    // Coefficients i and j are for the edge, while k is for the color
    Bool3DMatrix x(env, n);
    for (int i = 0; i < n; i++) {
        x[i] = BoolVarMatrix(env, n);

        for (int j = 0; j < n; j++) {
            x[i][j] = IloBoolVarArray(env, 4);

            for (int k = 0; k < 4; k++) {
                if (graph[i][j] == 1) {
                    x[i][j][k] = IloBoolVar(env);
                }
            }
        }
    }

    // Decision Variables:
    // Create an array that store the number of times a color is used
    IloNumVarArray c(env, 4);
    for (int k = 0; k < 4; k++) {
        c[k] = IloNumVar(env);
    }

    // Constraints:
    // Make sure that one edge has only one color
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            IloExpr constraint(env);

            for (int k = 0; k < 4; k++) {
                if (graph[i][j] == 1) {
                    constraint += x[i][j][k];
                }
            }

            edgeColoringProblem.add(constraint == 0);
        }
    }

    // Constraints:
    // Make sure that neighbors dont have the same color
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < 4; k++) {
                if (graph[i][j] == 1) {
                    edgeColoringProblem.add(x[i][j][k] + x[i][j][k] <= 1);
                }
            }
        }
    }

    // Constraints:
    // Set c as an upper bound
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < 4; k++) {
                if (graph[i][j] == 1) {
                    edgeColoringProblem.add(x[i][j][k] <= c[k]);
                }
            }
        }
    }

    // Objective Function:
    IloExpr objective(env);
    for (int k = 0; k < 4; k++) {
        objective += c[k] * costs[k];
    }
    edgeColoringProblem.add(IloMinimize(env, objective));

    // Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < 4; k++) {
                if (graph[i][j] == 1) {
                    cout << "x" << i << j << k << " -> " << cplex.getValue(x[i][j][k]) << endl;
                }
            }
        }
    }

    // End
    cplex.end();
    edgeColoringProblem.end();
    env.end();

}
