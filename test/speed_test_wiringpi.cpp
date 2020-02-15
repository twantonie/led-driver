#include <wiringPi.h>

#include <atomic>
#include <iostream>
#include <thread>

int main() {
  int major, minor;
  wiringPiVersion(&major, &minor);
  std::cout << "Wiring pi version: " << major << "." << minor << "\n";

  std::atomic<bool> going{true};

  std::thread t{[&going]() {
    constexpr int pin1{8}, pin2{9};

    wiringPiSetup();

    pinMode(pin1, OUTPUT);
    //pinMode(pin2, OUTPUT);

    while (going) {
      digitalWrite(pin1, HIGH);
      //digitalWrite(pin2, LOW);
      digitalWrite(pin1, LOW);
      //digitalWrite(pin2, HIGH);
    }
  }};

  std::cin.get();
  going = false;
  t.join();
}
