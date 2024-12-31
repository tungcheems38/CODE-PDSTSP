#pragma once

#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include "instance.h"
#include <ilcplex/ilocplex.h>

ILOSTLBEGIN
typedef IloArray<IloNumVarArray> NumVar2D;

using namespace std;

class Solver {
public:
    Solver(Instance*);

    Instance* instance;

	int C; //done
	int V; //done
	double speed_drone; //done
	int speed_truck; //done

	vector<vector<int>> nodes; //done
	set<int> c_prime; //done
	vector<vector<double>> distance_man; // round to nearest integer - done
	vector<vector<double>> distance_eucl; // round to nearest integer - done
	vector<vector<double>> time_truck; // round to nearest integer - done
	vector<vector<double>> time_drone; // round to nearest integer - done

    void run();
};

#endif
