#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>

#include "../include/checkGED.h"

using namespace std;

void CheckGED::loadGEDs(const string& gedpath) {
  string line;
  int cnt = 0;
  bool isX = true;
  try {
    ifstream fin(gedpath);
    while (getline(fin, line)) {
      if (line.length() < 2) continue;
      if (line[0] == '%' && line[1] == 'G') {  // read gid and lid
        string gid = line.substr(5, line.length() - 5);
        // cout << "G:" << gid << endl;
        getline(fin, line);
        string lid = line.substr(5, line.length() - 5);
        // cout << "L:" << lid << endl;
        GED ged(gid, lid);
        _geds.emplace_back(ged);
        _active.emplace_back(true);
        ++cnt;
      } else if (line[0] == 'v') {  // read v
        stringstream ss(line);
        long id, label;
        string value;
        ss >> value >> id >> label >> value;
        // cout << "ID:" << id << ",LABEL:" << label << ",VALUE:" << value <<
        // "\n";
        _geds[cnt - 1].addV(id, label, value);
      } else if (line[0] == 'e' && line[1] != 'q') {  // read e
        stringstream ss(line);
        long src, dst;
        string label;
        ss >> label >> src >> label >> dst;
        if (label[0] == '{') {
          label = label.substr(1, label.length() - 2);
        }
        // cout << "SRC:" << src << ",LABEL:" << label << ",DST:" << dst <<
        //"\n"; //test
        if (!_geds[cnt - 1].addE(src, boost::lexical_cast<long>(label), dst)) {
          cout << "GED V ID does not exist, in GED " << cnt << " in File "
               << gedpath << "\n";
          exit(1);
        }
        // cout << "OK\n"; //test
      } else if (line[0] == '%' && line[1] == 'X') {  // X?
        isX = true;
      } else if (line[0] == '%' && line[1] == 'Y') {  // Y?
        isX = false;
      } else if (line[0] == 'e' && line[1] == 'q') {  // read literals
        vector<string> details;
        boost::split(details, line, boost::is_any_of("\t"));
        if (details[0][3] == 'l') {
          // format1: eq-let vid key value
          // format2: eq-let vid value
          // Only read in its vid and value because the format of graph is RDF
          // which only has one key-value.
          if (isX) {
            if (details.size() == 4) {
              _geds[cnt - 1].addX_let(boost::lexical_cast<long>(details[1]),
                                      details[3]);
            } else {
              _geds[cnt - 1].addX_let(boost::lexical_cast<long>(details[1]),
                                      details[2]);
            }
          } else {
            if (details.size() == 4) {
              _geds[cnt - 1].addY_let(boost::lexical_cast<long>(details[1]),
                                      details[3]);
            } else {
              _geds[cnt - 1].addY_let(boost::lexical_cast<long>(details[1]),
                                      details[2]);
            }
          }
        } else if (details[0][3] == 'v') {
          if (isX) {
            _geds[cnt - 1].addX_var(
                boost::lexical_cast<long>(details[1]), details[2],
                boost::lexical_cast<long>(details[3]), details[4]);
          } else {
            _geds[cnt - 1].addY_var(
                boost::lexical_cast<long>(details[1]), details[2],
                boost::lexical_cast<long>(details[3]), details[4]);
          }
        } else if (details[0][3] == 'i') {
          // cout << "TEST: " << details[1] << "," << details[2] << "," <<
          // details[3] << "\n"; //test
          if (isX) {
            if (details.size() == 3) {
              string str_id = "id";
              _geds[cnt - 1].addX_id(boost::lexical_cast<long>(details[1]),
                                     str_id,
                                     boost::lexical_cast<long>(details[2]));
            } else {
              _geds[cnt - 1].addX_id(boost::lexical_cast<long>(details[1]),
                                     details[2],
                                     boost::lexical_cast<long>(details[3]));
            }
          } else {
            if (details.size() == 3) {
              string str_id = "id";
              _geds[cnt - 1].addY_id(boost::lexical_cast<long>(details[1]),
                                     str_id,
                                     boost::lexical_cast<long>(details[2]));
            } else {
              _geds[cnt - 1].addY_id(boost::lexical_cast<long>(details[1]),
                                     details[2],
                                     boost::lexical_cast<long>(details[3]));
            }
          }
          // cout << "EOK\n"; //test
        }
      }
    }
  } catch (boost::bad_lexical_cast& e) {
    cout << "GEDReadingError in GED NO." << cnt << ": " << e.what() << "\n";
    exit(1);
  } catch (std::exception& e) {
    cout << e.what() << "\n";
    exit(1);
  }
}

