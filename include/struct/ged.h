#include <iostream>
#include <map>

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

    void addV(long id, long label, string& value) {
      Vertex v(id, label, value);
      Node node(v);
      _nodes.emplace_back(node);
      _id_map[id] = _cnt++;
    }

    void addE(long src, long label, long dst) {
      Node& node = GED::node(src);
      Node& dnode = GED::node(dst);
      cout << "T1: " << src << "," << label << "," << dst << "\n";
      cout << "T2: " << node.v().id() << "," << dnode.v().id() << "\n";
      node.addNeighbor(&(GED::node(dst).v()), label);
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
        /*for(auto& node:_nodes) {
          Vertex& vertex = node.v();
          string value = vertex.value();
          str = str + "v\t" + boost::lexical_cast<string>(vertex.id()) + "\t"
          + boost::lexical_cast<string>(vertex.label()) + "\t" + vertex.value() + "\n";
        }*/
        for(auto& node2:_nodes) {
          vector<Vertex*>& neighbors = node2.neighbors();
          vector<long>& elabels = node2.elabels();
          int len = neighbors.size();
          for (auto v:neighbors) {
            cout << "DST:" << v->id() << "\n";
          }
          for(int i = 0; i < len; i++) {
            str = str + "e\t" + boost::lexical_cast<string>(node2.v().id()) + "\t{"
            + boost::lexical_cast<string>(elabels[i]) + "}\t" + boost::lexical_cast<string>(neighbors[i]->id()) + "\n";
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

    void toString(string& str) {
      str = str + "%Gid:" + boost::lexical_cast<string>(gid()) + "\n";
      str = str + "%Lid:" + lid() + "\n";
      patternString(str);
      literalString(str);
    }

    void printNeighbors() {
      for (auto node:_nodes) {
        long src = node.v().id();
        vector<long>& elabels = node.elabels();
        vector<Vertex*>& neighbor = node.neighbors();
        int len = neighbor.size();
        for(int i = 0; i < len; i++) {
          cout << src << "," << elabels[i] << "," << neighbor[i]->id() << "\n";
        }
      }
    }

  private:
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

    inline Node& node(long vid) { return _nodes[_id_map[vid]];}
};
