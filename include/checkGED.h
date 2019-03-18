#include <iostream>

#include "./struct/ged.h"

using namespace std;

class CheckGED {
  public:
		CheckGED(){}
		~CheckGED(){}

		inline Graph& graph() {return _graph;}
		inline vector<GED>& geds() {return _geds;}

    /* load graph from file */
		void loadGraph(const string& filename) {_graph.loadGraph(filename);}
    /* load GEDs from file */
    void loadGEDs(const string& gedpath);

		void printGraph() {
      string res = "";
      _graph.graphString(res);
      cout << res;
    }
		void printGEDs();

    /* rewrite GEDs, make vertices start from 0 and continous */
    void reWriteGEDs(string& path);
    /* validate GEDs */
    void validation();
    /* write correct GEDs into file */
		void writeValidatedGEDs(const string& gedpath);

    /* separate the GEDs into many parts */
    void delivery(string& gedpath, int frag_num);

#ifdef BOOST_GRAPH
/* CAUTION: the graph's and GEDs' id must start from 0 and be continous!
 * Support multi-elabels but only one vlabel.
 * BUG: Maybe boost vf2 bug. self-loops leads to wrong match.
 * For example: P1:0(a)-{b}->1(a),0(a)-{c}->0(a) can match G:0(a)-{b}->1(a),0(a)-{c}->0(a),0(a)-{b}->2(a);
 * but P2:0(a)-{b}->1(a) can not match G.
 */

    /* convert GEDs and Graph into boost graph */
    void convert2BG(Graph& g, vector<GED>& ged);
    /* ignore patterns that do not match with given condition */
    void boost_filter();
    /* use boost_vf2_isomorphism to produce mapping from patterns to graph */
    void boost_vf2();
    /* validate the literals in GEDs */
    void boost_validation();

    inline graph_type& boost_graph() {return _boost_graph;}
    inline vector<graph_type>& boost_patterns() {return _boost_patterns;}

    /* output mapping */
    void boost_writeMapping(string& mapfile);

  private:
    graph_type _boost_graph; //boost graph
    vector<graph_type> _boost_patterns; //GEDs vector
    vector<vector<map<long, long>>> _maps; //(pattern id, graph id)
#endif

	private:
		Graph _graph; //graph
		vector<GED> _geds; //GEDs
		vector<bool> _active;  //real or fake
};