void CheckGED::loadState(const string& stafile) {
  string line;
  int cnt = 0;
  try {
    ifstream fin(stafile);
    while (getline(fin, line)) {
      if (line == "The results:") {
        while (getline(fin, line)) {
          // cout << "LINE: " << line << "\n"; //test
          vector<string> details;
          boost::split(details, line, boost::is_any_of("\t"));
          for (auto& d : details) {
            if (d == "") continue;
            // cout << d << "\n"; //test
            _active[cnt++] = boost::lexical_cast<bool>(d);
          }
        }
        break;
      }
    }
  } catch (boost::bad_lexical_cast& e) {
    cout << "GEDStateReadingError: " << e.what() << "\n";
    exit(1);
  } catch (std::exception& e) {
    cout << e.what() << "\n";
    exit(1);
  }
}

void CheckGED::printGEDs() {
  cout << "Total:" << _geds.size() << "\n\n";
  for (auto& ged : _geds) {
    string str = "";
    ged.toString(str, false);
    cout << str << "\n";
  }
}

/* rewrite GEDs (vertexes start from 0 and are continous) */
void CheckGED::reWriteGEDs(const string& path) {
  try {
    std::ofstream fout(path);
    for (auto& ged : _geds) {
      string str = "";
      ged.toString(str, true);
      fout << "##############\n" << str;
    }
    fout.close();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    exit(1);
  }
}

#ifdef BOOST_GRAPH

void CheckGED::loadMapping(const string& mapfile) {
  string line;
  try {
    ifstream fin(mapfile);
    while (getline(fin, line)) {
      if (line[0] == '#') {
        vector<map<long, long>> map_v;
        while (getline(fin, line)) {
          if (line[0] == '$') break;
          map<long, long> tmp;
          vector<string> details;
          boost::split(details, line, boost::is_any_of("\t"));
          for (auto& d : details) {
            if (d == "") continue;
            d = d.substr(1, d.length() - 2);
            vector<string> pair;
            boost::split(pair, d, boost::is_any_of(","));
            tmp[boost::lexical_cast<long>(pair[0])] =
                boost::lexical_cast<long>(pair[1]);
          }
          map_v.emplace_back(tmp);
        }
        _maps.emplace_back(map_v);
      }
    }
  } catch (boost::bad_lexical_cast& e) {
    cout << "GEDMappingReadingError: " << e.what() << "\n";
    exit(1);
  } catch (std::exception& e) {
    cout << e.what() << "\n";
    exit(1);
  }
}

void CheckGED::convert2BG() {
  // convert graph
  Graph& g = _graph;
  vector<GED>& geds = _geds;
  // Add vertices...
  vector<Node>& nodes = g.allNodes();
  for (auto& n : nodes) {  // start from id 0
    add_vertex(vertex_property(n.v().label()), _boost_graph);
  }
  //... and edges
  for (auto& n : nodes) {
    vector<long>& eL = n.elabels();
    vector<long>& eN = n.neighbors();
    // cout << "ID:" << n.v().id() << "\n"; //test
    if (eL.size() == 0) continue;
    if (eL.size() == 1) {
      add_edge(n.v().id(), eN[0], edge_property(eL[0]), _boost_graph);
    } else {
      vector<pair<long, long>> edges;
      for (int i = 0; i < eL.size(); i++) {
        edges.emplace_back(std::make_pair(eN[i], eL[i]));
      }
      sort(edges.begin(), edges.end(), pairComp);  // sort by dst id
      int head = 0;
      int tail = 1;
      int edge_num = edges.size();
      // cout << "B:" << edge_num << "\n"; //test
      for (; tail <= edge_num; tail++) {
        if (tail == edge_num || edges[head].first != edges[tail].first) {
          set<long> edge_pro;
          for (; head < tail; head++) {
            // cout << "A:" << eL[head] << "," << eN[head] << "\n"; //test
            edge_pro.emplace(eL[head]);
          }
          add_edge(n.v().id(), eN[head - 1], edge_property(edge_pro),
                   _boost_graph);
        }
      }
    }
  }
  // convert GEDs
  for (int i = 0; i < geds.size(); i++) {
    if (_connected[i]) {  // connected GED
      // Create graph1
      graph_type pattern;
      // Add vertices...
      vector<Node>& nodes = geds[i].pattern();
      for (auto& n : nodes) {  // start from id 0
        add_vertex(vertex_property(n.v().label()), pattern);
      }
      //... and edges
      for (auto& n : nodes) {
        vector<long>& eL = n.elabels();
        vector<long>& eN = n.neighbors();
        for (int i = 0; i < eL.size(); i++) {
          add_edge(n.v().id(), eN[i], edge_property(eL[i]), pattern);
        }
      }
      vector<graph_type> pattern_v;
      pattern_v.emplace_back(pattern);
      _boost_patterns.emplace_back(pattern_v);
    } else {  // unconnected GED
      vector<map<long, long>>& map_v = _unconnected_maps[i];
      vector<graph_type> pattern_v;
      for (auto& id_map : map_v) {
        graph_type pattern;
        map<long, long> tmp_map;
        for (int pos = 0; pos < id_map.size(); pos++) {
          Node& n = geds[i].node(id_map[pos]);
          tmp_map[n.v().id()] = (long)pos;
          // cout << "ID: " << pos << "," << n.v().label() << "\n"; //test
          add_vertex(vertex_property(n.v().label()), pattern);
        }
        for (int pos = 0; pos < id_map.size(); pos++) {
          Node& n = geds[i].node(id_map[pos]);
          vector<long>& eL = n.elabels();
          vector<long>& eN = n.neighbors();
          for (int i = 0; i < eL.size(); i++) {
            // cout << "E: " << pos << "," << tmp_map[eN[i]] << "," << eL[i] <<
            // "\n"; //test
            add_edge((long)pos, tmp_map[eN[i]], edge_property(eL[i]), pattern);
          }
        }
        pattern_v.emplace_back(pattern);
      }
      _boost_patterns.emplace_back(pattern_v);
    }
  }
}

