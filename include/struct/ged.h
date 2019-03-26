#include <iostream>
#include <map>
#include <set>
#include <stack>

#include "../util.h"
#include "./graph.h"

using namespace std;

class GED {
 public:
  GED() {}
  ~GED() {}

  GED(string& gid, string& lid) : _gid(gid), _lid(lid), _cnt(0) {}

  GED(string& gid, string& lid, map<long, long>& id_map, long cnt,
      vector<Node>& nodes, vector<GED_TYPE>& x_type, vector<GED_TYPE>& y_type,
      map<long, string>& x_matches, map<long, string>& y_matches)
      : _gid(gid),
        _lid(lid),
        _id_map(id_map),
        _cnt(cnt),
        _nodes(nodes),
        _x_type(x_type),
        _y_type(y_type),
        _x_matches(x_matches),
        _y_matches(y_matches) {}

  void addV(long id, long label, string& value) {
    Vertex v(id, label, value);
    Node node(v);
    _nodes.emplace_back(node);
    _id_map[id] = _cnt++;
  }

  bool addE(long src, long label, long dst) {
    if (!GED::nodeExist(src) || !GED::nodeExist(dst)) {
      return false;
    }
    Node& node = GED::node(src);
    node.addNeighbor(dst, label);
    return true;
  }

  inline void addX_let(long id, string& value) {
    _x_type.emplace_back(EQ_LET);
    _x_matches[id] = value;
  }
  inline void addY_let(long id, string& value) {
    _y_type.emplace_back(EQ_LET);
    _y_matches[id] = value;
  }

  inline void addX_var(long a_id, string& a_value, long b_id, string& b_value) {
    _x_type.emplace_back(EQ_VAR);
    _x_matches[a_id] = a_value;
    _x_matches[b_id] = b_value;
  }
  inline void addY_var(long a_id, string& a_value, long b_id, string& b_value) {
    _y_type.emplace_back(EQ_VAR);
    _y_matches[a_id] = a_value;
    _y_matches[b_id] = b_value;
  }

  inline void addX_id(long a_id, string& value, long b_id) {
    _x_type.emplace_back(EQ_ID);
    _x_matches[a_id] = value;
    _x_matches[b_id] = value;
  }
  inline void addY_id(long a_id, string& value, long b_id) {
    _y_type.emplace_back(EQ_ID);
    _y_matches[a_id] = value;
    _y_matches[b_id] = value;
  }

  inline vector<Node>& pattern() { return _nodes; }
  inline vector<GED_TYPE>& xtype() { return _x_type; }
  inline vector<GED_TYPE>& ytype() { return _y_type; }
  inline map<long, string>& xmatches() { return _x_matches; }
  inline map<long, string>& ymatches() { return _y_matches; }

  inline string& gid() { return _gid; }
  inline string& lid() { return _lid; }

  /* get pattern string */
  void patternString(string& str, bool remap) {
    try {
      for (auto& node : _nodes) {
        Vertex& vertex = node.v();
        string value = vertex.value();
        if (remap) {
          str = str + "v\t" +
                boost::lexical_cast<string>(GED::pos(vertex.id())) + "\t" +
                boost::lexical_cast<string>(vertex.label()) + "\t" +
                vertex.value() + "\n";
        } else {
          str = str + "v\t" + boost::lexical_cast<string>(vertex.id()) + "\t" +
                boost::lexical_cast<string>(vertex.label()) + "\t" +
                vertex.value() + "\n";
        }
      }
      for (auto& node : _nodes) {
        vector<long>& neighbors = node.neighbors();
        vector<long>& elabels = node.elabels();
        int len = neighbors.size();
        for (int i = 0; i < len; i++) {
          Vertex& dst = GED::node(neighbors[i]).v();
          if (remap) {
            str = str + "e\t" +
                  boost::lexical_cast<string>(GED::pos(node.v().id())) + "\t" +
                  boost::lexical_cast<string>(elabels[i]) + "\t" +
                  boost::lexical_cast<string>(GED::pos(dst.id())) + "\n";
          } else {
            str = str + "e\t" + boost::lexical_cast<string>(node.v().id()) +
                  "\t" + boost::lexical_cast<string>(elabels[i]) + "\t" +
                  boost::lexical_cast<string>(dst.id()) + "\n";
          }
        }
      }
    } catch (boost::bad_lexical_cast& e) {
      cout << "PatternStringError: " << e.what() << "\n";
      exit(1);
    }
  }

