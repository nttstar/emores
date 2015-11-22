#ifndef EMORES_COMPACTSIFT_HPP_ 
#define EMORES_COMPACTSIFT_HPP_

#include <string>
#include <vector>
#include "sift.hpp"
#include "random_projection.hpp"

namespace emores {
class CompactSift {
public:
    struct CompactPoint {
        uint8_t block;
        uint32_t sift_value;
    };


public:

    CompactSift(Sift* sift, uint32_t t, uint32_t seed): sift_(sift), rp_(32, 128, t, seed) {
    }

    void GetSift(const std::string& img_file, std::vector<CompactPoint>& points) const {
        std::vector<Point> raw_points;
        GetSift(img_file, raw_points);
        for(uint32_t i=0;i<raw_points.size();i++) {
            const Point& rp = raw_points[i];
            uint32_t fp = rp_(rp.desc);
            uint32_t xb = rp.x*16;
            if(xb==16) xb=15;
            uint32_t yb = rp.y*16;
            if(yb==16) yb=15;
            uint32_t b = yb*16+xb;
            CompactPoint cp;
            cp.block = b;
            cp.sift_value = fp;
            points.push_back(cp);
        }
    }


    


private:
    Sift* sift_;
    RandomProjection rp_;

};

}

#endif