void CheckGED::boost_filter() {
  for (int i = 0; i < _geds.size(); i++) {
    vector<Node>& nodes = _geds[i].pattern();
    // Filter independent node situation
    for (auto& node : nodes) {
      if (node.neighbors().size() == 0 && !_geds[i].hasParent(node.v().id())) {
        _active[i] = false;
        // cout << "F\n"; //test
        break;
      }
    }
    // Filter GEDs with wrong literal format
    if (_geds[i].hasFormatError()) {
      _active[i] = false;
    }
  }
  // Filter GED who has unconnected patterns consisting with more than 2
  // connected patterns.
  for (int i = 0; i < _unconnected_maps.size(); i++) {
    if (_unconnected_maps[i].size() > 2) {
      _active[i] = false;
    }
  }
}

void CheckGED::boost_vf2() {
  vector<vector<graph_type>>& ged_bps = _boost_patterns;
  for (int i = 0; i < ged_bps.size(); i++) {
    vector<map<long, long>> id_maps;
    map<int, int> pos_map;
    cout << "vf2 on No." << i + 1 << " GED\n";
    if (_active[i] != false) {  // ignore filtered patterns
      // only support connected GED
      // and unconnected GED cosisting with 2 connected pattern
      for (int j = 0; j < ged_bps[i].size() && j < 2; j++) {
        graph_type& bp = ged_bps[i][j];
        // Create the vertex binary predicate
        vertex_comp_t vertex_comp = make_property_map_equivalent(
            get(boost::vertex_name, bp), get(boost::vertex_name, _boost_graph));
        // Create the vertex binary predicate
        edge_comp_t edge_comp = make_property_map_equivalent(
            get(boost::edge_name, bp), get(boost::edge_name, _boost_graph));
        // Create callback
        my_call_back<graph_type, graph_type> callback(bp, _boost_graph);
        // Print out all subgraph isomorphism mappings between graph1 and
        // graph2.
        // Function vertex_order_by_mult is used to compute the order of
        // vertices of graph1. This is the order in which the vertices are
        // examined during the matching process.
        bool flag = boost::vf2_subgraph_iso(
            bp, FilteredGraph(_boost_graph, FilterSelfEdges{&_boost_graph}),
            std::ref(callback), vertex_order_by_mult(bp),
            edges_equivalent(edge_comp).vertices_equivalent(vertex_comp));
        // get vector from callback
        auto set_of_vertex_iso_map = callback.get_setvmap();
        // store mapping
        if (ged_bps[i].size() == 1) {
          // pattern is connected
          for (auto& set_of_v : set_of_vertex_iso_map) {
            map<long, long> id_map;
            for (auto& v : set_of_v) {
              id_map[v.first] = v.second;
            }
            id_maps.emplace_back(id_map);
          }
        } else {
          // pattern is unconnected
          map<long, long>& tmp_map = _unconnected_maps[i][j];
          vector<map<long, long>> tmp_id_map;
          for (auto& set_of_v : set_of_vertex_iso_map) {
            map<long, long> id_map;
            for (auto& v : set_of_v) {
              id_map[tmp_map[v.first]] = v.second;
            }
            tmp_id_map.emplace_back(id_map);
          }
          int old_size = id_maps.size();
          int new_size = tmp_id_map.size();
          // cout << old_size << "," << new_size << "\n"; //test
          if (old_size != 0) {
            vector<map<long, long>> tmp = id_maps;
            // only store top "top_k"
            int top_k = 10000;
            for (int k = 1; k < new_size && k * old_size < top_k; k++) {
              id_maps.insert(id_maps.end(), tmp.begin(), tmp.end());
            }
            int old_cnt = 0;
            int new_cnt = 0;
            while (old_cnt < id_maps.size()) {
              for (int k = 0; k < old_size; k++, old_cnt++) {
                map<long, long>& tmp = tmp_id_map[new_cnt];
                for (auto& it : tmp) {
                  id_maps[old_cnt][it.first] = it.second;
                }
              }
              new_cnt++;
            }
          } else {
            id_maps = tmp_id_map;
          }
        }
      }
      if (ged_bps[i].size() > 1) {
        // remove homomorphism situation
        for (auto it = id_maps.begin(); it != id_maps.end();) {
          set<long> id_set;
          for (auto& m_it : (*it)) {
            id_set.emplace(m_it.second);
          }
          if (id_set.size() != (*it).size()) {
            id_maps.erase(it);
          } else {
            it++;
          }
        }
      }
    }
    _maps.emplace_back(id_maps);
  }
}

