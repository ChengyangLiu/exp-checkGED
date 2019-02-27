#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "../include/checkGED.h"

using namespace std;

void CheckGED::loadGEDs(const string& gedpath) {
	string line;
	try{
		ifstream fin(gedpath);
		int cnt = 0;
		bool isX = true;
		while(getline(fin, line)) {
			if (line.length() < 2) continue;
			if (line[0] == '%' && line[1] == 'G') {
				string gid = line.substr(5, line.length() - 5);
				getline(fin, line);
				string lid = line.substr(5, line.length() - 5);
				GED ged(boost::lexical_cast<long>(gid), lid);
				_geds.emplace_back(ged);
				cnt++;
			} else if (line[0] == 'v') {
				vector<string> details;
				boost::split(details, line, boost::is_any_of("\t"));
				_geds[cnt-1].addV(boost::lexical_cast<long>(details[1]), boost::lexical_cast<long>(details[2]), details[3]);
			} else if (line[0] == 'e' && line[1] != 'q') {
				vector<string> details;
				boost::split(details, line, boost::is_any_of("\t"));
				string elabel = details[2].substr(1, details[2].length()-2);
				_geds[cnt-1].addE(boost::lexical_cast<long>(details[1]), boost::lexical_cast<long>(details[2]), boost::lexical_cast<long>(details[3]));
			} else if (line[0] == '%' && line[1] == 'X') {
				isX = true;
			} else if (line[0] == '%' && line[1] == 'Y') {
				isX = false;
			} else if (line[0] == 'e' && line[1] == 'q') {
				vector<string> details;
				boost::split(details, line, boost::is_any_of("\t"));
				if (details[0][3] == 'l') {
					if (isX) {
						_geds[cnt-1].setX_type(EQ_LET);
						_geds[cnt-1].addX_let(boost::lexical_cast<long>(details[1]), details[2]);
					} else {
						_geds[cnt-1].setY_type(EQ_LET);
						_geds[cnt-1].addY_let(boost::lexical_cast<long>(details[1]), details[2]);
					}
				} else if (details[0][3] == 'v') {
					if (isX) {
						_geds[cnt-1].setX_type(EQ_VAR);
						_geds[cnt-1].addX_let(boost::lexical_cast<long>(details[1]), details[2]);
						_geds[cnt-1].addX_let(boost::lexical_cast<long>(details[3]), details[4]);
					} else {
						_geds[cnt-1].setY_type(EQ_VAR);
						_geds[cnt-1].addY_let(boost::lexical_cast<long>(details[1]), details[2]);
						_geds[cnt-1].addY_let(boost::lexical_cast<long>(details[3]), details[4]);
					}
				}
			}
		}
	} catch (exception& e) {
		cout << e.what();
		exit(0);
	}
}

void CheckGED::printGEDs() {

}

void CheckGED::validation() {

}

void CheckGED::writeGEDs() {

}


int main(int argc, char **argv) {

	string filename = "";
	string gedpath = "";
	if (argc > 2) { //get 1 parameter: location
		filename = argv[1];
		gedpath = argv[2];
	} else {
		cout << "Parameter <file path> must be needed!" << endl;
		exit(0);
	}

	CheckGED cg;
	cg.loadGraph(filename);
	cg.loadGEDs(gedpath);

	//test
	cg.printGraph();
	cg.printGEDs();

  //validate
	cg.validation();
	//write file
	cg.writeGEDs();

	return 1;
}
