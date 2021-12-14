#include "FileSystem.h"


bool FileSystem::ReadFileCSV(string filename, vector<vector<int>>_map) {
	fstream file;
	file.open(filename);
	if (!file.is_open()) {
		cout << "can't open file" << endl;
		return false;
	}
	regex ValidRow("\\b\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\b");
	string line, block;
	int i = 0;
	while (!file.eof()) {
		getline(file, line);
		if (!regex_match(line, ValidRow)) {	//checking valid line in .csv file, but work only for 10 numbers in a line	
			cout << "the map file is invalid format (error at line " << i + 1 << ")" << endl;
			file.close();
			return false;
		}
		if (line == "")break; // reach end of file

		// Send each line to sever here 
		
		stringstream s(line);
		int j = 0;
		while (getline(s, block, ',')) {
			int temp = stoi(block);
			_map[i][j] = temp;
			j++;
		}
		i++;
	}
	file.close();
	return true;
}