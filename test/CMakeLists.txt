add_executable(speed_test_wiringpi speed_test_wiringpi.cpp)
target_link_libraries(speed_test_wiringpi PUBLIC -lwiringPi -lpthread)

add_executable(speed_test_pi_gpio speed_test_pigpio.cpp)
target_link_libraries(speed_test_pi_gpio PUBLIC -lpigpio -lpthread)
