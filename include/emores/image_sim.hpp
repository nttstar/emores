#ifndef EMORES_IMAGESIM_HPP_
#define EMORES_IMAGESIM_HPP_

#include <string>
#include <vector>
#include <set>
#include "sift.hpp"
#include "compact_sift.hpp"
#include "random_projection.hpp"

namespace emores {
class ImageSim {
private:
    typedef std::map<csift_t, std::vector<block_t> > csift_map_t;
    typedef std::set<block_t> block_set_t;
public:
    typedef CompactSift::Point Point;
    ImageSim() {
        sift_ = new Sift(200, 300, 300);
        csift_ = new CompactSift(sift_, 11, 10000);
    }

    ~ImageSim() {
        delete csift_;
        delete sift_;
    }

    double Sim(const std::string& img1, const std::string& img2) {
        std::vector<Point> points1;
        std::vector<Point> points2;
        csift_->GetSift(img1, points1);
        csift_->GetSift(img2, points2);
        if(points1.empty()||points2.empty())
        {
            return -1.0;
        }
        csift_map_t map1;
        csift_map_t map2;
        for(uint32_t i=0;i<points1.size();i++) {
            map1[points1[i].sift_value].push_back(points1[i].block);
        }
        for(uint32_t i=0;i<points2.size();i++) {
            map2[points2[i].sift_value].push_back(points2[i].block);
        }
        block_set_t block_set_1;
        block_set_t block_set_2;
        uint32_t common = 0;
        for(csift_map_t::const_iterator it1 = map1.begin();it1!=map1.end();++it1) {
            csift_map_t::const_iterator it2 = map2.find(it1->first);
            if(it2==map2.end()) continue;
            const std::vector<block_t>& blocks1 = it1->second;
            const std::vector<block_t>& blocks2 = it2->second;
            uint32_t mcount = BlockMatch_(blocks1, blocks2, block_set_1, block_set_2);
            common += mcount;
        }
        std::cerr<<points1.size()<<","<<points2.size()<<","<<common<<","<<block_set_1.size()<<","<<block_set_2.size()<<std::endl;
        //for(block_set_t::const_iterator it = block_set_1.begin(); it!=block_set_1.end(); ++it) {
        //    std::cerr<<"block1 : "<<(int)(*it)<<std::endl;
        //}
        //for(block_set_t::const_iterator it = block_set_2.begin(); it!=block_set_2.end(); ++it) {
        //    std::cerr<<"block2 : "<<(int)(*it)<<std::endl;
        //}
        double r = (double)common/ std::min(points1.size(), points2.size());
        return r;
    }


private:
    uint32_t BlockMatch_(const std::vector<block_t>& blocks1, 
      const std::vector<block_t>& blocks2,
      block_set_t& block_set_1, block_set_t& block_set_2) const
    {
        uint32_t mcount = 0;
        for(uint32_t i=0;i<blocks1.size();i++) {
            block_t b1 = blocks1[i];
            std::vector<uint8_t> flag(blocks2.size(), 0);
            std::pair<double, uint32_t> best(-1.0, 0);
            for(uint32_t j=0;j<blocks2.size();j++) {
                if(flag[j]>0) continue;
                block_t b2 = blocks2[j];
                double dist = BlockDistance_(b1, b2);
                if(dist>8.0) continue;
                if(best.first<0.0 || dist<best.first) {
                    best.first = dist;
                    best.second = j;
                }
            }
            if(best.first>=0.0) {
                mcount++;
                flag[best.second] = 1;
                block_set_1.insert(b1);
                block_set_2.insert(blocks2[best.second]);
            }
        }
        return mcount;
    }

    double BlockDistance_(block_t b1, block_t b2) const {
        block_t y1 = b1/16;
        block_t x1 = b1%16;
        block_t y2 = b2/16;
        block_t x2 = b2%16;
        int16_t xdiff = x1;
        xdiff-=x2;
        xdiff*=xdiff;
        int16_t ydiff = y1;
        ydiff-=y2;
        ydiff*=ydiff;

        return std::sqrt( (double)xdiff+ydiff );

    }



private:
    Sift* sift_;
    CompactSift* csift_;
};
}

#endif
