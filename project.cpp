#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <tuple>
#include <map>
#include <unordered_map>
#include <chrono>

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


const string LINEITEM[] = {"ORDERKEY", "PARTKEY", "SUPPKEY", "LINENUMBER", "QUANTITY", "EXTENDEDPRICE", "DISCOUNT", "TAX", "RETURNFLAG", "LINESTATUS", "SHIPDATE", "COMMITDATE", "RECEIPTDATE", "SHIPINSTRUCT", "SHIPMODE", "COMMENT"};
const string ORDERS[] = {"ORDERKEY", "CUSTKEY", "ORDERSTATUS", "TOTALPRICE", "ORDERDATE", "ORDERPRIORITY", "CLERK", "SHIPPRIORITY", "COMMENT"};
const string NATION[] = {"NATIONKEY", "NAME", "REGIONKEY", "COMMENT"};
const string REGION[] = {"REGIONKEY", "NAME", "COMMENT"};

vector<vector<string>> parseFile(string filename, char delimiter) {
	stringstream ss;
	ifstream myFile;
	vector<string> row;
	vector<vector<string>> allRows;
	myFile.open(filename);

	if (!myFile.is_open()) {
		cerr << "Invalid input, unable to open file" << endl;
		exit(EXIT_FAILURE);
	}
	ss << myFile.rdbuf();
	string temp;
	while (getline(ss, temp)) {

		istringstream line(temp);
        while (getline(line, temp, delimiter)) {
            row.push_back(temp);
        }
        allRows.push_back(row);
        row.clear();
	}
	return allRows;
}

double join(vector<vector<string>> firstFile, vector<vector<string>> secondFile, int col1, int col2, string mode, string output) {
	auto start = chrono::steady_clock::now();
	vector<vector<string>> result;
	int i = 0;
	int j = 0;
	int len1 = firstFile.size();
	int len2 = secondFile.size();
	if (mode == "NESTED_LOOP") {
		vector<string> temp;
		for (i=0; i < len1; i++) {
			for (j=0; j < len2; j++) {
				if (firstFile[i][col1] == secondFile[j][col2]) {
					temp.insert(temp.begin(), firstFile[i].begin(), firstFile[i].end());
					temp.insert(temp.end(), secondFile[j].begin(), secondFile[j].end());
					result.push_back(temp);
					temp.clear();
				}
			}
		}

	}
	else {
		vector<string> temp;
		unordered_map<string, vector<string>> hashtable;
		for (i = 0; i < len1; i++) {
			hashtable.insert(pair<string, vector<string>> (firstFile[i][col1], firstFile[i]));
		}
		for (j = 0; j < len2; j++) {
			if (hashtable.find(secondFile[j][col2]) != hashtable.end()) {
				vector<string> first = hashtable.at(secondFile[j][col2]);
				temp.insert(temp.begin(), first.begin(), first.end());
				temp.insert(temp.end(), secondFile[j].begin(), secondFile[j].end());
				result.push_back(temp);
				temp.clear();
			}
		}
	}
	auto end = chrono::steady_clock::now();
	duration<double, milli> ms_double = end - start;
	if (output != "") {
		ofstream myfile;
		myfile.open(output);
		for (auto row : result) {
			for (auto pro: row) {
				myfile << pro << "|";
			}
			myfile << "\n";
		}
		myfile.close();
	}

	return ms_double.count();
}


int findColIndex(string filename, string colname) {
	int index = -1;
	if (filename == "lineitem.csv") {
		for (int i = 0; i < 16; i++) {
			if (colname == LINEITEM[i]) {
				index = i;
				break;
			}
		}
	}

	else if (filename == "orders.csv") {
		for (int i = 0; i < 9; i++) {
			if (colname == ORDERS[i]) {
				index = i;
				break;
			}
		}
	}

	else if (filename == "nation.csv") {
		for (int i = 0; i < 4; i++) {
			if (colname == NATION[i]) {
				index = i;
				break;
			}
		}
	}

	else if (filename == "region.csv") {
		for (int i = 0; i < 3; i++) {
			if (colname == REGION[i]) {
				index = i;
				break;
			}
		}
	}
	if (index == -1 ) {
		cerr << colname << "doesn't exist in " << filename << endl;
		exit(EXIT_FAILURE);
	}
	else {
		return index;
	}
} 


int main(int argc, char *argv[]) {
	if (argc != 6 && argc != 7) {
		cerr << "Incorrect number of argument" << endl;
		exit(EXIT_FAILURE);
	}
	string firstPath = argv[1];
	string colName1 = argv[2];
	string secondPath = argv[3];
	string colName2 = argv[4];
	string mode = argv[5];
	string output = "";
	if (argc == 7) {
		output = argv[6];
	}

	vector<vector<string>> firstFile = parseFile(firstPath, '|');
	vector<vector<string>> secondFile = parseFile(secondPath, '|');

	int colIndex1 = findColIndex(firstPath, colName1);
	int colIndex2 = findColIndex(secondPath, colName2);

	// double hashTime = 0;
	// double loopTime = 0;
	
	// for (int i = 0; i < 10; i++) {
	// 	double temptime = join(firstFile, secondFile, colIndex1, colIndex2, "NESTED_LOOP", output);
	// 	loopTime += temptime;
	// 	cout << temptime << endl;
	// }
	// for (int i = 0; i < 10; i++) {
	// 	double temptime = join(firstFile, secondFile, colIndex1, colIndex2, "HASH", output);
	// 	hashTime += temptime;
	// 	cout << temptime << endl;
	// }
	// double hashavgtime = hashTime / 10.0;
	// double loopavgtime = loopTime / 10.0;


	// cout << hashavgtime << endl;
	// cout << loopavgtime << endl;
	
	double time = join(firstFile, secondFile, colIndex1, colIndex2, mode, output);
}