#include <exception>
#include <fstream>
#include <map>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include "./node.h"

using namespace std;

/* class graph */
class Graph {
 public:
  Graph() {}
  ~Graph() {}

  /* load graph */
  void loadGraph(const string& filename) {
    string line;
    long src, dst, label;
    string value;
    long cnt = 0;
    try {
      ifstream fin_v(filename + ".v");
      // read vertex in
      while (getline(fin_v, line)) {
        stringstream ss(line);
        ss >> src >> label >> value;
        Vertex v(src, label, value);
        Node node(v);
        _nodes.emplace_back(node);
        _vid_map[src] = cnt++;
      }
      ifstream fin_e(filename + ".e");
      // read its neighbors in
      int line_cnt = 0;
      while (getline(fin_e, line)) {
        stringstream ss(line);
        ss >> src >> label >> dst;
        line_cnt++;
        if (!Graph::nodeExist(src)) {
          cout << "Vertex ID: " << src << " does not exist, Line " << line_cnt
               << " in File " << filename << ".e\n";
          exit(1);
        }
        if (!Graph::nodeExist(dst)) {
          cout << "Vertex ID: " << dst << " does not exist, Line " << line_cnt
               << " in File " << filename << ".e\n";
          exit(1);
        }
        Node& node = Graph::node(src);
        node.addNeighbor(dst, label);
      }
    } catch (exception& e) {
      cout << e.what();
      exit(1);
    }
  }

  /* print the graph */
  void graphString(string& str) {
    try {
      for (auto& node : _nodes) {
        Vertex& vertex = node.v();
        string value = vertex.value();
        str = str + "v\t" + boost::lexical_cast<string>(vertex.id()) + "\t" +
              boost::lexical_cast<string>(vertex.label()) + "\t" +
              vertex.value() + "\n";
      }
      for (auto& node : _nodes) {
        Vertex& vertex = node.v();
        vector<long>& neighbors = node.neighbors();
        vector<long>& elabels = node.elabels();
        int len = neighbors.size();
        for (int i = 0; i < len; i++) {
          Vertex& dst = Graph::node(neighbors[i]).v();
          str = str + "e\t" + boost::lexical_cast<string>(vertex.id()) + "\t{" +
                boost::lexical_cast<string>(elabels[i]) + "}\t" +
                boost::lexical_cast<string>(dst.id()) + "\n";
        }
      }
    } catch (boost::bad_lexical_cast& e) {
      cout << e.what() << endl;
      exit(1);
    }
  }

  /* rewrite graph (vertexes start from 0 and are continous) */
  void rewriteGraph(const string& path) {
    try {
      ofstream fvout(path + ".v");
      for (auto& node : _nodes) {
        Vertex& vertex = node.v();
        string value = vertex.value();
        fvout << Graph::pos(vertex.id()) << "\t" << vertex.label() << "\t"
              << vertex.value() << "\n";
      }
      fvout.close();
      ofstream feout(path + ".e");
      for (auto& node : _nodes) {
        Vertex& vertex = node.v();
        vector<long>& neighbors = node.neighbors();
        vector<long>& elabels = node.elabels();
        int len = neighbors.size();
        for (int i = 0; i < len; i++) {
          Vertex& dst = Graph::node(neighbors[i]).v();
          feout << Graph::pos(vertex.id()) << "\t" << elabels[i] << "\t"
                << Graph::pos(dst.id()) << "\n";
        }
      }
      feout.close();
    } catch (std::exception& e) {
      cout << e.what() << endl;
      exit(1);
    }
  }

  vector<Node>& allNodes() { return _nodes; }
  /* get node by given vid, only for existed id */
  inline Node& node(long vid) { return _nodes[_vid_map[vid]]; }

 private:
  /* check vid exist */
  inline bool nodeExist(long vid) {
    return _vid_map.find(vid) != _vid_map.end() ? true : false;
  }
  /* given vid, return its position */
  inline int pos(long vid) {
    return _vid_map.find(vid) != _vid_map.end() ? _vid_map[vid] : -1;
  }

  vector<Node> _nodes;       // nodes of graph
  map<long, long> _vid_map;  //(original VID, its position)
};
