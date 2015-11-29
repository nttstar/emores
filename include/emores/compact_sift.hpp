#ifndef EMORES_COMPACTSIFT_HPP_
#define EMORES_COMPACTSIFT_HPP_

#include <string>
#include <vector>
#include "sift.hpp"
#include "random_projection.hpp"

namespace emores {
class CompactSift {
public:
    struct Point {
        block_t block;
        csift_t sift_value;
        bool operator<(const Point& y) const {
            if(sift_value!=y.sift_value) return sift_value<y.sift_value;
            return block<y.block;
        }
    };


public:

    CompactSift(Sift* sift, uint32_t t, uint32_t seed): sift_(sift), rp_(CSIFT_DIM, SIFT_DIM, t, seed) {
    }

    void GetSift(const std::string& img_file, std::vector<Point>& points) const {
        std::vector<Sift::Point> raw_points;
        sift_->GetSift(img_file, raw_points);
        for(uint32_t i=0;i<raw_points.size();i++) {
            Sift::Point& rp = raw_points[i];
            csift_t fp = rp_.Get(rp.desc);
            uint32_t xb = rp.x*16;
            if(xb==16) xb=15;
            uint32_t yb = rp.y*16;
            if(yb==16) yb=15;
            uint32_t b = yb*16+xb;
            Point cp;
            cp.block = b;
            cp.sift_value = fp;
            points.push_back(cp);
        }
    }





private:
    Sift* sift_;
    RandomProjection<csift_t> rp_;

};
}
#endif

