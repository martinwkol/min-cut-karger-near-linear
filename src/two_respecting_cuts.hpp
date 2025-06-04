#pragma once

#include <vector>
#include <utility>
#include "types.hpp"
#include "rooted_spanning_tree.hpp"

/**
 * Find the cut with the smallest weight that 2-respects the rooted spanning tree
 * @param rst the rooted spanning tree of which the smallest 2-respecting cut is supposed to be found
 * @return cut
*/
Cut findSmallest2RespectingCut(const RootedSpanningTree& rst);