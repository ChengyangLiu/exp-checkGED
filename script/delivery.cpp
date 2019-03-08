#include <iostream>

#include <boost/lexical_cast.hpp>

using namespace std;

int main(int argc, char **argv) {
  string gedpath = "";
  int frag_num = 0;
	if (argc > 2) { //get 1 parameter: location
		gedpath = argv[1];
    frag_num = atoi(argv[2].c_str());
	} else {
		cout << "Parameter <file path> must be needed!" << endl;
		exit(0);
	}
	string line;
  int ged_cnt = 0;
	try{
		ifstream fin(gedpath);
    vector<string> strList;
    while(getline(fin, line)) {
      if (line[0] == '%' && line[1] == 'G') { ged_cnt++;}
      strList.emplace_back(line);
    }
    cont int volume = ged_cnt/frag_num + 1;

    int frag_id = 0;
    int i = 0;
    string gedfile = gedpath + boost::lexical_cast<string>(frag_id);
    ofstream fout(gedfile);
    for (auto& s:strList) {
      if (s[0] == '%' && s[1] == 'G') {
        i++;
        if (i == volume) {
          frag_id++;
          gedfile = gedpath + boost::lexical_cast<string>(frag_id);
          fout.close();
          ofstream fout(gedfile);
        }
      }
      fout << s;
    }
    fout.close();
  }
}
