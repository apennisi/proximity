#ifndef _PARAM_H_
#define _PARAM_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>


class DeepSortParam
{
    public:
        DeepSortParam() { ; }

        void setDt(const float& dt) 
        {
          dt_value = dt;
        }

        const float dt() const
        {
          return dt_value;
        }

        void setMetricModel(const std::string& model)
        {
          metric_model_file = model;
        }

        const std::string metric_model() const
        {
          return metric_model_file;
        }

        void setFeatureModel(const std::string& model)
        {
          feature_model_file = model;
        }

        const std::string feature_model() const
        {
          return feature_model_file;
        }

        void setNNBudget(const float& budget)
        {
          args_nn_budget_value = budget;
        }

        const float args_nn_budget() const
        {
          return args_nn_budget_value;
        }

        void setCosineDistance(const float& dist)
        {
          args_max_cosine_distance_value = dist;
        }

        const float args_max_cosine_distance() const
        {
          return args_max_cosine_distance_value;
        }

        void setMaxIouDistance(const float& dist)
        {
          max_iou_distance_value = dist;
        }
        
        const float max_iou_distance() const
        {
          return max_iou_distance_value;
        }

        void setMaxAge(const int age)
        {
          max_age_value = age;
        }

        const int max_age() const
        {
          return max_age_value;
        }

        void setInitValue(const int value)
        {
          n_init_value = value;
        }

        const int n_init() const
        {
          return n_init_value;
        }

        DeepSortParam& operator=(const DeepSortParam& _param)
        {
            this->metric_model_file = _param.metric_model();
            this->feature_model_file = _param.feature_model();
            this->args_nn_budget_value = _param.args_nn_budget();
            this->args_max_cosine_distance_value = _param.args_max_cosine_distance();
            this->dt_value = _param.dt();
            this->max_iou_distance_value = _param.max_iou_distance();
            this->max_age_value = _param.max_age();
            this->n_init_value = _param.n_init();
            return *this;
        }

        const void print() const
        {
            std::cout << "[METRIC_MODEL]: " << metric_model_file << std::endl;
            std::cout << "[FEATURE_MODEL]: " << feature_model_file << std::endl;
            std::cout << "[ARGS_NN_BUDGET]: " << args_nn_budget_value << std::endl;
            std::cout << "[ARGS_MAX_COSINE_DISTANCE]: " << args_max_cosine_distance_value << std::endl;
            std::cout << "[DT]: " << dt_value << std::endl;
            std::cout << "[MAX_IOU_DISTANCE]: " << max_iou_distance_value << std::endl;
            std::cout << "[MAX_AGE]: " << max_age_value << std::endl;
            std::cout << "[N_INIT]: "  << n_init_value << std::endl;
        }

    private:
        std::string metric_model_file;
        std::string feature_model_file;
        float args_nn_budget_value;
        float args_max_cosine_distance_value;
        float dt_value;
        float max_iou_distance_value;
        int max_age_value;
        int n_init_value; 
};


#endif
