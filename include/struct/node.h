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

    /* add a neighborhood vertex to an exist vertex */
		void addNeighbor(Vertex* neighbor, long elabel) {
			_neighbors.emplace_back(neighbor);
			_elabels.emplace_back(elabel);
		}

		inline Vertex& v() { return _vertex;}
		inline vector<Vertex*>& neighbors() {return _neighbors;}
		inline vector<long>& elabels() {return _elabels;}

	private:
		Vertex _vertex; //source
		vector<Vertex*> _neighbors; //destinations
		vector<long> _elabels; //labels
};
