#include "display.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

namespace led_driver {

Pixels::Pixels(int width_, int height_)
    : pixels(width * height, 0), width(width_), height(height_) {}

void Pixels::on(int row, int col) { set(row, col, true); }
void Pixels::off(int row, int col) { set(row, col, false); }
void Pixels::set(int row, int col, bool on) {
  const auto loc = width * 32 * row + col;
  pixels.at(loc) = on;
}

class LEDDisplay : public Display {
public:
  LEDDisplay(LEDPins pins) : pins_(pins), thread_(&LEDDisplay::loop_, this) {}

  ~LEDDisplay() {
    going_ = false;
    if (thread_.joinable())
      thread_.join();
  }

  void setPixels(Pixels &&pixels) final {
    if (use_one_) {
      two_ = pixels;
    } else {
      one_ = pixels;
    }

    change_over_ = true;
  }

private:
  void loop_();

  const LEDPins pins_;
  std::thread thread_;

  bool use_one_{true};
  std::atomic<bool> change_over_{false}, going_{true};
  Pixels one_, two_;
};

} // namespace led_driver
