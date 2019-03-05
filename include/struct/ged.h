#include <iostream>
#include <map>
#include <stack>

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

    void addE(long src, long label, long dst) {
      Node& node = GED::node(src);
      Node& dnode = GED::node(dst);
      node.addNeighbor(dst, label);
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

    inline void addX_id(long a_id, long b_id) {
      string label = "-1";
      _x_type.emplace_back(EQ_ID);
      _x_matches[a_id] = label;
      _x_matches[b_id] = label;
    }
    inline void addY_id(long a_id, long b_id) {
      string label = "-1";
      _y_type.emplace_back(EQ_ID);
      _y_matches[a_id] = label;
      _y_matches[b_id] = label;
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
      bool isExist = false;
      vector<vector<Node>> g_cans;
      vector<Node>& g_allNodes = g.allNodes();
      vector<Node>& p_nodes = _nodes;
      int cnt = p_nodes.size();
      for (auto& p_node:p_nodes) {
        vector<Node> g_can;
        for (auto& g_node:g_allNodes) {
          if (g_node.v().label() == p_node.v().label()) {
            g_can.emplace_back(g_node);
          }
        }
        g_cans.emplace_back(g_can);
      }
      vector<int> layer; //record position in graph pattern
      int flag = 0;
      for (int i = 0; i < cnt; i++) {
        int can_num = g_cans[i].size() - 1;
        layer.emplace_back(can_num);
        flag += can_num;
      }
      vector<int> old = layer;
      while (true) {
        bool isP = true;
        for (int i = 0; i < p_nodes.size(); i++) {
          if (p_nodes[i].neighbors().size() == 0) continue;
          // check edge relation
          vector<Node>& g_nodes = g_cans[i];
          vector<long>& p_neighbors = p_nodes[i].neighbors();
          vector<long>& g_neighbors = g_nodes[layer[i]].neighbors();
          if (p_neighbors.size() != g_neighbors.size()) {
            isP = false;
            break;
          }
          vector<long>& p_elabels = p_nodes[i].elabels();
          vector<long>& g_elabels = g_nodes[layer[i]].elabels();
          for(int j = 0; j < p_neighbors.size(); j++) {
            Vertex& p_dst = GED::node(p_neighbors[j]).v();
            Vertex& g_dst_check = GED::node(g_neighbors[j]).v();
            int k = 0;
            for (; k < p_nodes.size(); k++) {
              if (p_nodes[k].v().id() == p_dst.id()) break;
            }
            Vertex& g_dst = g_cans[k][layer[k]].v();
            if (g_dst.id() != g_dst_check.id()) {
              isP = false;
              break;
            }
          }
        }
        if (isP) {
          // check literal
          isExist = true;
          map<long, string>::iterator it = _x_matches.begin();
          for (auto& type:_x_type) {
            map<long, string> check_info;
            if (type == EQ_LET) {
              long vid = it->first;
              string value = (it++)->second;
              check_info[vid] = value;
            } else if (type == EQ_VAR) {
              long a_id = it->first;
              string a_value = (it++)->second;
              check_info[a_id] = a_value;
              long b_id = it->first;
              string b_value = (it++)->second;
              check_info[b_id] = b_value;
            }
            for (map<long, string>::iterator it = check_info.begin(); it != check_info.end(); it++) {
              int pos = GED::pos(it->first);
              Vertex& g_v_check = g_cans[pos][layer[pos]].v();
              if (it->second != g_v_check.value()) {
                isExist = false;
                break;
              }
            }
            if (!isExist) break;
          }
        }
        // update candidates
        for (int i = 0; i < cnt; i++) {
          flag += layer[i];
        }
        if (flag == 0) break;
        int total = layer.size();
        for (int i = 0; i < layer.size(); i++) {
          if (layer[i] == 0) {
            layer[i] = old[i];
          } else {
            layer[i]--;
            break;
          }
        }
      }
      return isExist;
    }

    void toString(string& str) {
      str = str + "%Gid:" + boost::lexical_cast<string>(gid()) + "\n";
      str = str + "%Lid:" + lid() + "\n";
      patternString(str);
      literalString(str);
    }

  private:
    inline Node& node(long vid) { return _nodes[_id_map[vid]];}
    inline int pos(long vid) { return _id_map[vid];}

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
