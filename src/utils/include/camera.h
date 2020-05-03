/*
 * Written by Andrea Pennisi
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <iostream>
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Dense>
#include "homography.h"
#include "yamlmanager.h"

using namespace socialdistancing::config;

namespace socialdistancing
{
    namespace utils
    {
        class Camera
        {
            public:
                /**
                 * @brief Constructor class Camera
                 * @param stream a string containing the source path of the streaming
                 * @param view a string containing the path to the binary image representing the field of view of the cameras
                 * @param homography_file a string containing the path to the homography file
                 * @param prox a bool which if is true means that the camera is close to scene to monitor
                 */
                Camera(const std::string& stream, const std::string& homography_file = "", const std::string& calibration_file = "");
                /**
                 * @brief get a frame from the stream
                 * @param frame cv::Mat where the frame is stored
                 * @return true if the frame is read successfully
                 */
                bool getFrame(cv::Mat& frame);
                
                /**
                 * @brief convert a point to world coordinates
                 * @param p cv::Point2f to convert
                 * @return the converted point
                 */
                cv::Point2f camera2world(const cv::Point2f& p) const;
                
                /**
                 * @brief convert a point to image coordinates
                 * @param p cv::Point2f to convert
                 * @return the converted point
                 */
                cv::Point2f world2camera(const cv::Point2f& p) const;
            private:
                /**
                 * @brief Read calibration file
                 * @param homography_file path to the homography file if exists
                 * @param calibration_file path to the calibration file if exists
                 */
                void readCalibration(const std::string& homography_file, const std::string& calibration_file);
            private:
                cv::VideoCapture cap;
                cv::Mat H, Hinv;
                cv::Mat cameraMatrix;
                cv::Mat distortion_coefficients;
                float p1, p2, k1, k2;
                cv::Mat rotationMatrix;
                cv::Mat extrinsicMatrix;
                Eigen::Matrix4f rotoTranslation;
        }; 
    }
}

#endif
