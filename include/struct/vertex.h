#include <iostream>

using namespace std;

/* vertex of a graph */
class Vertex {
 public:
  Vertex() {}
  ~Vertex() {}

  Vertex(long id, long label, string& value)
      : _id(id), _label(label), _value(value) {}

  inline long id() { return _id; }
  inline long label() { return _label; }
  inline const std::string& value() { return _value; }

 private:
  long _id;            // vertex id
  long _label;         // vertex label
  std::string _value;  // vertex value
};
