#include "solver.h"
#include <iostream>

using namespace std;

Solver::Solver(Instance* instance) : instance(instance) {
    C = instance-> C;
    V = instance-> V;
    speed_drone = instance->speed_drone; //done
    speed_truck = instance->speed_truck; //done

    nodes = instance->nodes;
    c_prime = instance->c_prime; //drone can go to them: C''
    distance_man = instance->distance_man;
    distance_eucl = instance->distance_eucl;
    time_truck = instance-> time_truck; //51x51 matrix
    time_drone = instance-> time_drone; //51x51 matrix
}

void Solver::run() {
    IloEnv env;
    try {
        int num_customer = C; //49 nodes, 48 customer
        int N_0 = C + 2; // 49 nodes 0-49
        int N_1 = C + 3; // 49 node 1-50 (50 = 0)

        IloModel model(env);

        IloNumVar X(env);

        // Objective Function:  minimize the latest return time to the depot for both drone and truck
        IloExpr expr0(env);
        expr0 += X;
        model.add(IloMinimize(env, expr0));

        // Constraint

        // 1st decision variable: x_ij: truck go from node i to node j
        IloArray<IloNumVarArray> x(env, N_0);

        for (int i = 0; i < N_0; ++i) {
            x[i] = IloNumVarArray(env, N_1+1);
            for (int j = 1; j < N_1; j++) {
                x[i][j] = IloNumVar(env, 0, 1, ILOBOOL);
            }
        }

        // (1) lower bounds on X, based on truck assignments.
        IloExpr expr1(env);
        for (int i = 0; i < N_0; ++i) {          
            for (int j = 1; j < N_1; j++) {
                if (i != j) {
                    expr1 += time_truck[i][j] * x[i][j];
                }
            }
        }
        model.add(X >= expr1);

        // 2nd decision variable: y_iv: customer i in C'' is served by drone v or not.
        IloArray<IloNumVarArray> y(env, c_prime.size()); //indexing from 1

        for (int i = 0; i < c_prime.size(); ++i) {
            y[i] = IloNumVarArray(env, V);
            for (int j = 0; j < V; j++) {
                y[i][j] = IloNumVar(env, 0, 1, ILOBOOL);
            }
        }

        // (2) lower bounds on X, based on drone assignments.
        for (int i = 0; i < V; i++) {
            IloExpr expr2(env);
            for (int j = 0; j < c_prime.size(); j++) {
                expr2 += (time_drone[0][i] + time_drone[i][N_0]) * y[j][i];
            }
            model.add(expr2 <= X);
        }

        //vector chứa những người được chở bởi drone
        vector<int> drone_eligible;
        for (auto x : c_prime) {
            drone_eligible.push_back(x);
        }

        // (3) Each customer is visited exactly once, either by the truck or a UAV
        for (int j = 1; j <= num_customer; j++) {
            IloExpr expr3(env);
            for (int i = 0; i < N_0; i++) { //sum_(i in N_0, i!=j) x_ij
                if (i !=j ) expr3 += x[i][j];
            }
            if (c_prime.count(j)) { //j in C''
                int index = 0;
                for (int tmp = 0; tmp < drone_eligible.size(); tmp++) {
                    if (drone_eligible[tmp] == j) index = tmp; //index này là số thứ tự của người j trong set C''
                }
                for (int k = 0; k < V; k++) {
                    expr3 += y[index][k]; //sum_(v in drone) y_jk 
                }
            }
            model.add(expr3 == 1);
        }

        // (4) the truck leaves the depot exactly once
        IloExpr expr4(env);
        for (int i = 1; i < N_1; i++) {
            expr4 += x[0][i];
        }
        model.add(expr4 == 1);


        // (5) the truck returns to the depot
        IloExpr expr5(env);
        for (int i = 0; i < N_0; i++) {
            expr5 += x[i][N_0];
        }
        model.add(expr5 == 1);

        // (6) A truck entering a customer node must also leave that node
        for (int j = 1; j <= num_customer; j++) {
            IloExpr expr6 (env), expr7(env);
            for (int i = 0; i < N_0; i++) {
                if (i != j) expr6 += x[i][j];
            }
            for (int k = 1; k < N_1; k++) {
                if (k != j) expr7 += x[j][k];
            }
            model.add(expr6 == expr7);
        }

        // (7) Subtour elimination constraint
        IloNumVarArray u(env, N_1, 1, N_0); //1 <= u_i <= c+2 (48 customer -> 50 - N_0)
        for (int i = 1; i <= num_customer; i++) {
            for (int j = 1; j < N_1; j++) {
                if (i != j) {
                    model.add(u[i] - u[j] + 1 <= N_1 * (1 - x[i][j]));
                }
            }
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());
        if (!cplex.solve()) {
            cerr << "Failed to solve the problem" << endl;
            throw(-1);
        }

        double objValue = cplex.getObjValue();
        env.end();
        cout << "Ket qua la: " << objValue;

    }
    catch (IloException& e) {
        cerr << "Concert exception caught: " << e << endl;
        throw(-1);
    }
}