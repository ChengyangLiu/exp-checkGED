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
    void reWriteGEDs(string& path);
		void validation();
		void writeValidatedGEDs(const string& gedpath);
    void delivery(string& gedpath, int frag_num);

#ifdef BOOST_GRAPH
    void convert2BG(Graph& g, vector<GED>& ged);
    void boost_vf2();
    void boost_readMap(string& mapfile);
    void boost_validation();

    inline graph_type& boost_graph() {return _boost_graph;}
    inline vector<graph_type>& boost_patterns() {return _boost_patterns;}

  private:
    graph_type _boost_graph;
    vector<graph_type> _boost_patterns;
    vector<vector<map<long, long>>> _maps; //(pattern id, graph id)
#endif

	private:
		Graph _graph;
		vector<GED> _geds;
		vector<bool> _active;
};
