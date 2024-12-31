#include "instance.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <cmath>

using namespace std;
/*
Viết file instance.cpp gồm: viết các hàm đã định nghĩa ở trong instance.h
*/
Instance::Instance(vector <string> instanceFile) {
    read_input_tau(instanceFile[1]);
    read_input_tau_prime(instanceFile[2]);
    read_input_node(instanceFile[0]);
    
}

void Instance::read_input_node(string inputFile)
{
    ifstream instream(inputFile);
    if (!instream.is_open()) {
        cout << "Unable to open file \n";
        exit(0);
    }
    string line;


    //read speed_drone
    if (getline(instream, line)) {
        stringstream ss(line);
        vector<string> all;
        string token;
        char delimiter = ',';
        while (getline(ss, token, delimiter)) {
            all.push_back(token);
        }
        speed_drone = stod(all[3]);
        cout << "Speed là: " << speed_drone;
        cout << endl;
        cout << endl;
    }

    //read nodes

    while (getline(instream, line)) {
        stringstream ss(line);
        vector<int> all;
        string token;
        char delimiter = ',';
        while (getline(ss, token, delimiter)) {
            int temp = stoi(token);
            all.push_back(temp);
        }
        nodes.push_back(all);
    }

    C = nodes.size() - 1;

    for (int i = 0; i < nodes.size(); i++) {
        for (int j = 0; j < nodes[i].size(); j++) {
            cout << nodes[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    // calculate manhattan distance
    for (int i = 0; i < nodes.size(); i++) {
        vector<double> man_distance;
        for (int j = 0; j < nodes.size(); j++) {
            double x1 = nodes[i][1];
            double y1 = nodes[i][2];
            double x2 = nodes[j][1];
            double y2 = nodes[j][2];
            double man_dist = get_man_distance(x1, y1, x2, y2);
            man_distance.push_back(man_dist);
        }
        double x1 = nodes[0][1];
        double y1 = nodes[0][2];
        double x2 = nodes[i][1];
        double y2 = nodes[i][2];
        double man_dist = get_man_distance(x1, y1, x2, y2);
        man_distance.push_back(man_dist);

        distance_man.push_back(man_distance);
    }
    vector<double> vector_zero(C + 2, 0);
    distance_man.push_back(vector_zero);

    // calculate euclid distance
    for (int i = 0; i < nodes.size(); i++) {
        vector<double> euclid_distance;
        for (int j = 0; j < nodes.size(); j++) {
            double x1 = nodes[i][1];
            double y1 = nodes[i][2];
            double x2 = nodes[j][1];
            double y2 = nodes[j][2];
            double euclid_dist = get_euclid_distance(x1, y1, x2, y2);
            euclid_distance.push_back(euclid_dist);
        }
        double x1 = nodes[0][1];
        double y1 = nodes[0][2];
        double x2 = nodes[i][1];
        double y2 = nodes[i][2];
        double euclid_dist = get_euclid_distance(x1, y1, x2, y2);
        euclid_distance.push_back(euclid_dist);

        distance_eucl.push_back(euclid_distance);
    }
    distance_eucl.push_back(vector_zero);
    // confirm a customer is drone-eligible
    vector<int> can_use_drone;
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i][3] == 0) {
            can_use_drone.push_back(nodes[i][0]);
        }
    }
	cout << can_use_drone.size();
    cout << endl;
	for (int i = 0; i < can_use_drone.size(); i++) {
		cout << can_use_drone[i] << " ";
	}
    for (int x : can_use_drone) {
        if ((is_drone_eligible((time_drone[0][x]))) && (x != C + 1)) {
            c_prime.insert(nodes[x-1][0]);
            cout << "chay" << endl;
        }
    }
    cout << endl;
    cout << c_prime.size();
    cout << endl;
    vector<int> myVector(c_prime.begin(), c_prime.end());
    for (auto x : myVector) {
        cout << x << " ";
    }

}

void Instance::read_input_tau(string inputFile)
{
    ifstream instream(inputFile);
    if (!instream.is_open()) {
        cout << "Unable to open file \n";
        exit(0);
    }
    string line;
    // read time truck
    while (getline(instream, line)) {
        stringstream ss(line);
        vector<double> all;
        string token;
        char delimiter = ',';
        while (getline(ss, token, delimiter)) {
            double temp = stod(token);
            all.push_back(temp);
        }
        time_truck.push_back(all);
    }
}

void Instance::read_input_tau_prime(string inputFile)
{
    ifstream instream(inputFile);
    if (!instream.is_open()) {
        cout << "Unable to open file \n";
        exit(0);
    }
    string line;
    // read time truck
    while (getline(instream, line)) {
        stringstream ss(line);
        vector<double> row;
        string token;
        while (getline(ss, token, ',')) {
            row.push_back(stod(token));
        }
        time_drone.push_back(row);
    }
    for (int i = 0; i < time_drone.size(); i++) {
        for (int j = 0; j < time_drone[i].size(); j++) {
            cout << time_drone[i][j] << " ";
            continue;
        }
        cout << endl;
    }
    cout << endl;
}
int Instance::get_euclid_distance(int x1, int y1, int x2, int y2) {
    cout << "CHAY 1";
    return int(sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}
int Instance::get_man_distance(int x1, int y1, int x2, int y2) {
    cout << "CHAY 2";
    return int((abs(x2 - x1) + abs(y2 - y1)));
}

bool Instance::is_drone_eligible(double a) {
    cout << "CHAY 3";
    return a*2 <= 35; //total time do not exceed 40 mins
} 