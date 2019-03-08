#include <iostream>
#include <map>
#include <stack>
#include <set>

#include "./graph.h"
#include "../util.h"

using namespace std;

class GED {
  public:
		GED(){}
		~GED(){}

    GED(long gid, string& lid) {
      _gid = gid;
      _lid = lid;
      _cnt = 0;
    }

    GED(long gid, string& lid, map<long, long>& id_map, long cnt, vector<Node>& nodes,
        vector<GED_TYPE>& x_type, vector<GED_TYPE>& y_type,
        map<long, string>& x_matches, map<long, string>& y_matches) {
      _gid = gid;
      _lid = lid;
      _id_map = id_map;
      _cnt = cnt;
      _nodes = nodes;
      _x_type = x_type;
      _y_type = y_type;
      _x_matches = x_matches;
      _y_matches = y_matches;
    }

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

    inline vector<Node>& pattern() {return _nodes;}
    inline vector<GED_TYPE>& xtype() {return _x_type;}
    inline vector<GED_TYPE>& ytype() {return _y_type;}
    inline map<long, string>& xmatches() {return _x_matches;}
    inline map<long, string>& ymatches() {return _y_matches;}

    inline long gid() {return _gid;}
    inline string& lid() {return _lid;}

    void patternString(string& str) {
      try {
        for(auto& node:_nodes) {
          Vertex& vertex = node.v();
          string value = vertex.value();
          str = str + "v\t" + boost::lexical_cast<string>(vertex.id()) + "\t"
          + boost::lexical_cast<string>(vertex.label()) + "\t" + vertex.value() + "\n";
        }
        for(auto& node:_nodes) {
          vector<long>& neighbors = node.neighbors();
          vector<long>& elabels = node.elabels();
          int len = neighbors.size();
          for(int i = 0; i < len; i++) {
            Vertex& dst = GED::node(neighbors[i]).v();
            str = str + "e\t" + boost::lexical_cast<string>(node.v().id()) + "\t{"
            + boost::lexical_cast<string>(elabels[i]) + "}\t" + boost::lexical_cast<string>(dst.id()) + "\n";
          }
        }
      } catch(boost::bad_lexical_cast& e) {
        cout << e.what() << endl;
        exit(0);
      }
    }

