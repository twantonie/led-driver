#include <memory>
#include <vector>

namespace led_driver {

struct Pixels {
  Pixels(int width = 1); // In boards
  void on(int row, int col);
  void off(int row, int col);

  std::array<std::vector<bool>, 16> pixels;
  int width;

private:
  void set(int row, int col, bool on);
};

class Display {
public:
  Display() = default;
  virtual ~Display() = default;

  virtual void set_pixels(Pixels &&pixels) = 0;
};

struct LEDPins {
  int A, B, OE, Clk, LAT, Dr;
};

class SetPins {
private:
  uint32_t set_{0}, clear_{0};

public:
  SetPins();
  void apply();
  void operator()(int pin, bool on);
};

std::unique_ptr<Display> create_led_display(LEDPins pins);

} // namespace led_driver