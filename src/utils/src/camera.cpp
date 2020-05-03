#include "camera.h"
#include <eigen3/Eigen/src/Core/Matrix.h>
#include <eigen3/Eigen/src/Geometry/Quaternion.h>
#include <opencv2/calib3d.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/eigen.hpp>

using namespace socialdistancing;
using namespace socialdistancing::utils;

Camera
::Camera(const std::string& stream, const std::string& homography_file, const std::string& calibration_file)
{
    cap.open(stream);
    if(!cap.isOpened())
    {
        throw std::invalid_argument("Invalid Stream File: " + stream);
    }
        
    readCalibration(homography_file, calibration_file);  
}

void
Camera::readCalibration(const std::string& homography_file, const std::string& calibration_file)
{
  YamlManager calibrationReader;
  if(homography_file.empty())
  {
    if(calibration_file.empty())
    {
      throw std::invalid_argument("No calibration or homography file provided!");
    }
    else
    {
      calibrationReader.read(calibration_file);
      cameraMatrix = cv::Mat(3, 3, CV_32FC1, cv::Scalar(0.));
      cameraMatrix.at<float>(2, 2) = 1.;
      float tmpValue;
      if(!calibrationReader.getElem("FocalLengthX", tmpValue))
      {
        throw std::invalid_argument("No FocalLengthX!");
      }
      cameraMatrix.at<float>(0,0) = tmpValue;
      
      if(!calibrationReader.getElem("FocalLengthY", tmpValue))
      {
        throw std::invalid_argument("No FocalLengthY!");
      }
      cameraMatrix.at<float>(1,1) = tmpValue;
      
      if(!calibrationReader.getElem("PrincipalPointX", tmpValue))
      {
        throw std::invalid_argument("No PrincipalPointX!");
      }
      cameraMatrix.at<float>(0,2) = tmpValue;
      
      if(!calibrationReader.getElem("PrincipalPointY", tmpValue))
      {
        throw std::invalid_argument("No PrincipalPointY!");
      }
      cameraMatrix.at<float>(1,2) = tmpValue;

      
      float x,y,z,w;


      if(!calibrationReader.getElem("RotationX", x))
      {
        throw std::invalid_argument("No RotationX!");
      }

      if(!calibrationReader.getElem("RotationY", y))
      {
        throw std::invalid_argument("No RotationY!");
      }

      if(!calibrationReader.getElem("RotationZ", z))
      {
        throw std::invalid_argument("No RotationZ!");
      }

      if(!calibrationReader.getElem("RotationW", w))
      {
        throw std::invalid_argument("No RotationW!");
      }

      rotoTranslation = Eigen::Matrix4f::Identity();
      rotoTranslation.block(0, 0, 3, 3) = Eigen::Quaternionf(w, x, y, z).toRotationMatrix();

      if(!calibrationReader.getElem("TranslationX", x))
      {
        throw std::invalid_argument("No TranslationX!");
      }

      if(!calibrationReader.getElem("TranslationY", y))
      {
        throw std::invalid_argument("No TranslationY!");
      }

      if(!calibrationReader.getElem("TranslationZ", z))
      {
        throw std::invalid_argument("No TranslationZ!");
      }
      
      rotoTranslation(0,3) = x;
      rotoTranslation(1,3) = y;
      rotoTranslation(2,3) = z;

      Eigen::MatrixXf copyRot(3,4);
      copyRot = rotoTranslation.block(0, 0, 3, 4);
      cv::eigen2cv(copyRot, extrinsicMatrix);
      if(!calibrationReader.getElem("DistortionK1", k1))
      {
        throw std::invalid_argument("No DistortionK1!");
      }

      if(!calibrationReader.getElem("DistortionK2", k2))
      {
        throw std::invalid_argument("No DistortionK2!");
      }

      if(!calibrationReader.getElem("DistortionP1", p1))
      {
        throw std::invalid_argument("No DistortionP1!");
      }

      if(!calibrationReader.getElem("DistortionP2", p2))
      {
        throw std::invalid_argument("No DistortionP2!");
      }

      distortion_coefficients = cv::Mat::zeros(1, 4, CV_32F);
      distortion_coefficients.at<float>(0,0) = k1;
      distortion_coefficients.at<float>(0,1) = k2;
      distortion_coefficients.at<float>(0,2) = p1;
      distortion_coefficients.at<float>(0,3) = p2;


      cv::Mat projectionMatrix = cameraMatrix * extrinsicMatrix;

      float p11 = projectionMatrix.at<float>(0, 0),
      p12 = projectionMatrix.at<float>(0, 1),
      p14 = projectionMatrix.at<float>(0, 3),
      p21 = projectionMatrix.at<float>(1, 0),
      p22 = projectionMatrix.at<float>(1, 1),
      p24 = projectionMatrix.at<float>(1, 3),
      p31 = projectionMatrix.at<float>(2, 0),
      p32 = projectionMatrix.at<float>(2, 1),
      p34 = projectionMatrix.at<float>(2, 3);


      Hinv = (cv::Mat_<float>(3, 3) << p11, p12, p14, p21, p22, p24, p31, p32, p34);
      H = Hinv.inv();
    }
  }
  else
  {
    calibrationReader.read(homography_file);
  
    if(!calibrationReader.getElem("Matrix", H, CV_32FC1))
    {
       throw std::invalid_argument("Homography is not specified!");
    }
    Hinv = H.inv();
  }
}

bool 
Camera::getFrame(cv::Mat& frame)
{
    cv::Mat temp;
    cap >> temp;
    if(temp.empty())
    {
        return false;
    }
    if(cameraMatrix.empty())
    {
      frame = temp.clone();
    }
    else
    {
      cv::undistort(temp, frame, cameraMatrix, distortion_coefficients);
    }
    return true;
}

cv::Point2f 
Camera::camera2world(const cv::Point2f& p) const
{
    double x, y;
    socialdistancing::geometry::Homography::calcProjection(p, H, x, y);
    return cv::Point2f(x, y);
}

cv::Point2f 
Camera::world2camera(const cv::Point2f& p) const
{
    double x, y;
    socialdistancing::geometry::Homography::calcProjection(p, Hinv, x, y);
    return cv::Point2f(x, y);
}
