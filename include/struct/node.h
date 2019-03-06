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

	private:
		Vertex _vertex; //source
		vector<long> _neighbors; //destinations
		vector<long> _elabels; //labels
};
