#include <gpiod.hpp>

#include <atomic>
#include <iostream>
#include <thread>

int main() {
  std::atomic<bool> going{true};

  std::thread t{[&going]() {
    gpiod::chip chip("gpiochip0");
    auto lines = chip.get_lines({2, 3});
    lines.request({"DriveAB", gpiod::line_request::DIRECTION_OUTPUT, 0},
                  {0, 0});

    while (going) {
      lines.set_values({1, 0});
      lines.set_values({0, 1});
    }
  }};

  std::cin.get();
  going = false;
  t.join();
}