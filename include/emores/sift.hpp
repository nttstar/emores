#ifndef EMORES_SIFT_HPP_ 
#define EMORES_SIFT_HPP_

#include <string>
#include <vector>
#include <am/succinct/fujimap/fujimap.hpp>
#include <util/DynamicBloomFilter.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace emores {
class Sift {
public:
    struct Param {
        uint32_t pcount;//points count
        uint32_t img_xsize;
        uint32_t img_ysize;
        float xcut;//proportion
        float ycut;
        Param()
        : pcount(200), img_xsize(300), img_ysize(300), xcut(0.25), ycut(0.25)
        {
        }
    };

    struct Point {
        float x;//proportion
        float y;
        std::vector<float> desc;
    };

    struct CompactPoint {
        uint8_t block;
        uint32_t sift_value;
    };


public:

    Sift():sift_(NULL) {
    }
    ~Sift() {
        if(sift_!=NULL) delete sift_;
    }

    Param& GetParam() { return param_; }

    void GetSift(const std::string& img_file, std::vector<Point>& points) const {
        if(sift_==NULL) {
            sift_ = new cv::SIFT(param_.pcount);
        }
        std::vector<cv::KeyPoint> keypoints;
        cv::Mat des;
        cv::Mat image;
        try {
            image = cv::imread(img_file, CV_LOAD_IMAGE_GRAYSCALE);
            if(image.cols==0 || image.rows==0) return;
            cv::Size img_size(param_.img_xsize, param_.img_ysize);
            double wratio = (double)img_size.width/image.cols;
            double hratio = (double)img_size.height/image.rows;
            //LOG(INFO)<<image.cols<<","<<image.rows<<","<<img_size.width<<","<<img_size.height<<std::endl;
            if(wratio<1.0 || hratio<1.0)
            {
                double ratio = std::min(wratio, hratio);
                cv::Mat rimage;
                cv::resize(image, rimage, cv::Size(), ratio, ratio);
                image = rimage;
            }
            cv::Mat mask; //TODO
            (*sift_)(image,mask, keypoints, des);
        }
        catch(std::exception& ex) {
            std::cerr<<ex.what()<<std::endl;
            return;
        }
        if(des.rows==0) return;
        assert(des.rows==keypoints.size());
        //LOG(INFO)<<"[Mat]rows:"<<des.rows<<",cols:"<<des.cols<<std::endl;
        std::size_t sift_count=0;
        for(int i=0;i<des.rows;i++)
        {
            Point pt;
            const cv::KeyPoint& kp = keypoints[i];
            pt.x = (float)kp.pt.x/image.cols;
            pt.y = (float)kp.pt.y/image.rows;
            pt.desc.resize(des.cols, 0.0);
            std::vector<float> fvec(des.cols, 0.0);
            for(int j=0;j<des.cols;j++)
            {
                float f = des.at<float>(i, j);
                assert(f<=255.0);
                pt.desc[j] = f;
            }
            points.push_back(pt);
        }
    }



private:
    Param param_;
    cv::SIFT* sift_;

};

}

#endif



