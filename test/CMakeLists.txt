add_executable(speed_test_wiringpi speed_test_wiringpi.cpp)
target_link_libraries(speed_test_wiringpi PUBLIC -lwiringPi -lpthread)

add_executable(speed_test_pi_gpio speed_test_pigpio.cpp)
target_link_libraries(speed_test_pi_gpio PUBLIC -lpigpio -lpthread)

add_executable(display_test display_test.cpp)
target_link_libraries(display_test led-driver)

add_executable(find_pin find_pin.cpp)
target_link_libraries(find_pin led-driver)
