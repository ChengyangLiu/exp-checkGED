#include <fstream>
#include <sstream>
#include <map>
#include <exception>

#include "./node.h"

using namespace std;

/* class graph */
class Graph {
  public:
    Graph(){}
    ~Graph(){}

    /* load graph */
    void loadGraph(const string& filename) {
      string line;
      long src, dst, label;
      string value;
      long cnt = 0;
      try{
        ifstream fin_v(filename + ".v");
        // read vertex in
        while(getline(fin_v, line)) {
          stringstream ss(line);
          ss >> src >> label >> value;
          Vertex v(src, label, value);
          Node node(v);
          _nodes.emplace_back(node);
          _vid_map[src] = cnt++;
        }
        _node_num = _nodes.size();
        ifstream fin_e(filename + ".e");
        // read its neighbors in
        while(getline(fin_e, line)) {
          stringstream ss(line);
          ss >> src >> label >> dst;
          Node& node = Graph::node(src);
          node.addNeighbor(&(Graph::node(dst).v()), label);
        }
      } catch (exception& e) {
        cout << e.what();
        exit(0);
      }
    }

    /* print the graph */
    void printGraph() {
      for(auto& node:_nodes) {
        Vertex& vertex = node.v();
        vector<Vertex*>& neighbors = node.neighbors();
        vector<long>& elabels = node.elabels();
        cout << vertex.id() << "," << vertex.label() << "," << vertex.value();
        int len = neighbors.size();
        for(int i = 0; i < len; i++) {
          cout << "," << neighbors[i]->id() << "," << elabels[i];
        }
        cout << "\n";
      }
    }

  private:
		inline Node& node(long vid) { return _nodes[_vid_map[vid]];}

		long _node_num; //as max node id
		vector<Node> _nodes; //nodes of graph
		map<long, long> _vid_map; //(original VID, its position)
};
