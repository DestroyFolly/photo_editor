cmake_minimum_required(VERSION 3.16)
project(tassov_task C)

set(CMAKE_C_STANDARD 99)
add_executable(tassov_task main.c filters/src/grayscale.c filters/inc/grayscale.h inc/types.h src/bmp_loader.c inc/bmp_loader.h filters/src/kernel.c filters/inc/kernel.h filters/src/gaussian_blur.c filters/inc/gaussian_blur.h filters/src/laplace.c filters/inc/laplace.h filters/src/sobel.c filters/inc/sobel.h filters/src/edge_detection.c filters/inc/edge_detection.h filters/src/bmp_process.c filters/inc/bmp_process.h inc/filters.h filters/inc/filters_errors.h)
target_link_libraries(tassov_task m)
