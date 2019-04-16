#include <iostream>

using namespace std;

/* Util for common use
 * Author: Liucy
 * Date: 4,2019
 */

enum GED_TYPE {
  EQ_LET = 0,
  EQ_VAR = 1,
  EQ_ID = 2,
};

enum LOG_TYPE {
  SYSTEM = 0,
  INFO = 1,
  TEST = 2,
  WARNING = 3,
  ERROR = 4,
};

/* check one number whether existing in a vector */
template <typename T>
bool const exist(const vector<T>& a, const T& b) {
  for (auto it = a.begin(); it != a.end(); it++) {
    if (*it == b) return true;
  }
  return false;
}

/* check whether 2 vector do not have intersection */
template <typename T>
bool const interEmpty(vector<T> const& a, vector<T> const& b) {
  for (int i = 0; i < a.size(); i++)
    for (int j = 0; j < b.size(); j++)
      if (a[i] == b[i]) return false;

  return true;
}

bool pairComp(const pair<long, long>& a, const pair<long, long>& b) {
  return a.first < b.first;
}

class LOG {
 public:
  LOG() {}
  ~LOG() {}

  static void system(string str) { cout << "[CORE]~ " << str << std::endl; }
  static void system(string str, int id) {
    cout << "[CORE]~ " << str << ": " << id << std::endl;
  }

  static void info(string str) { cout << "[INFO]- " << str << std::endl; }
  static void info(string str, int id) {
    cout << "[INFO]- " << str << ": " << id << std::endl;
  }

  static void test(string str) { cout << "[TEST]* " << str << std::endl; }
  static void test(string str, int id) {
    cout << "[TEST]* " << str << ": " << id << std::endl;
  }

  static void warning(string str) { cout << "[WARN]! " << str << std::endl; }
  static void warning(string str, int id) {
    cout << "[WARN]! " << str << ": " << id << std::endl;
  }

  static void error(string str) { cout << "[EROR]x " << str << std::endl; }
  static void error(string str, int id) {
    cout << "[EROR]x " << str << ": " << id << std::endl;
  }
};

#ifdef BOOST_GRAPH

#include <boost/container/flat_set.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>

// two containers have intersection
template <typename SortedRange1, typename SortedRange2,
          typename V = std::common_type_t<
              typename boost::range_value<SortedRange1>::type,
              typename boost::range_value<SortedRange2>::type>,
          typename Cmp = std::less<V>>
static inline bool has_intersection(SortedRange1 const& a,
                                    SortedRange2 const& b, Cmp cmp = {}) {
  auto equivalent = [cmp](V const& a, V const& b) {
    return !cmp(a, b) && !cmp(b, a);
  };

  auto ai = a.begin();
  auto bi = b.begin();

  while (ai != a.end() && (bi = b.lower_bound(*ai)) != b.end())
    if (equivalent(*ai++, *bi)) return true;

  return false;
}

// Define edge and vertex properties
// EdgeProperties
struct EdgeProperties {
  using Labels = boost::container::flat_set<long, std::less<>>;

  EdgeProperties(std::initializer_list<long> elabels = {})
      : _elabels(elabels) {}
  EdgeProperties(long elabel) { _elabels.insert(elabel); }
  EdgeProperties(std::set<long> const& elabels) {
    for (auto& elabel : elabels) _elabels.insert(elabel);
  }

  bool operator==(EdgeProperties const& other) const {
    return has_intersection(_elabels, other._elabels);
  }

  Labels _elabels;
};

typedef boost::property<boost::edge_name_t, EdgeProperties> edge_property;
typedef boost::property<boost::vertex_name_t, long> vertex_property;

// Using a vecS graphs => the index maps are implicit.
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
                              vertex_property, edge_property>
    graph_type;

typedef boost::property_map<graph_type, boost::vertex_name_t>::type
    vertex_name_map_t;
typedef boost::property_map_equivalent<vertex_name_map_t, vertex_name_map_t>
    vertex_comp_t;
typedef boost::property_map<graph_type, boost::edge_name_t>::type
    edge_name_map_t;
typedef boost::property_map_equivalent<edge_name_map_t, edge_name_map_t>
    edge_comp_t;

// remove the self-loops in graph
struct FilterSelfEdges {
  graph_type const* _g;
  bool operator()(graph_type::edge_descriptor ed) const {
    return source(ed, *_g) != target(ed, *_g);
  }
};
using FilteredGraph = boost::filtered_graph<graph_type, FilterSelfEdges>;

/* my call back, to contain the output in vf2_subgraph_iso */
template <typename Graph1, typename Graph2>
class my_call_back {
 public:
  my_call_back(const Graph1& graph1, const Graph2& graph2)
      : _graph1(graph1), _graph2(graph2) {}

  template <typename CorrespondenceMap1To2, typename CorrespondenceMap2To1>
  bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1) {
    BGL_FORALL_VERTICES_T(v, _graph1, Graph1) {
      _vertex_iso_map.emplace_back(
          get(boost::vertex_index_t(), _graph1, v),
          get(boost::vertex_index_t(), _graph2, get(f, v)));
    }
    _set_of_vertex_iso_map.push_back(_vertex_iso_map);
    _vertex_iso_map.clear();
    return true;
  }

  std::vector<std::vector<std::pair<long, long>>> get_setvmap() {
    return _set_of_vertex_iso_map;
  }

 private:
  const Graph1& _graph1;
  const Graph2& _graph2;
  std::vector<std::vector<std::pair<long, long>>> _set_of_vertex_iso_map;
  std::vector<std::pair<long, long>> _vertex_iso_map;
};

#endif
