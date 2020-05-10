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

    ss.str("");
    check_tracking = false;
    ss << "Tracking";
    if(!yamlManager.getElem(ss.str(), check_tracking))
    {
      std::cout << "NO TRACKING ACTIVE!" << std::endl;
    }

    ss.str("");

    if(check_tracking)
    {
      for(auto i = 0; i < cameraNum; ++i)
      {
        std::string tmpVar;
        DeepSortParam sortParam;
        ss << "MetricModel" << i+1;
        if(!yamlManager.getElem(ss.str(), tmpVar))
        {
            std::cout << "MetricModel"<< i+1 << " is not specified!" << std::endl;
            return false;
        }
        else
        {
          sortParam.setMetricModel(tmpVar);
        }

        ss.str("");
        ss << "FeatureModel" << i+1;
        if(!yamlManager.getElem(ss.str(), tmpVar))
        {
          std::cout << "FeatureModel" << i+1 << " is not specified!" << std::endl;
          return false;
        }
        else
        {
          sortParam.setFeatureModel(tmpVar);
        }

        float tmpValue;
        ss.str("");
        ss << "NNBudget" << i+1;

        if(!yamlManager.getElem(ss.str(), tmpValue))
        {
          std::cout << "NNBudget" << i+1 << " is not specified!" << std::endl;
          return false;
        }
        else
        {
          sortParam.setNNBudget(tmpValue);
        }

        ss.str("");
        ss << "MaxCosineDistance" << i+1;

        if(!yamlManager.getElem(ss.str(), tmpValue))
        {
          std::cout << "MaxCosineDistance" << i+1 << " is not specified!" << std::endl;
          return false;
        }
        else
        {
          sortParam.setCosineDistance(tmpValue);
        }

        ss.str("");
        ss << "Dt" << i+1;

        if(!yamlManager.getElem(ss.str(), tmpValue))
        {
          std::cout << "Dt" << i+1 << " is not specified!" << std::endl;
          tmpValue = 1.;
        }
        sortParam.setDt(tmpValue);

        ss.str("");
        ss << "IouDistance" << i+1;

        if(!yamlManager.getElem(ss.str(), tmpValue))
        {
          std::cout << "IouDistance" << i+1 << " is not specified!" << std::endl;
          return false;
        }
        else
        {
          sortParam.setMaxIouDistance(tmpValue);
        }

        ss.str("");
        ss << "MaxAge" << i+1;
        int initValue;
        if(!yamlManager.getElem(ss.str(), initValue))
        {
          std::cout << "MaxAge" << i+1 << " is not specified!" << std::endl;
          initValue = 30;
        }
        sortParam.setMaxAge(initValue);

        ss.str("");
        ss << "NInit" << i+1;
        
        if(!yamlManager.getElem(ss.str(), initValue))
        {
          std::cout << "NInit" << i+1 << " is not specified!" << std::endl;
          initValue = 3;
        }
        sortParam.setInitValue(initValue);

        deepParam.push_back(sortParam);

        ss.str("");
      }
    }

    ss.str("");
    
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

    if(check_tracking)
    {
      i = 0;
      for(auto& param : deepParam)
      {
        std::cout << "TRACKING " << i+1 << std::endl;
        param.print();
        i++;
        std::cout << std::endl;
      }
    }
    else
    {
      std::cout << "TRACKING NO ACTIVE" << std::endl;
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
