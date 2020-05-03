# Proximity

<p align="center">
<a href="https://www.youtube.com/watch?v=XL8tCEtEvtE"  target="_blank"><img src="images/readme_image.png"/></a>
</p>
<br>

Proximity is a proof of concept which measure the social distancing between people in public spaces. It works with calibrated or uncalibrated (with homography) cameras. Moreover, it works with multiple cameras a the same time.

# Requirements
* C++14
* OpenCV
* Darknet ([https://github.com/AlexeyAB/darknet](https://github.com/AlexeyAB/darknet))
* Boost
* Eigen
* CUDA (not mandatory)
* Yaml-cpp

# How to build

1. Clone *darknet* repository into the main folder (proximity).
2. Modify the Makefile in order to use GPU (if any), to use OPENCV, and to build the library:
	- GPU=1, LIBSO=1 and OPENCV=1
3. If necessary, modify the cuda path as well as the architecture (inside the makefile)
4. Inside the darknet folder, go into the forlder include and edit the file yolo_v2_class.hpp
	- Comment Lines 107 and 179 (_#ifdef_ _OPENCV_ and _#endif_)
5. Compile it with: ```make```
6. Download the COCO weights: 
	- [https://pjreddie.com/media/files/yolov3.weights](https://pjreddie.com/media/files/yolov3.weights)
	- Move the downloaded file into the folder *weights* (you have to create it) in the main folder (proximity)
7. In the main folder *proximity*, create a folder build
8. navigate into the folder build and compile it with the following commands: ```cmake .. && make```

# How it works
Download the video for testing from <a herf="https://drive.google.com/open?id=1o5JqqQT9q0Ag6jIHV5id0JNBqZBWgd4i" target="_blank">here</a>, then:
1. To launch the application: ```./multi_camera_proximity ../configs/config_town.yaml```
2. To create a new homography file: ```./homography_app /path/to/the/source/image /path/to/the/destination/image /path/to/yaml/file (where you save the homography)```

# LICENSE
GNU
