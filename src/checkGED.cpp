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
			if (line[0] == '%' && line[1] == 'G') { //read gid and lid
				string gid = line.substr(5, line.length() - 5);
				//cout << "G:" << gid << endl;
				getline(fin, line);
				string lid = line.substr(5, line.length() - 5);
				//cout << "L:" << lid << endl;
				GED ged(boost::lexical_cast<long>(gid), lid);
				_geds.emplace_back(ged);
				_active.emplace_back(true);
				++cnt;
			} else if (line[0] == 'v') { //read v
				vector<string> details;
				boost::split(details, line, boost::is_any_of("\t"));
				//cout << "ID:" << details[1] << ",LABEL:" << details[2] << ",VALUE:" << details[3] << endl;
				_geds[cnt-1].addV(boost::lexical_cast<long>(details[1]), boost::lexical_cast<long>(details[2]), details[3]);
			} else if (line[0] == 'e' && line[1] != 'q') { //read e
				vector<string> details;
				boost::split(details, line, boost::is_any_of("\t"));
				string elabel = details[2].substr(1, details[2].length()-2);
				//cout << "SRC:" << details[1] << ",LABEL:" << elabel << ",DST:" << details[3] << endl;
				if (!_geds[cnt-1].addE(boost::lexical_cast<long>(details[1]), boost::lexical_cast<long>(elabel), boost::lexical_cast<long>(details[3]))) {
					cout << "GED V ID does not exist, in GED " << cnt << " in File " << gedpath << "\n";
					exit(0);
				}
			} else if (line[0] == '%' && line[1] == 'X') { //X?
				isX = true;
			} else if (line[0] == '%' && line[1] == 'Y') { //Y?
				isX = false;
			} else if (line[0] == 'e' && line[1] == 'q') { //read literals
				vector<string> details;
				boost::split(details, line, boost::is_any_of("\t"));
				if (details[0][3] == 'l') {
					//format1: eq-let vid key value
					//format2: eq-let vid value
					//Only read in its vid and value because the format of graph is RDF which only has one key-value.
					if (isX) {
						if (details.size() == 4) {
							_geds[cnt-1].addX_let(boost::lexical_cast<long>(details[1]), details[3]);
						} else {
							_geds[cnt-1].addX_let(boost::lexical_cast<long>(details[1]), details[2]);
						}
					} else {
						if (details.size() == 4) {
							_geds[cnt-1].addY_let(boost::lexical_cast<long>(details[1]), details[3]);
						} else {
							_geds[cnt-1].addY_let(boost::lexical_cast<long>(details[1]), details[2]);
						}
					}
				} else if (details[0][3] == 'v') {
					if (isX) {
						_geds[cnt-1].addX_var(boost::lexical_cast<long>(details[1]), details[2], boost::lexical_cast<long>(details[3]), details[4]);
					} else {
						_geds[cnt-1].addY_var(boost::lexical_cast<long>(details[1]), details[2], boost::lexical_cast<long>(details[3]), details[4]);
					}
				} else if (details[0][3] == 'i') {
					if (isX) {
						_geds[cnt-1].addX_id(boost::lexical_cast<long>(details[1]), details[2], boost::lexical_cast<long>(details[3]));
					} else {
						_geds[cnt-1].addY_id(boost::lexical_cast<long>(details[1]), details[2], boost::lexical_cast<long>(details[3]));
					}
				}
			}
		}
	} catch(boost::bad_lexical_cast& e) {
		cout << e.what() << endl;
		exit(0);
	} catch (exception& e) {
		cout << e.what();
		exit(0);
	}
}

void CheckGED::printGEDs() {
	cout << "Total:" << _geds.size() << "\n\n";
	for (auto& ged:_geds) {
		string str = "";
		ged.toString(str);
		cout << str << "\n";
	}
}

void CheckGED::validation() {
	for (int i = 0; i < _geds.size(); i++) {
		cout << "Checking NO." << (i+1) << " GED";
		if(!_geds[i].existGED(_graph)) {
			_active[i] = false;
			cout << "\t0\n";
		} else {
			cout << "\t1\n";
		}
	}
	cout << "\nThe results:\n";
	for (int i = 0; i < _active.size(); i++) {
		if (_active[i]) {
			cout << "1\t";
		} else {
			cout << "0\t";
		}
	}
	cout << "\n";
}

void CheckGED::writeValidatedGEDs(const string& gedpath) {
	ofstream fout(gedpath + ".vali");
	int cnt = _active.size();
	for (int i = 0; i < cnt; i++) {
		if (_active[i]) {
			string res = "";
			_geds[i].toString(res);
			fout << res;
		}
	}
	fout.close();
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

	//print info
	cg.printGraph();
	cg.printGEDs();

  //validate
	cg.validation();
	//write file
	cg.writeValidatedGEDs(gedpath);

	return 1;
}