void CheckGED::boost_writeMapping(const string& mapfile) {
  try {
    ofstream fout(mapfile);
    int id = 0;
    for (auto& set_of_v : _maps) {
      fout << "#" << ++id << ":\n";
      for (auto& vs : set_of_v) {
        for (auto& v : vs) {
          fout << "(" << v.first << "," << v.second << ")"
               << "\t";
        }
        fout << "\n";
      }
      fout << "$"
           << "\n";
    }
    fout.close();
  } catch (std::exception& e) {
    cout << e.what() << endl;
    exit(1);
  }
}

void CheckGED::boost_validation() {
  for (int i = 0; i < _geds.size(); i++) {
    cout << "Checking NO." << (i + 1) << " GED";
    if (_active[i] = true) {
      if (!_geds[i].validateGED(_graph, _maps[i])) {
        _active[i] = false;
      }
    }
    cout << "\t" << _active[i] << "\n";
  }
  printRes();
}

#endif

void CheckGED::validation() {
  clock_t start, end, tmp;
  tmp = start = clock();
  for (int i = 0; i < _geds.size(); i++) {
    cout << "Checking NO." << (i + 1) << " GED";
    if (_active[i] = true) {
      if (!_geds[i].validateGED(_graph, _maps[i])) {
        _active[i] = false;
      }
    }
    cout << "\t" << _active[i] << "\n";

    if ((i + 1) % 200 == 0) {
      end = clock();
      cout << "Cost: " << (double)(end - tmp) / CLOCKS_PER_SEC << " s\n";
      tmp = end;
    }
  }
  end = clock();
  cout << "Total cost: " << (double)(end - start) / CLOCKS_PER_SEC << " s\n";
  printRes();
}

void CheckGED::writeValidatedGEDs(const string& gedpath) {
  ofstream fout(gedpath);
  int cnt = _active.size();
  for (int i = 0; i < cnt; i++) {
    if (_active[i]) {
      string res = "";
      _geds[i].toString(res, false);
      fout << "##############\n" << res;
    }
  }
  fout.close();
}

void CheckGED::delivery(const string& gedpath, int frag_num) {
  string line;
  int ged_cnt = 0;
  try {
    ifstream fin(gedpath);
    vector<string> strList;
    while (getline(fin, line)) {
      if (line[0] == '%' && line[1] == 'G') {
        ged_cnt++;
      }
      strList.emplace_back(line);
    }
    // cout << frag_num << "\n";
    // cout << ged_cnt << "\n";
    const int volume = ((int)ged_cnt / frag_num) + 1;
    // cout << volume << "\n";

    int frag_id = 0;
    int i = 0;
    int total = strList.size();
    string last = "";
    for (; frag_id < frag_num; frag_id++) {
      int now_cnt = 0;
      string resfile = gedpath + boost::lexical_cast<string>(frag_id);
      ofstream fout(resfile);
      if (last != "") fout << last << "\n";
      for (; i < total; i++) {
        if (strList[i][0] == '%' && strList[i][1] == 'G') {
          now_cnt++;
          if (now_cnt > volume) {
            break;
          }
        }
        fout << strList[i] << "\n";
      }
      fout.close();
    }
  } catch (std::exception& e) {
    cout << e.what();
    exit(1);
  }
}

