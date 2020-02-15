#include <pigpio.h>

#include <atomic>
#include <iostream>
#include <thread>

void check(int res) {
  if (res != 0) {
    throw std::runtime_error("Something went wrong");
  }
}

int main() {
  std::cout << "Pigpio version: " << gpioInitialise() << "\n";

  std::atomic<bool> going{true};

  std::thread t{[&going]() {
    constexpr int pin1{2}, pin2{3};

    check(gpioSetMode(pin1, PI_OUTPUT));
    check(gpioSetMode(pin2, PI_OUTPUT));

    while (going) {
      check(gpioWrite_Bits_0_31_Set((1 << pin1) | (1 << pin2)));
      check(gpioWrite_Bits_0_31_Clear((1 << pin1) | (1 << pin2)));
    }
  }};

  std::cin.get();
  going = false;
  t.join();
}