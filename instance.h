#pragma once
#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
/*
Các bước để viết từ mô hình vào file Instance:
Bước 1: Xác định các yếu tố thuộc lớp Public và lớp Private
Lớp Public chứa định nghĩa các tham số và các tập - các mảng lưu trữ khoảng cách và thời gian (không có decision var)
Lớp Public chứa các hàm để tính các yếu tố cần thiết, để đọc file
Bước 2: Xác định kiểu dữ liệu cho từng biến và từng hàm => set, vector, int, ...
Bước 3: Viết vào mô hình

*/
using namespace std;

class Instance {
public:
	//string instanceName;

	int C; // số lượng nút
	int V = 1; // số lượng drone
	double speed_drone; // tốc độ của drone
	int speed_truck = 2; // tốc độ của xe tải

	vector<vector<int>> nodes; // vector lưu trữ các node
	set<int> c_prime; // tập hợp lưu trữ các đỉnh có thể giao được bằng drone (C') - chưa tính khoảng cách
	vector<vector<double>> distance_man; // khoảng cách Mahattan (tổng giá trị tuyệt đối x và y)
	vector<vector<double>> distance_eucl; // khoảng cách Euclidean (căn tổng bình phương)
	vector<vector<double>> time_truck; // tau
	vector<vector<double>> time_drone; // tau_prime

	Instance(vector <string> instanceFile);
	// int get_time_drone_flying(int a);
	// int get_distance_drone_flying(int a);
	// int get_time_truck(int a, int b);
	// int get_distance_truck(int a, int b);
	bool is_drone_eligible(double a); // là node mà drone chở được

private:
	void read_input_node(string inputFile); // Hàm đọc input
	void read_input_tau(string inputFile); // Hàm đọc input
	void read_input_tau_prime(string inputFile); // Hàm đọc input
	int get_euclid_distance(int x1, int y1, int x2, int y2); // Hàm tính
	int get_man_distance(int x1, int y1, int x2, int y2);
};


#endif