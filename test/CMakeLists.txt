add_executable(gpio_info gpio_info.cpp)
target_link_libraries(gpio_info PRIVATE -lgpiodcxx)

add_executable(gpio_set gpio_set.cpp)
target_link_libraries(gpio_set PRIVATE -lgpiodcxx)

add_executable(test_ab test_ab.cpp)
target_link_libraries(test_ab PRIVATE led-driver)

add_executable(speed_test_gpiod speed_test_gpiod.cpp)
target_link_libraries(speed_test_gpiod PUBLIC -lgpiodcxx -lpthread)

add_executable(speed_test_wiringpi speed_test_wiringpi.cpp)
target_link_libraries(speed_test_wiringpi PUBLIC -lwiringPi -lpthread)
