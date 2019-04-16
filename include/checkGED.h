#include <iostream>

#include "./struct/ged.h"

using namespace std;

/* Class: exp-checkGED is a tool to validate GEDs or GFDs
 * Author: Liucy
 * Date: 4,2019
 */
class CheckGED {
 public:
  CheckGED() {}
  ~CheckGED() {}

  inline Graph& graph() { return _graph; }
  inline vector<GED>& geds() { return _geds; }

  /* load graph from file */
  void loadGraph(const string& filename) { _graph.loadGraph(filename); }
  /* load GEDs from file */
  void loadGEDs(const string& gedpath);
  /* load GED state from file */
  void loadState(const string& stafile);

  void printGraph() {
    string res = "";
    _graph.graphString(res);
    LOG::test(res);
  }
  void printGEDs();

  /* rewrite GEDs, make vertices start from 0 and continous */
  void reWriteGEDs(const string& path);
  /* validate GEDs */
  void validation();
  /* write correct GEDs into file */
  void writeValidatedGEDs(const string& gedpath);

  /* separate the GEDs into many parts */
  void delivery(const string& gedpath, int frag_num);

  /* classify GEDs, connected or disconnected */
  void classify();

#ifdef BOOST_GRAPH
  /* CAUTION: the graph's and GEDs' id must start from 0 and be continous!
   * Support multi-elabels but only one vlabel.
   * /*BUG: Maybe boost vf2 bug. self-loops leads to wrong match.
   * For example: P1:0(a)-{b}->1(a),0(a)-{c}->0(a) can be matched in
   * G:0(a)-{b}->1(a),0(a)-{c}->0(a),0(a)-{b}->2(a);
   * but P2:0(a)-{b}->1(a) can not be matched in G.
   * FixBug: Filter big graph's self-loops and then do pattern matching.
   * So only support patterns without self-loops.
   */

  /* load isomorphism mapping from file */
  void loadMapping(const string& mapfile);
  /* convert GEDs and Graph into boost graph */
  void convert2BG();
  /* ignore patterns that do not match with given condition */
  void boost_filter();
  /* use boost_vf2_isomorphism to produce mapping from patterns to graph */
  void boost_vf2();
  /* validate the literals in GEDs */
  void boost_validation();

  /* output mapping */
  void boost_writeMapping(const string& mapfile);
  /* unactivate GED whose number of isomorphism is less than k */
  void filter_k(int k) {
    if (k >= 10) {
      for (int i = 0; i < _active.size(); i++) {
        if (_maps[i].size() < k) _active[i] = false;
      }
    } else {
      for (int i = 0; i < _active.size(); i++) {
        if (_maps[i].size() != k) _active[i] = false;
      }
    }
  }

  inline graph_type& boost_graph() { return _boost_graph; }

  inline vector<vector<graph_type>>& boost_patterns() {
    return _boost_patterns;
  }

 private:
  /* print the state of GEDs */
  void printRes() {
    string res = "\nThe results:\n";
    for (int i = 0; i < _active.size(); i++) {
      res += _active[i] ? "1\t" : "0\t";
      if ((i + 1) % 5 == 0) res += "\n";
    }
    LOG::test(res);
  }

  graph_type _boost_graph;                     // boost graph
  vector<vector<graph_type>> _boost_patterns;  // GEDs vector
  vector<vector<map<long, long>>> _maps;       // (pattern id, graph id)

#endif

 private:
  Graph _graph;          // graph
  vector<GED> _geds;     // GEDs
  vector<bool> _active;  // real or fake

  /* for GEDs consisting with disconnected patterns */
  vector<bool> _connected;                            // GED is connected?
  vector<vector<map<long, long>>> _disconnected_maps;  // (map id, pattern id)
};