void CheckGED::classify() {
  for (int i = 0; i < _geds.size(); i++) {
    vector<Node>& nodes = _geds[i].pattern();
    set<long> all_ids;
    for (auto& n : nodes) all_ids.emplace(n.v().id());
    vector<map<long, long>> map_v;
    while (all_ids.size() != 0) {
      set<long> add_ids;
      add_ids.emplace(*(all_ids.begin()));
      auto iter = add_ids.begin();
      while (iter != add_ids.end()) {
        long id = *iter;
        vector<long>& neighbors = _geds[i].node(id).neighbors();
        for (auto& nebor : neighbors) {
          add_ids.emplace(nebor);
        }
        set<long> parents;
        _geds[i].getParent(id, parents);
        for (auto& par : parents) {
          add_ids.emplace(par);
        }
        iter++;
      }
      if (add_ids.size() == nodes.size()) break;  // connected pattern
      // unconnected, separate pattern into many connected patterns
      map<long, long> id_map;
      int cnt = 0;
      for (auto iter = add_ids.begin(); iter != add_ids.end(); iter++) {
        long id = *iter;
        id_map[cnt++] = id;
        all_ids.erase(id);
      }
      map_v.emplace_back(id_map);
    }

    if (map_v.size() == 0) {
      _connected.emplace_back(true);
    } else {
      _connected.emplace_back(false);
    }
    _unconnected_maps.emplace_back(map_v);
  }
}

int main(int argc, char** argv) {
  int mode = 0;
  int frag_num = 1;
  string filename = "";
  string gedpath = "";
  if (argc < 2) {
    cout << "Missing Parameters!" << endl;
    exit(1);
  }
  mode = atoi(argv[1]);
  if (mode == 1) {
    // validate GEDs  ./checkGED 1 $Graph_file $GED_path
    if (argc < 4) {
      cout << "Missing Parameters!" << endl;
      exit(1);
    }
    filename = argv[2];
    gedpath = argv[3];
    string resfile = gedpath + ".vali";

    CheckGED cg;

    cg.loadGraph(filename);
    cg.printGraph();

    cg.loadGEDs(gedpath);
    cg.printGEDs();

    // judge GED is connected or unconnected
    cg.classify();

#ifdef BOOST_GRAPH
    /* Graph's and GEDs' vertices must start from id 0 and continous. */
    string mapfile = gedpath + ".map";
    // convert graph and geds to boost graph format
    cg.convert2BG();
    // filter unexpected GEDs
    cg.boost_filter();
    // use boost vf2 to produce mapping from GEDs to Graph
    cg.boost_vf2();
    // write mapping from GEDs to Graph
    cg.boost_writeMapping(mapfile);
    // validate literals
    cg.boost_validation();
#else
    // validate
    cg.validation();
#endif

    // write results
    cg.writeValidatedGEDs(resfile);
  } else if (mode == 2) {
    // cut GEDs ./checkGED 2 $GED_path NUM
    if (argc < 4) {
      cout << "Missing Parameters!" << endl;
      exit(1);
    }
    gedpath = argv[2];

    int frag_num = atoi(argv[3]);
    CheckGED cg;
    cg.delivery(gedpath, frag_num);
  } else if (mode == 3) {
    // rewrite Graph and GEDs ./checkGED 3 $Graph_file $GEDs_path
    if (argc < 4) {
      cout << "Missing Parameters!" << endl;
      exit(1);
    }
    filename = argv[2];
    gedpath = argv[3];

    CheckGED cg;
    Graph& g = cg.graph();
    cg.loadGraph(filename);
    // cg.printGraph();
    // rewrite graph, make vertices start from id 0 and continous.
    string repath = filename + ".remap";
    g.rewriteGraph(repath);

    cg.loadGEDs(gedpath);
    // cg.printGEDs();
    // rewrite GEDs, make vertices start from id 0 and continous.
    repath = gedpath + ".remap";
    cg.reWriteGEDs(repath);
  }
#ifdef BOOST_GRAPH
  else if (mode == 4) {
    // select right GEDs whose number of isomorphism match is more than k
    if (argc < 4) {
      cout << "Missing Parameters!" << endl;
      exit(1);
    }
    gedpath = argv[2];
    int k = atoi(argv[3]);
    CheckGED cg;
    cg.loadGEDs(gedpath);
    const string stafile = gedpath + ".log";
    cg.loadState(stafile);
    const string mapfile = gedpath + ".map";
    cg.loadMapping(mapfile);
    // unactivate GED whose number of isomorphism is less than k
    cg.filter_k(k);
    const string resfile = gedpath + ".vali_m" + argv[3];
    cg.writeValidatedGEDs(resfile);
  }
#endif

  return 0;
}
