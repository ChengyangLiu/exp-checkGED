#include <fstream>
#include <sstream>
#include <map>
#include <exception>

#include <boost/lexical_cast.hpp>

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
        ifstream fin_e(filename + ".e");
        // read its neighbors in
        while(getline(fin_e, line)) {
          stringstream ss(line);
          ss >> src >> label >> dst;
          Node& node = Graph::node(src);
          node.addNeighbor(dst, label);
        }
      } catch (exception& e) {
        cout << e.what();
        exit(0);
      }
    }

    /* print the graph */
    void graphString(string& str) {
      try {
        for(auto& node:_nodes) {
          Vertex& vertex = node.v();
          string value = vertex.value();
          str = str + "v\t" + boost::lexical_cast<string>(vertex.id()) + "\t"
          + boost::lexical_cast<string>(vertex.label()) + "\t" + vertex.value() + "\n";
        }
        for(auto& node:_nodes) {
          Vertex& vertex = node.v();
          vector<long>& neighbors = node.neighbors();
          vector<long>& elabels = node.elabels();
          int len = neighbors.size();
          for(int i = 0; i < len; i++) {
            Vertex& dst = Graph::node(neighbors[i]).v();
            str = str + "e\t" + boost::lexical_cast<string>(vertex.id()) + "\t{"
            + boost::lexical_cast<string>(elabels[i]) + "}\t" + boost::lexical_cast<string>(dst.id()) + "\n";
          }
        }
      } catch(boost::bad_lexical_cast& e) {
        cout << e.what() << endl;
        exit(0);
      }
    }

  private:
		inline Node& node(long vid) { return _nodes[_vid_map[vid]];}

		vector<Node> _nodes; //nodes of graph
		map<long, long> _vid_map; //(original VID, its position)
};