  /* get literal string */
  void literalString(string& str, bool remap) {
    str += "%X\n";
    map<long, string>::iterator it = _x_matches.begin();
    try {
      if (remap) {
        for (auto type : _x_type) {
          if (type == EQ_LET) {
            str += "eq-let\t" +
                   boost::lexical_cast<string>(GED::pos(it->first)) + "\t";
            str += (it++)->second + "\n";
          } else if (type == EQ_VAR) {
            str += "eq-var\t" +
                   boost::lexical_cast<string>(GED::pos(it->first)) + "\t";
            str += (it++)->second + "\t";
            str += boost::lexical_cast<string>(GED::pos(it->first)) + "\t";
            str += (it++)->second + "\n";
          } else if (type == EQ_ID) {
            str += "eq-id\t" +
                   boost::lexical_cast<string>(GED::pos(it->first)) + "\t";
            str += (it++)->second + "\t";
            str += boost::lexical_cast<string>(GED::pos(it->first)) + "\t";
            str += (it++)->second + "\n";
          }
        }
      } else {
        for (auto type : _x_type) {
          if (type == EQ_LET) {
            str += "eq-let\t" + boost::lexical_cast<string>(it->first) + "\t";
            str += (it++)->second + "\n";
          } else if (type == EQ_VAR) {
            str += "eq-var\t" + boost::lexical_cast<string>(it->first) + "\t";
            str += (it++)->second + "\t";
            str += boost::lexical_cast<string>(it->first) + "\t";
            str += (it++)->second + "\n";
          } else if (type == EQ_ID) {
            str += "eq-id\t" + boost::lexical_cast<string>(it->first) + "\t";
            str += (it++)->second + "\t";
            str += boost::lexical_cast<string>(it->first) + "\t";
            str += (it++)->second + "\n";
          }
        }
      }
      str += "%Y\n";
      it = _y_matches.begin();
      if (remap) {
        for (auto type : _y_type) {
          if (type == EQ_LET) {
            str += "eq-let\t" +
                   boost::lexical_cast<string>(GED::pos(it->first)) + "\t";
            str += (it++)->second + "\n";
          } else if (type == EQ_VAR) {
            str += "eq-var\t" +
                   boost::lexical_cast<string>(GED::pos(it->first)) + "\t";
            str += (it++)->second + "\t";
            str += boost::lexical_cast<string>(GED::pos(it->first)) + "\t";
            str += (it++)->second + "\n";
          } else if (type == EQ_ID) {
            str += "eq-id\t" +
                   boost::lexical_cast<string>(GED::pos(it->first)) + "\t";
            str += (it++)->second + "\t";
            str += boost::lexical_cast<string>(GED::pos(it->first)) + "\t";
            str += (it++)->second + "\n";
          }
        }
      } else {
        for (auto type : _y_type) {
          if (type == EQ_LET) {
            str += "eq-let\t" + boost::lexical_cast<string>(it->first) + "\t";
            str += (it++)->second + "\n";
          } else if (type == EQ_VAR) {
            str += "eq-var\t" + boost::lexical_cast<string>(it->first) + "\t";
            str += (it++)->second + "\t";
            str += boost::lexical_cast<string>(it->first) + "\t";
            str += (it++)->second + "\n";
          } else if (type == EQ_ID) {
            str += "eq-id\t" + boost::lexical_cast<string>(it->first) + "\t";
            str += (it++)->second + "\t";
            str += boost::lexical_cast<string>(it->first) + "\t";
            str += (it++)->second + "\n";
          }
        }
      }
    } catch (boost::bad_lexical_cast& e) {
      cout << "LiteralStringError: " << e.what() << "\n";
      exit(1);
    }
  }

