/*
 * Written by Andrea Pennisi
 */

#ifndef _CAMERA_PARAM_H_
#define _CAMERA_PARAM_H_

#include <iostream>

namespace socialdistancing
{
    namespace config
    {
        class CameraParam
        {
            public:
                /**
                 * @brief Constructor class KalmanParam
                 */
                CameraParam() { ; }
                
                /**
                 * @brief set the path to the stream device
                 * @param s string containing the path
                 */
                void 
                setStream(const std::string& s)
                {
                    stream = s;
                }
                
                /**
                 * @brief get the path to the stream device
                 * @return string containing the path
                 */
                inline const std::string 
                getStream() const
                {
                    return stream;
                }
                
                /**
                 * @brief set the path to the homography file
                 * @param h string containing the path
                 */
                void
                setHomography(const std::string& h)
                {
                    homography = h;
                }
                
                /**
                 * @brief set the path to the calibration file
                 * @param h string containing the path
                 */
                void
                setCalibration(const std::string& c)
                {
                  calibration = c;
                }
                
                /**
                 * @brief get the path to the calibration file
                 * @return a string containing the path
                 */

                inline const std::string
                getCalibration() const
                {
                  return calibration;
                }
                
                /**
                 * @brief get the path to the homography file
                 * @return a string containing the path
                 */
                inline const std::string
                getHomography() const
                {
                    return homography;
                }
                
                
                /**
                 * @brief copy operator
                 * @param _param object to copy
                 * @return the pointer to the current instance
                 */ 
                CameraParam& operator=(const CameraParam& _param)
                {
                    stream = _param.getStream();
                    homography = _param.getHomography();
                    return *this;
                }
                
                /**
                 * @brief print all the parameters
                 */
                void 
                print()
                {
                    std::cout << "[STREAM]: " << stream << std::endl;
                    if(homography.empty())
                    {
                      std::cout << "[CALIBRATION]: " << calibration << std::endl;
                    }
                    else
                    {
                      std::cout << "[HOMOGRAPHY]: " << homography << std::endl;
                    }
                }
                
            private:
                std::string stream;
                std::string homography;
                std::string calibration;
                std::string fieldOfView;
                bool proximity;
        };
    }
}

#endif
