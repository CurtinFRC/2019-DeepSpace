#pragma once

#include <cmath>

namespace curtinfrc {
  class Selector {
   public:
    Selector(int length = 3, int position = 0) : _length(length), _position(position) {};

    int Get() const { return _position; };
    int GetLength() const { return _length; };

    int Set(int position) { return Shift(position - Get()); };

    int ShiftLeft(int amount = 1) { Shift(-std::abs(amount)); return Get(); };
    int ShiftRight(int amount = 1) { Shift(std::abs(amount)); return Get(); };

    int Shift(int amount) {
      _position += amount;
      while (_position < 0) _position += _length;
      _position %= _length;
      return Get();
    };

   private:
    int _length;
    int _position;
  };
}
