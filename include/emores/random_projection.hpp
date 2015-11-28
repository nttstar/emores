#ifndef EMORES_RANDOMPROJECTION_HPP_
#define EMORES_RANDOMPROJECTION_HPP_

#include <string>
#include <vector>
//#include <am/succinct/fujimap/fujimap.hpp>
//#include <util/DynamicBloomFilter.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cstdlib>

namespace emores {
class RandomProjection {

public:
    typedef std::pair<uint16_t, int8_t> Item;
    typedef std::vector<Item> Row;
    typedef std::vector<Row> Matrix;
    RandomProjection(uint32_t row, uint32_t col, uint32_t t, uint32_t seed)
    : row_(row), col_(col), t_(t), seed_(seed)
    {
        m_.resize(row_);
        srand(seed);
        for(uint32_t i=0;i<row_;i++) {
            Row& row = m_[i];
            for(uint16_t j=0;j<col_;j++) {
                uint32_t ra = rand() % (t*2);
                int8_t v = 0;
                if(ra==0) v = 1;
                else if(ra==1) v = -1;
                if(v!=0) {
                    row.push_back(std::make_pair(j, v));
                }
            }
        }
    }
    uint32_t Get(std::vector<float>& sift_desc) const {
        float sum = 0.0;
        for(uint32_t i=0;i<sift_desc.size();i++) {
            sum += sift_desc[i]*sift_desc[i];
        }
        sum = std::sqrt(sum);
        for(uint32_t i=0;i<sift_desc.size();i++)
        {
            sift_desc[i] /= sum;
        }
        uint32_t fp=0;
        for(uint32_t f=0;f<row_;f++)
        {
            const Row& row = m_[f];
            float v = 0.0;
            for(uint32_t ri=0;ri<row.size();ri++)
            {
                const Item& item = row[ri];
                v+=sift_desc[item.first]*item.second;
            }
            if(v>0.0)
            {
                uint32_t vv = (1<<f);
                fp |= vv;
            }
        }
        return fp;
    }

private:
    Matrix m_;
    uint32_t row_;
    uint32_t col_;
    uint32_t t_;
    uint32_t seed_;

};

}

#endif
