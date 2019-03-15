#include <vector>

#include "./vertex.h"

using namespace std;

/* node of a graph, that's an encapsulation for vertex */
class Node {
  public:
    Node(){}
    ~Node(){}

		Node(Vertex& vertex) {
			_vertex = vertex;
		}

    Node(Vertex& vertex, vector<long>& neighbors, vector<long>& elabels) {
      _vertex = vertex;
      _neighbors = neighbors;
      _elabels = elabels;
    }

    /* add a neighborhood vertex to an exist vertex */
		void addNeighbor(long dst, long elabel) {
			_neighbors.emplace_back(dst);
			_elabels.emplace_back(elabel);
		}

		inline Vertex& v() { return _vertex;}
		inline vector<long>& neighbors() {return _neighbors;}
		inline vector<long>& elabels() {return _elabels;}

    /* given neighbor, obtain its elabels */
    void elabel(Node& node, vector<long>& elabels) {
      long id = node.v().id();
      for (int i = 0; i < _neighbors.size(); i++) {
        if (id == _neighbors[i]) {
          elabels.emplace_back(_elabels[i]);
        }
      }
    }

    /* get all nodes in given graph whose states are the same with present node */
    void getIsomorNodes (vector<Node>& g, vector<Node>& isomor_nodes) {
      // check self
      vector<Node> t1;
      for (auto& n:g) {
        if (n.v().label() == _vertex.label()) {
          t1.emplace_back(n);
        }
      }
      // check its neighbors
      vector<long>& src_neighbors = _neighbors;
      vector<long>& src_elabels = _elabels;
      vector<Node> t2;
      for (auto& n:t1) {
        vector<long>& n_neighbors = n.neighbors();
        vector<long>& n_elabels = n.elabels();
        int src_len = src_neighbors.size();
        int n_len = n_neighbors.size();
        if (src_len != n_len) continue;
        int i;
        for (i = 0; i < src_len; i++) {
          if (src_neighbors[i] != n_neighbors[i] || src_elabels[i] != n_elabels[i]) break;
        }
        if (i == src_len) {
          t2.emplace_back(n);
        }
      }
      // check its parents
      vector<Node> src_parents;
      vector<long> src_p_elabels;
      getParents(g, _vertex.id(), src_parents, src_p_elabels);
      for (auto& n:t2) {
        vector<Node> n_parents;
        vector<long> n_p_elabels;
        getParents(g, n.v().id(), n_parents, n_p_elabels);
        int src_len = src_parents.size();
        int n_len = n_parents.size();
        if (src_len != n_len) continue;
        int i;
        for (i = 0; i < src_len; i++) {
          if (src_parents[i].v().id() != n_parents[i].v().id() || src_p_elabels[i] != n_p_elabels[i]) break;
        }
        if (i == src_len) {
          isomor_nodes.emplace_back(n);
        }
      }
    }

    /* get its parents and elabels by given id */
    void getParents(vector<Node>& g, const long id, vector<Node>& parents, vector<long>& elabels) {
      for (auto& n:g) {
        vector<long>& n_neighbors = n.neighbors();
        vector<long>& n_elabels = n.elabels();
        for (int i = 0; i < n_neighbors.size(); i++) {
          if (n_neighbors[i] == id) {
            parents.emplace_back(n);
            elabels.emplace_back(n_elabels[i]);
            break;
          }
        }
      }
    }

	private:
		Vertex _vertex; //source
		vector<long> _neighbors; //destinations
		vector<long> _elabels; //labels
};
