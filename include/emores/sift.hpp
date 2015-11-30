#ifndef EMORES_SIFT_HPP_
#define EMORES_SIFT_HPP_

#include <string>
#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "consts.hpp"

namespace emores {
class Sift {
public:

    struct Point {
        float x;//proportion
        float y;
        std::vector<float> desc;
    };

public:

    Sift(uint32_t pcount, uint32_t img_x, uint32_t img_y):sift_(pcount), img_x_(img_x), img_y_(img_y) {
    }
    ~Sift() {
    }

    void GetSift(const std::string& img_file, std::vector<Point>& points) const {
        std::vector<cv::KeyPoint> keypoints;
        cv::Mat des;
        cv::Mat image;
        try {
            image = cv::imread(img_file, CV_LOAD_IMAGE_GRAYSCALE);
            if(image.cols==0 || image.rows==0) return;
            cv::Size img_size(img_x_, img_y_);
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
            sift_(image,mask, keypoints, des);
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
            for(int j=0;j<des.cols;j++)
            {
                float f = des.at<float>(i, j);
                assert(f<=255.0);
                pt.desc[j] = f;
            }
            Normalize_(pt.desc);
            points.push_back(pt);
        }
    }

private:
    void Normalize_(std::vector<float>& fvec) const {
        float sum = 0.0;
        for(uint32_t i=0;i<fvec.size();i++) {
            sum += fvec[i]*fvec[i];
        }
        sum = std::sqrt(sum);
        float sum2 = 0.0;
        for(uint32_t i=0;i<fvec.size();i++) {
            fvec[i] /= sum;
            if(fvec[i]>0.2) fvec[i] = 0.2;
            sum2 += fvec[i]*fvec[i];
        }
        sum2 = std::sqrt(sum2);
        for(uint32_t i=0;i<fvec.size();i++) {
            fvec[i] /= sum2;
        }
    }



private:
    cv::SIFT sift_;
    uint32_t img_x_;
    uint32_t img_y_;

};
}
#endif
