function(applications)

	include_directories(${PROJECT_BINARY_DIR}/../src/apps/include/)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/apps/include/)
	file(GLOB PROX_SRC "src/apps/src/proximity.cpp")
	add_executable(proximity ${PROX_SRC})
	target_link_libraries(proximity ${OpenCV_LIBS} featuregetter thirdpart matching config objectdetector utils homography segmentation)

	file(GLOB PROXIMITY_SRC "src/apps/src/multi_camera_proximity.cpp")
	add_executable( multi_camera_proximity ${PROXIMITY_SRC})
	target_link_libraries( multi_camera_proximity ${OpenCV_LIBS} objectdetector config utils homography)
	
	file(GLOB HOMOGRAPHY_SRC "src/apps/src/homography_app.cpp")
	add_executable( homography_app ${HOMOGRAPHY_SRC})
	target_link_libraries( homography_app ${OpenCV_LIBS} homography config)
	
endfunction()
