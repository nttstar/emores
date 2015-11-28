#ifndef EMORES_IMAGEMATCHER_HPP_
#define EMORES_IMAGEMATCHER_HPP_

#include <string>
#include <vector>
#include "sift.hpp"
#include "compact_sift.hpp"
#include "random_projection.hpp"

namespace emores {
class ImageMatcher {


private:
    CompactSift* sift_;
};
}

#endif
