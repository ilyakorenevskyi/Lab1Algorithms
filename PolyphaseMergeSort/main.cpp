#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#define A "FileA.txt"
#define B "FileB.txt"
#define C "FileC.txt"
#define D "FileD.txt"
#define Data "DataFile.txt"
using namespace std;
void gen(int n) {
	ofstream fout(Data);
	srand(time(0));
	int temp;
	for (int i = 0; i < n; i++) {
		temp = rand() % 1000;
		fout << temp;
		if (i != n - 1) fout << " ";
	}
	fout.close();
}
int partition(int beg, int end, vector<int>& myVector) {
	int pivot = myVector[end];
	int i = beg;
	for (int j = beg; j <= end; j++) {
		if (myVector[j] <= pivot) {
			swap(myVector[i], myVector[j]);
			i++;
		}
	}
	return i;
}
void quickSortRecur(int beg, int end, vector<int>& myVector) {
	if (beg >= end) {
		return;
	}
	int pivot = partition(beg, end, myVector);
	quickSortRecur(pivot, end, myVector);
	quickSortRecur(beg, pivot - 2, myVector);
}
void quickSort(vector<int> &myVector) {
	return quickSortRecur(0, myVector.size() - 1, myVector);
}
vector<int> read(string file, int s, int& pos){  //Used to divide First File to vectors
	vector<int> v;
	int buff;
	ifstream fin(file);
	fin.seekg(pos);
	for (int i = 0; i < s; i++){
		if (!fin.eof()){
			fin >> buff;
			v.push_back(buff);
		}
	}
	pos = fin.tellg();
	fin.close();
	return v;
}
int read(string file, int& pos){   //Read element from the file using current pos, then change current pos
	int buff;
	ifstream fin(file);
	fin.seekg(pos);
	if (!fin.eof())	fin >> buff;
	pos = fin.tellg();
	fin.close();
	return buff;
}
void write(string file, vector<int> v){             //Write sorted vector in the file
	ofstream fout(file, ios_base::app);
	for (int i = 0; i < v.size(); i++)
		fout << v[i] << " ";
	fout.close();
}
void write(string file, int i){			//Write element in the file
	ofstream fout(file, ios_base::app);
	fout << i << " ";
	fout.close();
}
void clear(string file){   //Clear file
	ofstream fout(file, ios_base::trunc);
	fout.close();
}
void clearAll(){
	clear(A);
	clear(B);
	clear(C);
	clear(D);
}
bool empty(string file){  //Check if file is empty
	ifstream fin(file);
	fin.seekg(0, ios::end);
	int p = fin.tellg();
	if (p == 0) return 1;
	else return 0;
}
void divide(string in, int size){  // Divide Data file into two files
	int count = 0;
	string out = A;
	int pos = 0;
	vector<int> v;
	while (pos >= 0){
		v = read(in, size, pos);    //Read vector with size S
		quickSort(v);
		write(out, v);
		out = (out == A) ? B : A;  // Change files
	}
}
void polyphaseMerge(int s){
	clearAll();
	string in1 = A, in2 = B, out = C;
	int pos1 = 0, pos2 = 0, ctr1, ctr2;
	int size = s;
	divide(Data, size);
	int cur_char1, cur_char2;
	while (!empty(in2)){
		if (out == C){
			clear(C);
			clear(D);
		}
		else{
			clear(A);
			clear(B);
		}
		while (pos1 >= 0 || pos2 >= 0){
			ctr1 = 0;
			ctr2 = 0;
			cur_char1 = read(in1, pos1);
			if (pos1 < 0) cur_char1= INT_MAX;
			cur_char2 = read(in2, pos2);
			if (pos2 < 0) cur_char2 = INT_MAX;
			if ((pos1 < 0) && (pos2 < 0)) break;
			for (int i = 0; i < size * 2; i++){
				if ((cur_char1 < cur_char2) && (pos1 >= 0)){
					ctr1++;
					write(out, cur_char1);
					if (ctr1 >= size) cur_char1 = INT_MAX;
					else cur_char1 = read(in1, pos1);
					if (pos1 < 0) cur_char1 = INT_MAX;
				}
				else if (pos2 >= 0){
					ctr2++;
					write(out, cur_char2);
					if (ctr2 >= size) cur_char2 = INT_MAX;
					else cur_char2 = read(in2, pos2);
					if (pos2 < 0) cur_char2 = INT_MAX;
				}
			}
			if (out == A) out = B;
			else if (out == B) out = A;
			else if (out == C) out = D;
			else if (out == D) out = C;
		}
		size *= 2;
		if (in1 == A){
			pos1 = 0;
			pos2 = 0;
			in1 = C;
			in2 = D;
			out = A;
		}
		else{
			pos1 = 0;
			pos2 = 0;
			in1 = A;
			in2 = B;
			out = C;
		}
	}
	clear(Data);
	ifstream fin((out == A) ? C : A);
	for (fin >> s; !fin.eof(); fin >> s)
		write(Data, s);
	clearAll();
}
int main(){
	int size;
	cout << "Enter available memory size: ";
	cin >> size;
	cout << "Enter 0 if you want to enter data, or 1 if you want to generate data\n";
	bool c;
	cin >> c;
	cout << "Enter amount of data:\n";
	int n;
	cin >> n;
	if (c) {
		gen(n);
	}
	else {
		ofstream fout(Data);
		int temp;
		for (int i = 0; i < n; i++) {
			cin >> temp;
			fout << temp;
			if (i != n - 1) fout << " ";
		}
		fout.close();
	}
	polyphaseMerge(size);
	cout << "Sort is done" << endl;
	return 0;
}
