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

    void addE(long src, long dst, long label) {
      Node& node = GED::node(src);
      node.addNeighbor(&(GED::node(dst).v()), label);
    }

    inline void setX_type(GED_TYPE type) {_x_type.emplace_back(type);}
    inline void setY_type(GED_TYPE type) {_y_type.emplace_back(type);}

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

    inline vector<Node>& pattern() {return _nodes;}
    inline vector<GED_TYPE>& xtype() {return _x_type;}
    inline vector<GED_TYPE>& ytype() {return _y_type;}
    inline map<long, string>& xmatches() {return _x_matches;}
    inline map<long, string>& ymatches() {return _y_matches;}

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
