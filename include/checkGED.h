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
		void printGraph() {_graph.printGraph();}
		void printGEDs();
		void validation();
		void writeGEDs();

	private:
		Graph _graph;
		vector<GED> _geds;
		vector<bool> _active;
};