  /* validate GED */
  bool validateGED(Graph& g) {
    vector<vector<Node>> g_cans;
    vector<Node>& g_allNodes = g.allNodes();
    vector<Node>& p_nodes = _nodes;
    const int cnt = p_nodes.size();
    // obetain all possible candidates in graph
    // CAUTION: value of vertex is not considered. (all are ANY)
    for (auto& p_node : p_nodes) {
      vector<Node> g_can;
      for (auto& g_node : g_allNodes) {
        if (g_node.v().label() == p_node.v().label()) {
          g_can.emplace_back(g_node);
        }
      }
      g_cans.emplace_back(g_can);
    }
    /// test
    /*for (auto& g_can:g_cans) {
      for (auto& g:g_can) {
        cout << g.v().id() << "," << g.v().label() << "," << g.v().value() <<
    "\t";
      }
      cout << "\n";
    }*/

    vector<int> layer;  // record position in graph pattern
    for (int i = 0; i < cnt; i++) {
      int can_num = g_cans[i].size() - 1;
      layer.emplace_back(can_num);
      // cout << can_num << "\t"; //test
    }
    // cout << "\n"; //test
    vector<int> old = layer;

    vector<Node> isomor_nodes;
    set<string> values;
    int find_num = 0;  // the number of finding this ged
    int t_cnt = 0;
    bool iterate = true;
    while (iterate) {
      // cout << "T_num:" << t_cnt++ << "\n"; //test
      // update candidates
      for (int i = 0; i < layer.size(); i++) {
        if (layer[i] == 0) {
          layer[i] = old[i];
        } else {
          layer[i]--;
          break;
        }
      }
      // if all posibilities are iterated, then break
      int flag = 0;
      for (int i = 0; i < cnt; i++) {
        // cout << "N:" << layer[i] << ","; //test
        flag += layer[i];
      }
      // cout << flag << "\n"; //test
      if (flag == 0) {
        iterate = false;
      }
      // except homomorphism
      set<long> g_v;
      for (int i = 0; i < cnt; i++) {
        g_v.emplace(g_cans[i][layer[i]].v().id());
      }
      if (g_v.size() != cnt) continue;

      // check validation of pattern
      bool isP = true;
      for (int i = 0; i < p_nodes.size(); i++) {
        if (p_nodes[i].neighbors().size() ==
            0) {  // only check node with neighbors in pattern
          continue;
        }
// check edge relation
#ifdef E_FIRST  // check edges' info first
        vector<long>& p_neighbors = p_nodes[i].neighbors();
        Node& g_node = g_cans[i][layer[i]];
        vector<long>& g_neighbors = g_node.neighbors();
        if (p_neighbors.size() > g_neighbors.size()) {
          // if num of p_node's neighbors > num of g_node's neighbors, pruning
          isP = false;
          break;
        }
        bool findNode = true;
        vector<long>& p_elabels = p_nodes[i].elabels();
        vector<long>& g_elabels = g_node.elabels();
        for (int j = 0; j < p_neighbors.size(); j++) {
          int k;
          bool sameELabel = false;
          for (k = 0; k < g_elabels.size(); k++) {
            // cout << "Elabel:" << p_elabels[j] << "," << g_elabels[k] << "\n";
            // //test
            if (g_elabels[k] == p_elabels[j]) {  // elabel is equal
              sameELabel = true;
              Node& p_dst_node = GED::node(p_neighbors[j]);
              int pos = GED::pos(p_dst_node);
              Node& g_dst_node = g_cans[pos][layer[pos]];
              // test
              /*cout << "P:" << p_dst_node.v().id() << "," <<
              p_dst_node.v().label() << "," << p_elabels[j] << "\n";
              cout << "G:" << g_dst_node.v().id() << "," <<
              g_dst_node.v().label() << ",{";
              vector<long> g_test;
              g_node.elabel(g_dst_node, g_test);
              for (auto tt:g_test) {
                cout << tt << ",";
              }
              cout << "}\n";
              cout << "Same:" << g_neighbors[k] << "," << g_dst_node.v().id() <<
              "\n";
              cout << "NVlabel:" << p_dst_node.v().label() << "," <<
              g_dst_node.v().label() << "\n";*/
              if (g_neighbors[k] == g_dst_node.v().id() &&
                  p_dst_node.v().label() == g_dst_node.v().label()) {
                // judge whether structures are same
                // judge whether neighbor's label is equal
                break;
              }
            }
          }
          if (!sameELabel) {  // present g_node don't have satisfying neighbors
                              // with given elabel, so remove it.
            g_cans[i].erase(g_cans[i].begin() + layer[i]);
            if (old[i] == 0) {
              if ((_x_type.size() != 0 && _x_type[0] == EQ_ID) ||
                  (_y_type.size() != 0 && _y_type[0] == EQ_ID)) {
                return true;
              } else {
                return false;
              }
            }
            old[i]--;
            findNode = false;
            break;
          }
          if (k == g_elabels.size()) {
            findNode = false;
            break;
          }
        }
        if (!findNode) {
          isP = false;
          break;
        }
#elif V_FIRST  // check neighbors' info first
        // Discarded: do not have pruning, too much cost of time
        vector<long>& p_neighbors = p_nodes[i].neighbors();
        Node& g_node = g_cans[i][layer[i]];
        if (p_neighbors.size() > g_node.neighbors().size()) {
          // if num of p_node's neighbors > num of g_node's neighbors, pruning
          isP = false;
          break;
        }
        vector<long>& p_elabels = p_nodes[i].elabels();
        for (int j = 0; j < p_neighbors.size(); j++) {
          Node& p_dst_node = GED::node(p_neighbors[j]);
          int pos = GED::pos(p_dst_node);
          Node& g_dst_node = g_cans[pos][layer[pos]];
          /*cout << "P:" << p_dst_node.v().id() << "," << p_dst_node.v().label()
          << "," << p_elabels[j] << "\n";
          cout << "G:" << g_dst_node.v().id() << "," << g_dst_node.v().label()
          << ",{";
          vector<long> g_test;
          g_node.elabel(g_dst_node, g_test);
          for (auto tt:g_test) {
            cout << tt << ",";
          }
          cout << "}\n";*/
          if (g_node.v().id() == g_dst_node.v().id() ||
              p_dst_node.v().label() != g_dst_node.v().label()) {
            // remove self circle
            // judge whether neighbor's label is equal
            isP = false;
            break;
          } else {  // judge whether neighbor's elabel is equal
            vector<long> g_elabels;
            g_node.elabel(g_dst_node, g_elabels);
            int k = 0;
            int label_num = g_elabels.size();
            for (; k < label_num; k++) {
              if (g_elabels[k] == p_elabels[j]) break;
            }
            if (k == label_num) {
              isP = false;
              break;
            }
          }
        }
#endif
      }
      // check validation of literals
      bool isX = true;
      bool isY = true;
      if (isP) {
        // cout << "Pattern is Correct!\n"; //test
        // check literal
        for (int j = 0; j < 2; j++) {
          vector<GED_TYPE>& types = (j == 0) ? _x_type : _y_type;
          map<long, string>::iterator it =
              (j == 0) ? _x_matches.begin() : _y_matches.begin();
          for (auto& type : types) {
            if (type == EQ_LET) {
              long vid = it->first;
              string value = (it++)->second;
              int pos = GED::pos(vid);
              if (pos == -1) {
                return false;
              }
              // cout << "value:" << value << "," <<
              // g_cans[pos][layer[pos]].v().value() << "\n"; //test
              if (value != g_cans[pos][layer[pos]].v().value()) {
                if (j == 0) {
                  isX = false;
                  break;
                } else {
                  if (values.size() == 0) {
                    vector<Node> g_tmp;
                    for (int k = 0; k < layer.size(); k++) {
                      g_tmp.emplace_back(g_cans[k][layer[k]]);
                    }
                    g_cans[pos][layer[pos]].getIsomorNodes(g_tmp, isomor_nodes);
                  }
                  bool find = false;
                  for (auto& node : isomor_nodes) {
                    if (value == node.v().value()) {
                      find = true;
                      values.emplace(value);
                      break;
                    }
                  }
                  if (!find || values.size() > isomor_nodes.size()) {
                    isY = false;
                    break;
                  }
                }
              }
            } else if (type == EQ_VAR) {
              long a_id = it->first;
              long a_elabel = atoi((it++)->second.c_str());
              // cout << a_elabel << "\n"; //test
              long b_id = it->first;
              long b_elabel = atoi((it++)->second.c_str());
              // cout << b_elabel << "\n"; //test
              if (a_elabel == -1) {  // elabel = -1, compare their name
                int a_pos = GED::pos(a_id);
                int b_pos = GED::pos(b_id);
                if (a_pos == -1 || b_pos == -1) {
                  return false;
                }
                // cout << "value:" << g_cans[a_pos][layer[a_pos]].v().value()
                // << "," << g_cans[b_pos][layer[b_pos]].v().value() << "\n";
                // //test
                if (g_cans[a_pos][layer[a_pos]].v().value() !=
                    g_cans[b_pos][layer[b_pos]].v().value()) {
                  if (j == 0) {
                    isX = false;
                  } else {
                    isY = false;
                  }
                  break;
                }
              } else {  // compare their neighbors with given elabels
                int a_pos = GED::pos(a_id);
                int b_pos = GED::pos(b_id);
                // cout << "POS:" << a_pos << "," << b_pos << "\n"; //test
                if (a_pos == -1 || b_pos == -1) {
                  return false;
                }
                // cout << "value:" << g_cans[a_pos][layer[a_pos]].v().value()
                // << "," << g_cans[b_pos][layer[b_pos]].v().value() << "\n";
                // //test
                vector<long> a_dst_id, b_dst_id;
                for (int k = 0; k < 2;
                     k++) {  // find all neighbors with given elabel
                  vector<long>& dst_id = (k == 0) ? a_dst_id : b_dst_id;
                  Node& node = (k == 0) ? g_cans[a_pos][layer[a_pos]]
                                        : g_cans[b_pos][layer[b_pos]];
                  long given_label = (k == 0) ? a_elabel : b_elabel;
                  vector<long>& neighbors = node.neighbors();
                  vector<long>& elabels = node.elabels();
                  for (int t = 0; t < elabels.size(); t++) {
                    if (elabels[t] == given_label) {
                      dst_id.emplace_back(neighbors[t]);
                    }
                  }
                }
                if (interEmpty(a_dst_id, b_dst_id)) {  // if their neighbors'
                                                       // ids are all different,
                                                       // pruning
                  if (j == 0) {
                    isX = false;
                  } else {
                    isY = false;
                  }
                  break;
                }
              }
            } else if (type == EQ_ID) {
              return false;
            }
          }
          // cout << isX << "," << isY << "\n"; //test
          if (!isX) break;
        }
      }
      if (isX && !isY) {
        return false;
      }  // satisfy X but not Y
      if (isP && isX && isY) {
        find_num++;
      }  // find one satisfing ged
    }
    if (_y_type[0] == EQ_ID) {
      return true;
    }  // graph does not have the same pattern
    return (find_num == 0) ? false : true;
  }

#ifdef BOOST_GRAPH
  /* validate GED using mapping results of boost vf2 */
  bool validateGED(Graph& g, vector<map<long, long>>& maps) {
    int find_num = 0;
    // TODO:check for literals?
    if (maps.size() == 0) {  // do not have matches
      if ((_x_type.size() != 0 && _x_type[0] == EQ_ID) ||
          (_y_type.size() != 0 && _y_type[0] == EQ_ID)) {
        // X:eq-id -> Y || X:ANY -> Y:eq-id
        return true;
      }
    } else {  // have matches
      vector<GED_TYPE> types;
      map<long, string>::iterator it;
      for (auto& m : maps) {
        bool isX = true;
        types = _x_type;
        it = _x_matches.begin();
        for (auto& type : types) {  // check X
          if (type == EQ_LET) {
            long vid = it->first;
            string value = (it++)->second;
            long gid = m.at(vid);
            Node& g_node = g.node(gid);
            if (g_node.v().value() != value) {  // value is not equal, break
              isX = false;
              break;
            }
          } else if (type == EQ_VAR) {
            long a_id = it->first;
            long a_elabel = atoi((it++)->second.c_str());
            // cout << a_elabel << "\n"; //test
            long b_id = it->first;
            long b_elabel = atoi((it++)->second.c_str());
            // cout << b_elabel << "\n"; //test
            Node& g_node1 = g.node(m.at(a_id));
            Node& g_node2 = g.node(m.at(b_id));
            if (a_elabel == -1) {  // elabel = -1, compare their name
              if (g_node1.v().value() != g_node2.v().value()) {
                isX = false;
                break;
              }
            } else {  // compare their neighbors with given elabels
              // get node1's neighbors with given elabel
              vector<Node> cans;
              vector<long> eN1 = g_node1.neighbors();
              vector<long> eL1 = g_node1.elabels();
              for (int i = 0; i < eN1.size(); i++) {
                if (eL1[i] == a_elabel) {
                  cans.emplace_back(g.node(eN1[i]));
                }
              }
              int be_cnt = cans.size();
              if (be_cnt == 0) {  // get 0, break
                isX = false;
                break;
              }
              // get node2's neighbors with given elabel
              vector<long> eN2 = g_node2.neighbors();
              vector<long> eL2 = g_node2.elabels();
              for (int i = 0; i < eN2.size(); i++) {
                if (eL2[i] == b_elabel) {
                  cans.emplace_back(g.node(eN2[i]));
                }
              }
              int af_cnt = cans.size();
              if (af_cnt == be_cnt) {  // get 0, break
                isX = false;
                break;
              } else {  // conpare all nodes are the same one
                long id = cans[0].v().id();
                int k = 1;
                for (; k < af_cnt; k++) {
                  if (cans[k].v().id() != id) break;
                }
                if (k != af_cnt) {
                  isX = false;
                  break;
                }
              }
            }
          } else if (type == EQ_ID) {
            return false;
          }
        }
        if (!isX) continue;  // X is not satisfied, needn't to check Y, continue
        bool isY = true;
        types = _y_type;
        it = _y_matches.begin();
        for (auto& type : types) {  // check Y
          if (type == EQ_LET) {
            long vid = it->first;
            std::string value = (it++)->second;
            long gid = m.at(vid);
            // cout << "T: " << gid << "\n"; //test
            Node& g_node = g.node(gid);
            if (value != g_node.v().value()) {  // value is not equal, break
              isY = false;
              break;
            }
          } else if (type == EQ_VAR) {
            long a_id = it->first;
            long a_elabel = atoi((it++)->second.c_str());
            // cout << a_elabel << "\n"; //test
            long b_id = it->first;
            long b_elabel = atoi((it++)->second.c_str());
            // cout << b_elabel << "\n"; //test
            Node& g_node1 = g.node(m.at(a_id));
            Node& g_node2 = g.node(m.at(b_id));
            if (a_elabel == -1) {  // elabel = -1, compare their name
              if (g_node1.v().value() != g_node2.v().value()) {
                isY = false;
                break;
              }
            } else {  // compare their neighbors with given elabels
              // get node1's neighbors with given elabel
              vector<Node> cans;
              vector<long> eN1 = g_node1.neighbors();
              vector<long> eL1 = g_node1.elabels();
              for (int i = 0; i < eN1.size(); i++) {
                if (eL1[i] == a_elabel) {
                  cans.emplace_back(g.node(eN1[i]));
                }
              }
              int be_cnt = cans.size();
              if (be_cnt == 0) {  // get 0, break
                isY = false;
                break;
              }
              // get node2's neighbors with given elabel
              vector<long> eN2 = g_node2.neighbors();
              vector<long> eL2 = g_node2.elabels();
              for (int i = 0; i < eN2.size(); i++) {
                if (eL2[i] == b_elabel) {
                  cans.emplace_back(g.node(eN2[i]));
                }
              }
              int af_cnt = cans.size();
              if (af_cnt == be_cnt) {  // get 0, break
                isY = false;
                break;
              } else {  // conpare all nodes are the same one
                long id = cans[0].v().id();
                int k = 1;
                for (; k < af_cnt; k++) {
                  if (cans[k].v().id() != id) break;
                }
                if (k != af_cnt) {
                  isY = false;
                  break;
                }
              }
            }
          } else if (type == EQ_ID) {
            isY = false;
            break;
          }
        }
        if (isX && isY) {
          find_num++;
        }
        if (isX && !isY) {
          return false;
        }
      }
    }
    // if given ged does not exist in graph, in case "eq-id", return true
    if (_y_type.size() != 0 && _y_type[0] == EQ_ID) return true;
    // cout << "find: " << find_num << "\n"; //test
    return (find_num == 0) ? false : true;
  }
#endif

