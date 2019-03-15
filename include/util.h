#include <iostream>

using namespace std;

enum GED_TYPE {
  EQ_LET = 0,
  EQ_VAR = 1,
  EQ_ID = 2,
};

/* check whether 2 vector do not have intersection */
template <typename T>
bool const interEmpty (vector<T> const& a, vector<T> const& b) {
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < b.size(); j++) {
      if (a[i] == b[i]) {
        return false;
      }
    }
  }
  return true;
}

bool pairComp(const pair<long, long>& a, const pair<long, long>& b){
  return a.first < b.first;
}

#ifdef BOOST_GRAPH

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/graph_utility.hpp>

// Define edge and vertex properties
typedef boost::property<boost::edge_name_t, long> edge_property;
typedef boost::property<boost::vertex_name_t, long, boost::property<boost::vertex_index_t, int> > vertex_property;
// Using a vecS graphs => the index maps are implicit.
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, vertex_property, edge_property> graph_type;

typedef boost::property_map<graph_type, boost::vertex_name_t>::type vertex_name_map_t;
typedef boost::property_map_equivalent<vertex_name_map_t, vertex_name_map_t> vertex_comp_t;
typedef boost::property_map<graph_type, boost::edge_name_t>::type edge_name_map_t;
typedef boost::property_map_equivalent<edge_name_map_t, edge_name_map_t> edge_comp_t;

// EdgeProperties
class EdgeProperties: public edge_property {
  public:
    EdgeProperties(vector<long> elabels) :_elabels(elabels) {}

    /* overload == */
    bool operator==(EdgeProperties const& other) const {
      for (auto& my_l:_elabels) {
        for (auto& o_l:other._elabels) {
          if (my_l == o_l) return true;
        }
      }
      return false;
    }
  private :
    vector<long> _elabels;
};

/* my call back, to contain the output in vf2_subgraph_iso */
template <typename Graph1, typename Graph2>
class my_call_back {

public:
  my_call_back(const Graph1& graph1, const Graph2& graph2) : _graph1(graph1), _graph2(graph2) {}

  template <typename CorrespondenceMap1To2, typename CorrespondenceMap2To1>
  bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1) {
    BGL_FORALL_VERTICES_T(v, _graph1, Graph1) {
      _vertex_iso_map.emplace_back(get(boost::vertex_index_t(), _graph1, v), get(boost::vertex_index_t(), _graph2, get(f, v)));
    }
    _set_of_vertex_iso_map.push_back(_vertex_iso_map);
    _vertex_iso_map.clear();
    return true;
  }

  std::vector <std::vector <std::pair<long, long>>> get_setvmap() { return _set_of_vertex_iso_map; }

private:
  const Graph1& _graph1;
  const Graph2& _graph2;
  std::vector <std::vector <std::pair<long, long>>> _set_of_vertex_iso_map;
  std::vector <std::pair<long, long>> _vertex_iso_map;
};

#endif
