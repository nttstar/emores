#ifndef EMORES_IMAGEMATCHER_HPP_
#define EMORES_IMAGEMATCHER_HPP_

#include <string>
#include <vector>
#include "sift.hpp"
#include "compact_sift.hpp"
#include "random_projection.hpp"

namespace emores {
class ImageMatcher {
public:
    typedef CompactSift::Point Point;
    ImageMatcher() {
        sift_ = new Sift(200, 300, 300);
        csift_ = new CompactSift(sift_, 11, 10000);
    }

    ~ImageMatcher() {
        delete csift_;
        delete sift_;
    }

    bool IsMatched(const std::string& img1, const std::string& img2) {
        std::vector<Point> points1;
        std::vector<Point> points2;
        csift_->GetSift(img1, points1);
        csift_->GetSift(img2, points2);
        if(points1.empty()||points2.empty())
        {
            return -1.0;
        }
        std::set<uint32_t> set;
        for(uint32_t i=0;i<points1.size();i++) {
            set.insert(points1[i].sift_value);
        }
        uint32_t common = 0;
        for(uint32_t i=0;i<points2.size();i++) {
            if(set.find(points2[i].sift_value)!=set.end()) {
                common++;
            }
        }
        std::cerr<<points1.size()<<","<<points2.size()<<","<<common<<std::endl;
        double r = (double)common/ std::min(points1.size(), points2.size());
        return r;
    }

private:
    Sift* sift_;
    CompactSift* csift_;
};
}

#endif
