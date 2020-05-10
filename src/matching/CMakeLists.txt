function(matchingtracks)  
  include_directories(${PROJECT_BINARY_DIR}/../src/thirdPart/include/)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/thirdPart/include/)
  include_directories(${PROJECT_BINARY_DIR}/../src/features/include/)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/features/include/)	
  include_directories(${PROJECT_BINARY_DIR}/../src/matching/include/)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/matching/include/)
  #add_definitions(-DUSE_FACE_NET)
    
  file(GLOB_RECURSE MATCHING_SRC "src/matching/*.cpp")
  
  add_library(matching SHARED ${MATCHING_SRC})
  target_link_libraries(matching ${OpenCV_LIBS} config featuregetter thirdpart)
  
endfunction()
