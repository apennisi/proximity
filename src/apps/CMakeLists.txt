function(applications)
	file(GLOB PROXIMITY_SRC "src/apps/src/multi_camera_proximity.cpp")
	add_executable( multi_camera_proximity ${PROXIMITY_SRC})
	target_link_libraries( multi_camera_proximity ${OpenCV_LIBS} objectdetector config utils homography)
	
	file(GLOB HOMOGRAPHY_SRC "src/apps/src/homography_app.cpp")
	add_executable( homography_app ${HOMOGRAPHY_SRC})
	target_link_libraries( homography_app ${OpenCV_LIBS} homography config)
	
endfunction()
