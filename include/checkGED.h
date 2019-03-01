#include <iostream>

#include "./struct/ged.h"

using namespace std;

class CheckGED {
  public:
		CheckGED(){}
		~CheckGED(){}

		inline Graph& graph() {return _graph;}
		inline vector<GED>& geds() {return _geds;}

		void loadGraph(const string& filename) {_graph.loadGraph(filename);}
		void loadGEDs(const string& gedpath);
		void printGraph() {
      string res = "";
      _graph.graphString(res);
      cout << res;
    }
		void printGEDs();
		void validation();
		void writeValidatedGEDs(const string& gedpath);

	private:
		Graph _graph;
		vector<GED> _geds;
		vector<bool> _active;
};
