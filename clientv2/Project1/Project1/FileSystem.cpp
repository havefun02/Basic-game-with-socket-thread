#include "FileSystem.h"


string FileSystem::ReadFileCSV(string filename) {
	fstream file;
	stringstream builder;
	file.open(filename);
	if (!file.is_open()) {
		cout << "can't open file" << endl;
		return "can't open file";
	}
	regex ValidRow("\\b\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\b");
	string line, block;
	int i = 0;
	while (!file.eof()) {
		getline(file, line);
		if (line == "")break;
		if (!regex_match(line, ValidRow)) {	//checking valid line in .csv file, but work only for 10 numbers in a line	
			cout << "the map file is invalid format (error at line " << i + 1 << ")" << endl;
			file.close();
			return "fail";
		}

	 // reach end of file
		
		stringstream s(line);
		while (getline(s, block, ',')) {
			builder << block;
		}
	}
	file.close();
	return builder.str();
}

vector<vector<int>> FileSystem::LineToMatrix(string matrix) {
	vector<vector<int>> _map;
	const int n = 10;			// matrix 10x10
	int pos = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			string temp = matrix.substr(pos, 1);
			int num = -1;
			try
			{
				num = stoi(temp);
			}

			catch (const std::exception&)
			{
				cout << "invalid number" << endl;
				return _map;
			}

			_map[i][j] = num;
			pos++;
		}
	}

	return _map;
}