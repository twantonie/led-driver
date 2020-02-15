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

class Display {
public:
  Display() = default;
  virtual ~Display() = default;

  virtual void setPixels(Pixels &&pixels);
};

/* enum class Pins {
  SDA = 8,
  SCL = 9,
  GPIO7 = 7,
  TxD = 15,
  RxD = 16,
  GPIO0 = 0,
  GPIO1 = 1,
  GPIO2 = 2,
  GPIO3 = 3,
  GPIO4 = 4,
  GPIO5 = 5,
  MOSI = 12,
  MISO = 13,
  GPIO6 = 6,
  SCLK = 14,
  CE0 = 10,
  CE1 = 11
} */

struct LEDPins {
  int A, B, OE, Clk, LAT, Dr;
};

std::unique_ptr<Display> create_led_display(LEDPins pins);

} // namespace led_driver