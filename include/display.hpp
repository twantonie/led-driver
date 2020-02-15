#include <memory>
#include <vector>

namespace led_driver {

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

struct LEDPins {
  int A, B, OE, Clk, LAT, Dr;
};

std::unique_ptr<Display> create_led_display(LEDPins pins);

} // namespace led_driver