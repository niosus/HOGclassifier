FIND_PACKAGE(OpenMP)
FIND_PACKAGE(OpenCV 2 REQUIRED)

if(OPENMP_FOUND)
 set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -D_PWN_USE_OPENMP_ -DEIGEN_DONT_PARALLELIZE")
 set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif(OPENMP_FOUND)

if(OPENCV_FOUND)
  message("opencv found")
endif(OPENCV_FOUND)

ADD_LIBRARY(logger_lib
	logger.h logger.cpp
)

ADD_LIBRARY(parser_lib
	directory_parser.h directory_parser.cpp
)

ADD_LIBRARY(svm_lib
	svm.h svm.cpp
	svm_binder.h svm_binder.cpp
)

ADD_LIBRARY(feature_detection_lib
	feature_detector.h feature_detector.cpp)

ADD_EXECUTABLE(train_and_predict main.cpp)

SET_TARGET_PROPERTIES(logger_lib PROPERTIES OUTPUT_NAME ${LIB_PREFIX}_logger_lib)
SET_TARGET_PROPERTIES(parser_lib PROPERTIES OUTPUT_NAME ${LIB_PREFIX}_parser_lib)
SET_TARGET_PROPERTIES(svm_lib PROPERTIES OUTPUT_NAME ${LIB_PREFIX}_svm_lib)
SET_TARGET_PROPERTIES(feature_detection_lib PROPERTIES OUTPUT_NAME ${LIB_PREFIX}_feature_detection_lib)

SET_TARGET_PROPERTIES(train_and_predict PROPERTIES OUTPUT_NAME train_and_predict)

TARGET_LINK_LIBRARIES(svm_lib logger_lib)
TARGET_LINK_LIBRARIES(feature_detection_lib svm_lib ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(train_and_predict parser_lib svm_lib feature_detection_lib logger_lib ${OpenCV_LIBS})


