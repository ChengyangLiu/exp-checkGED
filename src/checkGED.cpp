#include <iostream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "../include/checkGED.h"

using namespace std;

void CheckGED::loadGEDs(const string& gedpath) {
	string line;
	int cnt = 0;
	bool isX = true;
	try{
		ifstream fin(gedpath);
		while(getline(fin, line)) {
			if (line.length() < 2) continue;
			if (line[0] == '%' && line[1] == 'G') { //read gid and lid
				string gid = line.substr(5, line.length() - 5);
				//cout << "G:" << gid << endl;
				getline(fin, line);
				string lid = line.substr(5, line.length() - 5);
				//cout << "L:" << lid << endl;
				GED ged(gid, lid);
				_geds.emplace_back(ged);
				_active.emplace_back(true);
				++cnt;
			} else if (line[0] == 'v') { //read v
				stringstream ss(line);
				long id, label;
				string value;
				ss >> value >> id >> label >> value;
				//cout << "ID:" << id << ",LABEL:" << label << ",VALUE:" << value << "\n";
				_geds[cnt-1].addV(id, label, value);
			} else if (line[0] == 'e' && line[1] != 'q') { //read e
				stringstream ss(line);
				long src, dst;
				string label;
				ss >> label >> src >> label >> dst;
				if (label[0] == '{') {
					label = label.substr(1, label.length()-2);
				}
				//cout << "SRC:" << src << ",LABEL:" << label << ",DST:" << dst << "\n";
				if (!_geds[cnt-1].addE(src, boost::lexical_cast<long>(label), dst)) {
					cout << "GED V ID does not exist, in GED " << cnt << " in File " << gedpath << "\n";
					exit(0);
				}
			} else if (line[0] == '%' && line[1] == 'X') { //X?
				isX = true;
			} else if (line[0] == '%' && line[1] == 'Y') { //Y?
				isX = false;
			} else if (line[0] == 'e' && line[1] == 'q') { //read literals
				vector<string> details;
				boost::split(details, line, boost::is_any_of("\t"));
				if (details[0][3] == 'l') {
					//format1: eq-let vid key value
					//format2: eq-let vid value
					//Only read in its vid and value because the format of graph is RDF which only has one key-value.
					if (isX) {
						if (details.size() == 4) {
							_geds[cnt-1].addX_let(boost::lexical_cast<long>(details[1]), details[3]);
						} else {
							_geds[cnt-1].addX_let(boost::lexical_cast<long>(details[1]), details[2]);
						}
					} else {
						if (details.size() == 4) {
							_geds[cnt-1].addY_let(boost::lexical_cast<long>(details[1]), details[3]);
						} else {
							_geds[cnt-1].addY_let(boost::lexical_cast<long>(details[1]), details[2]);
						}
					}
				} else if (details[0][3] == 'v') {
					if (isX) {
						_geds[cnt-1].addX_var(boost::lexical_cast<long>(details[1]), details[2], boost::lexical_cast<long>(details[3]), details[4]);
					} else {
						_geds[cnt-1].addY_var(boost::lexical_cast<long>(details[1]), details[2], boost::lexical_cast<long>(details[3]), details[4]);
					}
				} else if (details[0][3] == 'i') {
					if (isX) {
						_geds[cnt-1].addX_id(boost::lexical_cast<long>(details[1]), details[2], boost::lexical_cast<long>(details[3]));
					} else {
						_geds[cnt-1].addY_id(boost::lexical_cast<long>(details[1]), details[2], boost::lexical_cast<long>(details[3]));
					}
				}
			}
		}
	} catch(boost::bad_lexical_cast& e) {
		cout << "GEDReadingError in GED NO." << (cnt-1) << ": " << e.what() << "\n";
		exit(0);
	} catch (std::exception& e) {
		cout << e.what() << "\n";
		exit(0);
	}
}

void CheckGED::printGEDs() {
	cout << "Total:" << _geds.size() << "\n\n";
	for (auto& ged:_geds) {
		string str = "";
		ged.toString(str);
		cout << str << "\n";
	}
}

#ifdef BOOST_GRAPH

/* CAUTION: the graph's id must start from 0 and be continous! */
void CheckGED::convert2BG(Graph& g, vector<GED>& geds) {
	// Add vertices...
	vector<Node>& nodes = g.allNodes();
	for (auto& n:nodes) { // start from id 0
		add_vertex(vertex_property(n.v().label()), _boost_graph);
	}
	//... and edges
	for (auto& n:nodes) {
		vector<long>& eL = n.elabels();
		vector<long>& eN = n.neighbors();
		for (int i = 0; i < eL.size(); i++) {
			add_edge(n.v().id(), eN[i], edge_property(eL[i]), _boost_graph);
		}
	}

	for (auto& ged:geds) {
		// Create graph1
		graph_type pattern;
		// Add vertices...
		vector<Node>& nodes = ged.pattern();
		for (auto& n:nodes) { // start from id 0
			add_vertex(vertex_property(n.v().label()), pattern);
		}
		//... and edges
		for (auto& n:nodes) {
			vector<long>& eL = n.elabels();
			vector<long>& eN = n.neighbors();
			for (int i = 0; i < eL.size(); i++) {
				add_edge(n.v().id(), eN[i], edge_property(eL[i]), pattern);
			}
		}
		_boost_patterns.emplace_back(pattern);
	}
}

