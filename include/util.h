#include <iostream>

using namespace std;

enum GED_TYPE {
  EQ_LET = 0,
  EQ_VAR = 1,
  EQ_ID = 2,
};

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

#ifdef BOOST_GRAPH

#include <boost/graph/vf2_sub_graph_iso.hpp>

// Define edge and vertex properties
typedef boost::property<boost::edge_name_t, long> edge_property;
typedef boost::property<boost::vertex_name_t, long, boost::property<boost::vertex_index_t, int> > vertex_property;
// Using a vecS graphs => the index maps are implicit.
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, vertex_property, edge_property> graph_type;

typedef boost::property_map<graph_type, boost::vertex_name_t>::type vertex_name_map_t;
typedef boost::property_map_equivalent<vertex_name_map_t, vertex_name_map_t> vertex_comp_t;
typedef boost::property_map<graph_type, boost::edge_name_t>::type edge_name_map_t;
typedef boost::property_map_equivalent<edge_name_map_t, edge_name_map_t> edge_comp_t;

#endif