  /* check whether the format of literal is correct */
  bool checkLiteralFormat() {
    for (int i = 0; i < 2; i++) {
      vector<GED_TYPE>& types = (i == 0) ? _x_type : _y_type;
      map<long, string>::iterator it =
          (i == 0) ? _x_matches.begin() : _y_matches.begin();
      for (auto type : types) {
        vector<long> ids;
        if (type == EQ_LET) {
          ids.emplace_back((it++)->first);
        } else if (type == EQ_VAR) {
          ids.emplace_back((it++)->first);
          ids.emplace_back((it++)->first);
        } else if (type == EQ_ID) {
          ids.emplace_back((it++)->first);
          ids.emplace_back((it++)->first);
        }
        for (vector<long>::iterator vit = ids.begin(); vit != ids.end();
             vit++) {
          if (!nodeExist(*vit)) {
            return false;
          }
        }
      }
    }
    return true;
  }

  void toString(string& str, bool remap) {
    str = str + "%GID:" + gid() + "\n";
    str = str + "%LID" + lid() + "\n";
    patternString(str, remap);
    literalString(str, remap);
  }

  /* return true, if given node has parents */
  bool hasParent(long id) {
    for (auto& node : _nodes) {
      vector<long>& neighbors = node.neighbors();
      for (auto& nebor_id : neighbors)
        if (nebor_id == id) return true;
    }
    return false;
  }

