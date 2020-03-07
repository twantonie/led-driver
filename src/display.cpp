#include "display.hpp"

#include <atomic>
#include <chrono>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>

#include <pigpio.h>
#include <pthread.h>

namespace led_driver {

Pixels::Pixels(int width_) : width(width_) {
  int size = width * 32;
  for (auto &p : pixels) {
    p.resize(size, false);
  }
}

void Pixels::on(int row, int col) { set(row, col, true); }
void Pixels::off(int row, int col) { set(row, col, false); }
void Pixels::set(int, int, bool) {
  // const auto loc = width * 32 * row + col;
  // pixels.at(loc) = on;
}

class LEDDisplay : public Display {
public:
  LEDDisplay(LEDPins pins) : pins_(pins), thread_(&LEDDisplay::loop_, this) {
    // sched_param sch;
    // sch.sched_priority = 20;
    // if (pthread_setschedparam(thread_.native_handle(), SCHED_RR, &sch)) {
    //   std::cout << "Failed to setschedparam: " << std::strerror(errno) <<
    //   '\n';
    // }
  }

  ~LEDDisplay() {
    going_ = false;
    if (thread_.joinable())
      thread_.join();
  }

  void set_pixels(Pixels &&pixels) override {
    std::lock_guard grd(mtx_);
    pixels_ = pixels;
  }

private:
  void loop_();

  const LEDPins pins_;
  std::thread thread_;

  std::atomic<bool> going_{true};
  std::mutex mtx_;
  Pixels pixels_{1};
};

std::unique_ptr<Display> create_led_display(LEDPins pins) {
  return std::make_unique<LEDDisplay>(pins);
}

SetPins::SetPins() {
  std::cout << "Pigpio version: " << gpioInitialise() << "\n";
}

void SetPins::apply() {
  if (set_) {
    if (int ret = gpioWrite_Bits_0_31_Set(set_); ret != 0) {
      std::cout << "Set returned " << ret << "\n";
    }
    set_ = 0;
  }

  if (clear_) {
    if (int ret = gpioWrite_Bits_0_31_Clear(clear_); ret != 0) {
      std::cout << "Clear returned " << ret << "\n";
    }
    clear_ = 0;
  }
}

void SetPins::operator()(int pin, bool on) {
  if (on) {
    set_ |= (1 << pin);
  } else {
    clear_ |= (1 << pin);
  }
}

class Wait {
public:
  void operator()(std::chrono::nanoseconds wait) {
    sleep_til_ += wait;
    std::this_thread::sleep_until(sleep_til_);
  }

private:
  std::chrono::steady_clock::time_point sleep_til_ =
      std::chrono::steady_clock::now();
};

void clear(const LEDPins &pins, SetPins &set) {
  set(pins.A, false);
  set(pins.B, false);
  set(pins.OE, false);
  set(pins.Clk, false);
  set(pins.LAT, false);
  set(pins.Dr, false);
  set.apply();
}

void dumb_instructions(volatile int instructions) {
  volatile uint16_t cnt{0};
  for (volatile int i = 0; i < instructions; i++) {
    cnt++;
  }
}

void LEDDisplay::loop_() {
  bool a{true}, b{true};
  SetPins set;

  set(pins_.OE, true);

  Wait wait;
  while (going_) {
    std::lock_guard grd(mtx_);

    for (const auto &part : pixels_.pixels) {
      a = !a;
      set(pins_.A, a);
      set(pins_.LAT, true);

      if (a) {
        b = !b;
        set(pins_.B, b);
      }

      set.apply();
      // dumb_instructions(90);
      wait(std::chrono::microseconds(5));

      set(pins_.LAT, false);

      set.apply();
      wait(std::chrono::microseconds(5));

      bool prev = false;
      for (auto p : part) {
        set(pins_.Clk, true);

        if (p != prev) {
          prev = p;
          set(pins_.Dr, !prev);
        }

        set.apply();

        set(pins_.Clk, false);

        set.apply();
      }

      set(pins_.Clk, true);
      set(pins_.Dr, true);
      set.apply();

      wait(std::chrono::microseconds(490));
    }
  }

  clear(pins_, set);
}

} // namespace led_driver
