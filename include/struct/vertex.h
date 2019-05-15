#include <iostream>

#include <stdlib.h>

using namespace std;

/* Class: vertex of a graph
 * Author: Liucy
 * Date: 4,2019
 */
class Vertex {
 public:
  Vertex() {}
  ~Vertex() {}

  Vertex(long id, long label, string& value)
      : _id(id), _label(label), _value(value) {}

  inline long id() { return _id; }
  inline long label() { return _label; }
  inline std::string& value() { return _value; }

 private:
  long _id;            // vertex id
  long _label;         // vertex label
  std::string _value;  // vertex value
};
