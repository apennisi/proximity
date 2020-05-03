#include "configmanager.h"

using namespace socialdistancing::config;

bool 
ConfigManager::read(const std::string& config)
{
    if(!exist(config))
    {
        throw std::invalid_argument("Invalid file name: " + config);
    }
    yamlManager.read(config);
    
    if(!yamlManager.getElem("Number", cameraNum))
    {
        std::cout << "Camera Number is not specified!" << std::endl;
        return false;
    }
    
    std::string planViewName;
    if(!yamlManager.getElem("Planview", planViewName))
    {
        std::cout << "No Planview is not specified!" << std::endl;
    }
    else
    {
      planView = cv::imread(planViewName);
    
      if(!yamlManager.getElem("Show Planview", show))
      {
        show = false;
      }
    }
 
    
    std::stringstream ss;
    for(auto i = 0; i < cameraNum; ++i)
    {
        CameraParam param;
        ss << "Camera" << i+1;
        std::string tmpVar;
        if(!yamlManager.getElem(ss.str(), tmpVar))
        {
            std::cout << "Camera"<< i+1 << " Stream is not specified!" << std::endl;
            return false;
        }
        param.setStream(tmpVar);

				ss.str("");
        ss << "MinimumDist" << i+1;
				float tmpDist = -1.;
        if(!yamlManager.getElem(ss.str(), tmpDist))
				{
					std::cout << "No minimum distance specified" << std::endl;
				}
				else
				{
					minDist.push_back(tmpDist);
				}

        
        ss.str("");
        ss << "Homography" << i+1;
        auto homography = false;
        if(!yamlManager.getElem(ss.str(), tmpVar))
        {
            std::cout << "Homography"<< i+1 << " is not specified!" << std::endl;
        }
        else
        {
          param.setHomography(tmpVar);
          homography = true;
					if(tmpDist == -1.) 
					{
						tmpDist = 50; //50 pixels
						minDist.push_back(tmpDist);
					}

        }

        auto calibration = false;
        if(!homography)
        {
          ss.str("");
          ss << "Calibration" << i+1; 
          if(!yamlManager.getElem(ss.str(), tmpVar))
          {
            std::cout << "No Homography or Calibration file specified!" << std::endl;
            return false;
          }
					param.setCalibration(tmpVar);
          calibration = true;
					if(tmpDist == -1.)
					{
						tmpDist = 1.; //1 meters
						minDist.push_back(tmpDist);
					}
        }
        
				ss.str("");
        cameraParam.push_back(param);
    }  
        
    std::string detectorTmpVal;
    
    if(!yamlManager.getElem("Config", detectorTmpVal))
    {
        std::cout << "Config is not specified!" << std::endl;
        return false;
    }
    
    if(!exist(detectorTmpVal))
    {
        throw std::invalid_argument("Invalid file name: " + detectorTmpVal);
    }
    
    detectorParam.setConfig(detectorTmpVal);
    
    if(!yamlManager.getElem("Weights", detectorTmpVal))
    {
        std::cout << "Weights is not specified!" << std::endl;
        return false;
    }
    
    if(!exist(detectorTmpVal))
    {
        throw std::invalid_argument("Invalid file name: " + detectorTmpVal);
    }
    
    detectorParam.setWeights(detectorTmpVal);
    
    float thresh;
    
    if(!yamlManager.getElem("MinThreshold", thresh))
    {
        thresh = .8;
    }
    
    detectorParam.setThreshold(thresh);

    return true;    
}

void 
ConfigManager::print()
{
    auto i = 0;
    for(auto& param : cameraParam)
    {
        std::cout << "CAMERA " << i+1 << std::endl;
        param.print();
        i++;
        std::cout << std::endl;
    }
        
    std::cout << std::endl;
    std::cout << "DETECTOR" << std::endl;
    detectorParam.print();
}

bool 
ConfigManager::exist(const std::string& name) const
{
    if(name.find("://")!=std::string::npos) return true;

    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}
