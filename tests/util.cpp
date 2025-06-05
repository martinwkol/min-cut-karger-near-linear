#include "util.hpp"
#include "types.hpp"
#include <sstream>

std::string cut2string(const Cut& cut) {
    std::stringstream s;
    s << "Weight: " << cut.weight << '\n';
    s << "Vertices: ";
    for (VertexID vertex : cut.vertices) {
        s << vertex << ", ";
    }
    s << '\n';
    return s.str();
}