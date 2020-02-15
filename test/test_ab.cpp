#include <led_driver.hpp>

#include <iostream>

int main() {
    led_driver::DriveAB drive("gpiochip0", 2, 3);
    drive.start();
    std::cout << "DriveAB running, press any key to stop\n";
    std::cin.get();
    drive.stop();
}