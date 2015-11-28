all:
	g++-4.8 -O2 -o ./bin/image_sim -I./include -lopencv_core -lopencv_features2d  -lopencv_highgui -lopencv_imgproc -lopencv_nonfree process/image_sim.cc
