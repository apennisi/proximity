function(features) 
  include_directories(${PROJECT_BINARY_DIR}/../src/utils/include/)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/utils/include/)
  include_directories(${PROJECT_BINARY_DIR}/../src/features/include/)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/features/include/)
      
  file(GLOB_RECURSE FEATURE_SRC "src/features/src/*")
  
  add_library(featuregetter SHARED ${FEATURE_SRC})
  target_link_libraries(featuregetter ${OpenCV_LIBS} TensorflowCC::TensorflowCC ${CUDA_LIBRARIES} utils)
   	
endfunction()
