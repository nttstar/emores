#include <emores/image_sim.hpp>
#include <iostream>
using namespace emores;

int main(int argc, char** argv) {
    std::string img1(argv[1]);
    std::string img2(argv[2]);
    ImageSim is;
    double sim = is.Sim(img1, img2);
    std::cout<<"sim: "<<sim<<std::endl;
}
