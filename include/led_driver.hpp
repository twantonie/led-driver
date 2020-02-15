#include <gpiod.hpp>

#include <atomic>
#include <thread>
#include <vector>

namespace led_driver {

class DriveAB {
public:
  DriveAB(std::string chip, unsigned int A_gpio, unsigned int B_gpio);
  ~DriveAB();
  void start();
  void stop();

private:
  void loop_();
  std::thread thread_;
  std::atomic<bool> going_{false};

  gpiod::line_bulk lines_;
};

struct Pixels {
  Pixels(int width = 1, int height = 1); // In boards
  void on(int row, int col);
  void off(int row, int col);

  std::vector<bool> pixels;
  int width;
  int height;

private:
  void set(int row, int col, bool on);
};

class Display {};

} // namespace led_driver