  /* if given node has parents, get its parents */
  void getParent(long id, set<long>& pars) {
    for (auto& node : _nodes) {
      vector<long>& neighbors = node.neighbors();
      for (auto& nebor_id : neighbors)
        if (nebor_id == id) pars.emplace(node.v().id());
    }
  }

  /* get node by given vid, only for existed id */
  inline Node& node(long vid) { return _nodes[_id_map[vid]]; }

 private:
  /* check exist */
  inline bool nodeExist(long vid) {
    return _id_map.find(vid) != _id_map.end() ? true : false;
  }
  /* given vid, return its position */
  inline int pos(long vid) {
    return _id_map.find(vid) != _id_map.end() ? _id_map[vid] : -1;
  }

  /* given node, return its position */
  int pos(Node& node) {
    long id = node.v().id();
    for (int i = 0; i < _nodes.size(); i++) {
      if (_nodes[i].v().id() == id) return i;
    }
    return -1;
  }

  string _gid;
  string _lid;

  /* patterns */
  map<long, long> _id_map;  //(original VID, its position)
  long _cnt;                // vertex number
  vector<Node> _nodes;      // vertices and edges

  /* literals */
  vector<GED_TYPE> _x_type;  // EQ_LET, EQ_VAR or EQ_ID
  vector<GED_TYPE> _y_type;
  map<long, string> _x_matches;
  map<long, string> _y_matches;
};
