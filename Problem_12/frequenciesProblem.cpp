#include <stdio.h>
#include <ilcplex/ilocplex.h>
using namespace std;

// 2D Matrix of boolean variables
typedef IloArray<IloBoolVarArray> BoolVarMatrix;

int main() {
    
    IloEnv env;
    IloModel frequenciesProblem(env, "Frequencies Problem");
    IloCplex cplex(frequenciesProblem);

    // Statement Data:
    const int n_antenas = 10, m_freqs = 100;

    // Decision Variables:
    IloBoolVarArray frequencies(env, m_freqs);
    BoolVarMatrix freq_ij(env, n_antenas);
    
    for (int i = 0; i < n_antenas; i++) {
        freq_ij[i] = IloBoolVarArray(env, m_freqs);

        for (int j = 0; j < m_freqs; j++) {
            freq_ij[i][j] = IloBoolVar(env);
        }
    }

    // Restrictions:
    // Atribution:
    for (int i = 0; i < n_antenas; i++) {
        IloExpr sum(env);

        for (int j = 0; j < m_freqs; j++) {
            sum += freq_ij[i][j];
        }

        frequenciesProblem.add(sum == 1);
    }

    // Restrictions:
    // Interference:
    for (int i = 0; i < n_antenas; i++) {
        for (int j = 0; j < m_freqs; j++) {
            frequenciesProblem.add(freq_ij[i][j] <= frequencies[j]);
            frequenciesProblem.add(freq_ij[i][j] + freq_ij[j][i] <= 1);
        }
    }

    // Objective Function:
    IloExpr objective(env);
    for (int j = 0; j < m_freqs; j++) {
        objective += frequencies[j];
    }
    frequenciesProblem.add(IloMinimize(env, objective));

    // Get Solution:
    cplex.solve();
    cout << "Objective Function: " << cplex.getObjValue() << endl;

    // End
    cplex.end();
    frequenciesProblem.end();
    env.end();

}
