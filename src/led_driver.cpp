#include "led_driver.hpp"

#include <chrono>
#include <iostream>

namespace led_driver {

DriveAB::DriveAB(std::string chip_descr, unsigned int A_gpio,
                 unsigned int B_gpio) {
  gpiod::chip chip(chip_descr);
  // Messed up naming down below, this is the easiest fix
  lines_ = chip.get_lines({B_gpio, A_gpio});
  lines_.request({"DriveAB", gpiod::line_request::DIRECTION_OUTPUT, 0}, {0, 0});
}

DriveAB::~DriveAB() { stop(); }

void DriveAB::start() {
  if (going_)
    return;

  if (thread_.joinable())
    thread_.join();

  going_ = true;
  thread_ = std::thread(&DriveAB::loop_, this);
}

void DriveAB::stop() {
  going_ = false;
  if (thread_.joinable())
    thread_.join();
}

namespace {
enum class States { A_High_B_High, A_High_B_Low, A_Low_B_High, A_Low_B_Low };

std::vector<int> get_ab_values(States &state) {
  switch (state) {
  case States::A_High_B_High:
    state = States::A_High_B_Low;
    return {1, 1};
  case States::A_High_B_Low:
    state = States::A_Low_B_High;
    return {1, 0};
  case States::A_Low_B_High:
    state = States::A_Low_B_Low;
    return {0, 1};
  case States::A_Low_B_Low:
    state = States::A_High_B_High;
    return {0, 0};
  default:
    state = States::A_High_B_High;
    return {0, 0};
  }
}
} // namespace

void DriveAB::loop_() {
  States next_state = States::A_High_B_High;
  auto sleep_til = std::chrono::steady_clock::now();
  while (going_) {
    sleep_til += std::chrono::microseconds(500);
    auto values = get_ab_values(next_state);
    lines_.set_values(values);
    std::this_thread::sleep_until(sleep_til);
  }
}

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