void CheckGED::boost_vf2() {
	vector<graph_type>& bps = _boost_patterns;
	int cnt = 0;
	for (auto& bp:bps) {
		cout << "No. " << ++cnt << ":\n";
		// Create the vertex binary predicate
		vertex_comp_t vertex_comp = make_property_map_equivalent(get(boost::vertex_name, bp), get(boost::vertex_name, _boost_graph));
	  // Create the vertex binary predicate
	  edge_comp_t edge_comp = make_property_map_equivalent(get(boost::edge_name, bp), get(boost::edge_name, _boost_graph));
		// Create callback
	  boost::vf2_print_callback<graph_type, graph_type> callback(bp, _boost_graph);
	  // Print out all subgraph isomorphism mappings between graph1 and graph2.
	  // Function vertex_order_by_mult is used to compute the order of
	  // vertices of graph1. This is the order in which the vertices are examined
	  // during the matching process.
	  boost::vf2_subgraph_iso(bp, _boost_graph, callback, vertex_order_by_mult(bp), edges_equivalent(edge_comp).vertices_equivalent(vertex_comp));
		cout << "\n";
	}
}
#endif

void CheckGED::validation() {
	clock_t start, end, tmp;
	tmp = start = clock();
	for (int i = 0; i < _geds.size(); i++) {
		cout << "Checking NO." << (i+1) << " GED";
		if (!_geds[i].checkLiteralFormat()) {
			_active[i] = false;
			cout << "\t-1\n"; //Literals are wrong format.
		} else {
			if (!_geds[i].validateGED(_graph)) {
				_active[i] = false;
				cout << "\t0\n"; //The GED is wrong.(maybe pattern or literals do not exist or "X->Y" is not one and only in graph)
			} else {
				cout << "\t1\n"; //The GED is right.(pattern is right and "X->Y" is unique)
			}
		}
		if ((i+1)%200 == 0) {
			end = clock();
			cout << "Cost: " << (double)(end - tmp)/CLOCKS_PER_SEC << " s\n";
			tmp = end;
		}
	}
	end = clock();
	cout << "Total cost: " << (double)(end - start)/CLOCKS_PER_SEC << " s\n";
	cout << "\nThe results:\n";
	for (int i = 0; i < _active.size(); i++) {
		if (_active[i]) {
			cout << "1\t";
		} else {
			cout << "0\t";
		}
		if ((i+1) % 5 == 0) cout << "\n";
	}
	cout << "\n";
}

void CheckGED::writeValidatedGEDs(const string& gedpath) {
	ofstream fout(gedpath + ".vali");
	int cnt = _active.size();
	for (int i = 0; i < cnt; i++) {
		if (_active[i]) {
			string res = "";
			_geds[i].toString(res);
			fout << res;
		}
	}
	fout.close();
}

void CheckGED::delivery(string& gedpath, int frag_num) {
	string line;
  int ged_cnt = 0;
	try{
		ifstream fin(gedpath);
    vector<string> strList;
    while(getline(fin, line)) {
      if (line[0] == '%' && line[1] == 'G') { ged_cnt++;}
      strList.emplace_back(line);
    }
		//cout << frag_num << "\n";
		//cout << ged_cnt << "\n";
    const int volume = ((int) ged_cnt/frag_num) + 1;
		//cout << volume << "\n";

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
		exit(0);
	}
}

int main(int argc, char **argv) {
	int mode = 0;
	int frag_num = 1;
	string filename = "";
	string gedpath = "";
	if (argc < 2) {
		cout << "Missing Parameters!" << endl;
		exit(0);
	}
	mode = atoi(argv[1]);
	if (mode == 1) { //validate GEDs  ./checkGED 1 $Graph_file $GED_path
		if (argc < 4) {
			cout << "Missing Parameters!" << endl;
			exit(0);
		}
		filename = argv[2];
		gedpath = argv[3];

		CheckGED cg;
		Graph& g = cg.graph();
		vector<GED>& geds = cg.geds();

		cg.loadGraph(filename);
		cg.printGraph();

		cg.loadGEDs(gedpath);
		cg.printGEDs();

	#ifdef BOOST_GRAPH
		cg.convert2BG(g, geds);
		cg.boost_vf2();
	#else
	  //validate
	  cg.validation();
	  //write file
	  cg.writeValidatedGEDs(gedpath);
	#endif
	} else if (mode == 2) { //cut GEDs ./checkGED 2 $GED_path NUM
		if (argc < 4) {
			cout << "Missing Parameters!" << endl;
			exit(0);
		}
		gedpath = argv[2];

		int frag_num = atoi(argv[3]);
		CheckGED cg;
		cg.delivery(gedpath, frag_num);
	} else if (mode == 3) { //rewrite Graph ./checkGED 3 $Graph_file
		if (argc < 3) {
			cout << "Missing Parameters!" << endl;
			exit(0);
		}
		filename = argv[2];

		CheckGED cg;
		Graph& g = cg.graph();

		cg.loadGraph(filename);
		cg.printGraph();
		// rewrite graph, make vertices start from id 0 and continous.
		string repath = filename + ".remap";
		g.rewriteGraph(repath);
	}
	return 1;
}
