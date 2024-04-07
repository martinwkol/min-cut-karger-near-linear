#pragma once

#include <vector>
#include <utility>
#include "rooted_spanning_tree.hpp"

/**
 * Find the cut with the smallest weight that 2-respects the rooted spanning tree
 * @param rst the rooted spanning tree of which the smallest 2-respecting cut is supposed to be found
 * @return a pair of a vector of vertex indices from one side of the cut and the weight of the cut
*/
std::pair<std::vector<VertexID>, EdgeWeight> findSmallest2RespectingCut(const RootedSpanningTree& rst);