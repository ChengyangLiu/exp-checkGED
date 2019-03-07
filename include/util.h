#include <iostream>

using namespace std;

enum GED_TYPE {
  EQ_LET = 0,
  EQ_VAR = 1,
  EQ_ID = 2,
};

template <typename T>
bool const interEmpty (vector<T> const& a, vector<T> const& b) {
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < b.size(); j++) {
      if (a[i] == b[i]) {
        return false;
      }
    }
  }
  return true;
}