    void literalString(string& str) {
      str += "%X\n";
      map<long, string>::iterator it = _x_matches.begin();
      try {
        for (auto type:_x_type) {
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
            str += boost::lexical_cast<string>((it++)->first) + "\n";
          }
        }
        str += "%Y\n";
        it = _y_matches.begin();
        for (auto type:_y_type) {
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
            str += boost::lexical_cast<string>((it++)->first) + "\n";
          }
        }
      } catch(boost::bad_lexical_cast& e) {
        cout << e.what() << endl;
        exit(0);
      }
    }

    bool existGED(Graph& g) {
      vector<vector<Node>> g_cans;
      vector<Node>& g_allNodes = g.allNodes();
      vector<Node>& p_nodes = _nodes;
      const int cnt = p_nodes.size();
      // obetain all possible candidates in graph
      // CAUTION: value of vertex is not considered. (all are ANY)
      for (auto& p_node:p_nodes) {
        vector<Node> g_can;
        for (auto& g_node:g_allNodes) {
          if (g_node.v().label() == p_node.v().label()) {
            g_can.emplace_back(g_node);
          }
        }
        g_cans.emplace_back(g_can);
      }
      ///test
      /*for (auto& g_can:g_cans) {
        for (auto& g:g_can) {
          cout << g.v().id() << "," << g.v().label() << "," << g.v().value() << "\t";
        }
        cout << "\n";
      }*/

      vector<int> layer; //record position in graph pattern
      for (int i = 0; i < cnt; i++) {
        int can_num = g_cans[i].size() - 1;
        layer.emplace_back(can_num);
        //cout << can_num << "\t"; //test
      }
      //cout << "\n"; //test
      vector<int> old = layer;
      layer[0]++;

      vector<Node> isomor_nodes;
      set<string> values;
      int find_num = 0; // the number of finding this ged
      int t_cnt = 0;
      bool iterate = true;
      while (iterate) {
        //cout << "T_num:" << t_cnt++ << "\n"; //test
        // update candidates
        for (int i = 0; i < layer.size(); i++) {
          if (layer[i] == 0) {
            layer[i] = old[i];
          } else {
            layer[i]--;
            break;
          }
        }
        //if all posibilities are iterated, then break
        int flag = 0;
        for (int i = 0; i < cnt; i++) {
          //cout << "N:" << layer[i] << ","; //test
          flag += layer[i];
        }
        //cout << flag << "\n"; //test
        if (flag == 0) { iterate = false;}
        //except homomorphism
        set<long> g_v;
        for (int i = 0; i < cnt; i++) {
          g_v.emplace(g_cans[i][layer[i]].v().id());
        }
        if (g_v.size() != cnt) continue;

        //check validation of pattern
        bool isP = true;
        for (int i = 0; i < p_nodes.size(); i++) {
          if (p_nodes[i].neighbors().size() == 0) { //only check node with neighbors in pattern
            continue;
          }
          // check edge relation
          bool findNode = true;
          Node& g_node = g_cans[i][layer[i]];
          vector<long>& p_neighbors = p_nodes[i].neighbors();
          vector<long>& g_neighbors = g_node.neighbors();
          if (p_neighbors.size() > g_neighbors.size()) {
            //if num of p_node's neighbors > num of g_node's neighbors, pruning
            isP = false;
            break;
          }
          vector<long>& p_elabels = p_nodes[i].elabels();
          vector<long>& g_elabels = g_node.elabels();
          for(int j = 0; j < p_neighbors.size(); j++) {
            int k;
            bool sameELabel = false;
            for (k = 0; k < g_elabels.size(); k++) {
              //cout << "Elabel:" << p_elabels[j] << "," << g_elabels[k] << "\n"; //test
              if (g_elabels[k] == p_elabels[j]) { //elabel is equal
                sameELabel = true;
                Node& p_dst_node = GED::node(p_neighbors[j]);
                int pos = GED::pos(p_dst_node);
                Node& g_dst_node = g_cans[pos][layer[pos]];
                //test
                /*cout << "P:" << p_dst_node.v().id() << "," << p_dst_node.v().label() << "," << p_elabels[j] << "\n";
                cout << "G:" << g_dst_node.v().id() << "," << g_dst_node.v().label() << ",{";
                vector<long> g_test;
                g_node.elabel(g_dst_node, g_test);
                for (auto tt:g_test) {
                  cout << tt << ",";
                }
                cout << "}\n";
                cout << "Same:" << g_neighbors[k] << "," << g_dst_node.v().id() << "\n";
                cout << "NVlabel:" << p_dst_node.v().label() << "," << g_dst_node.v().label() << "\n";*/
                if (g_neighbors[k] == g_dst_node.v().id() && p_dst_node.v().label() == g_dst_node.v().label()) {
                  //judge whether structures are same
                  //judge whether neighbor's label is equal
                  break;
                }
              }
            }
            if (!sameELabel) { //present g_node don't have satisfying neighbors with given elabel, so remove it.
              g_cans[i].erase(g_cans[i].begin() + layer[i]);
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
        }
        //check validation of literals
        bool isX = true;
        bool isY = true;
        if (isP) {
          //cout << "Pattern is Correct!\n"; //test
          // check literal
          for (int j = 0; j < 2; j++) {
            vector<GED_TYPE>& types = (j == 0) ? _x_type : _y_type;
            map<long, string>::iterator it = (j == 0) ? _x_matches.begin() : _y_matches.begin();
            for (auto& type:types) {
              if (type == EQ_LET) {
                long vid = it->first;
                string value = (it++)->second;
                int pos = GED::pos(vid);
                if (pos == -1) { return false;}
                //cout << "value:" << value << "," << g_cans[pos][layer[pos]].v().value() << "\n"; //test
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
                    for (auto& node:isomor_nodes) {
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
                long a_elabel = boost::lexical_cast<long>((it++)->second);
                long b_id = it->first;
                long b_elabel = boost::lexical_cast<long>((it++)->second);
                if (a_elabel == -1) { //elabel = -1, compare their name
                  int a_pos = GED::pos(a_id);
                  int b_pos = GED::pos(b_id);
                  if (a_pos == -1 || b_pos == -1) { return false;}
                  //cout << "value:" << g_cans[a_pos][layer[a_pos]].v().value() << "," << g_cans[b_pos][layer[b_pos]].v().value() << "\n"; //test
                  if (g_cans[a_pos][layer[a_pos]].v().value() != g_cans[b_pos][layer[b_pos]].v().value()) {
                    if (j == 0) {
                      isX = false;
                    } else {
                      isY = false;
                    }
                    break;
                  }
                } else { //compare their neighbors with given elabels
                  int a_pos = GED::pos(a_id);
                  int b_pos = GED::pos(b_id);
                  if (a_pos == -1 || b_pos == -1) { return false;}
                  vector<long> a_dst_id, b_dst_id;
                  for (int k = 0; k < 2; k++) { //find all neighbors with given elabel
                    vector<long>& dst_id = (k == 0) ? a_dst_id : b_dst_id;
                    Node& node = (k == 0) ? g_cans[a_pos][layer[a_pos]] : g_cans[b_pos][layer[b_pos]];
                    long given_label = (k == 0) ? a_elabel : b_elabel;
                    vector<long>& neighbors = node.neighbors();
                    vector<long>& elabels = node.elabels();
                    for (int t = 0; t < elabels.size(); t++) {
                      if (elabels[t] == given_label) { dst_id.emplace_back(neighbors[t]);}
                    }
                  }
                  if (interEmpty(a_dst_id, b_dst_id)) { //if their neighbors' ids are all different, pruning
                    if (j == 0) {
                      isX = false;
                    } else {
                      isY = false;
                    }
                    break;
                  }
                }
              }
            }
            //cout << isX << "," << isY << "\n"; //test
            if (!isX) break;
          }
        }
        if (isX && !isY) { return false;} //satisfy X but not Y
        if (isP && isX && isY) { find_num++;} //find one satisfing ged
      }
      return (find_num == 0) ? false : true;
    }

    void toString(string& str) {
      str = str + "%Gid:" + boost::lexical_cast<string>(gid()) + "\n";
      str = str + "%Lid:" + lid() + "\n";
      patternString(str);
      literalString(str);
    }

  private:
    inline Node& node(long vid) { return _nodes[_id_map[vid]];}
    /* check exist */
    inline bool nodeExist(long vid) { return _id_map.find(vid) != _id_map.end() ? true : false;}
    /* given vid, return its position */
    inline int pos(long vid) { return _id_map.find(vid) != _id_map.end() ? _id_map[vid] : -1;}

    /* given node, return its position */
    int pos(Node& node) {
      long id = node.v().id();
      for (int i = 0; i < _nodes.size(); i++) {
        if (_nodes[i].v().id() == id) return i;
      }
      return -1;
    }

    long _gid;
    string _lid;

    /* patterns */
    map<long, long> _id_map; //(original VID, its position)
    long _cnt; //vertex number
    vector<Node> _nodes;  //vertices and edges

    /* literals */
    vector<GED_TYPE> _x_type; //EQ_LET, EQ_VAR or EQ_ID
    vector<GED_TYPE> _y_type;
    map<long, string> _x_matches;
    map<long, string> _y_matches;
};
