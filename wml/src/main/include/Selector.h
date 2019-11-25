#pragma once

#include <algorithm>
#include <cmath>

namespace wml {
  class Selector {
   public:
    Selector(int length = 3, int position = 0, bool wrap = false) : _length(length), _position(position), _wrap(wrap) {};

    int Get() const { return _position; };
    int GetLength() const { return _length; };

    int Set(int position) { return Shift(position - Get()); };

    int ShiftLeft(int amount = 1) { Shift(-std::abs(amount)); return Get(); };
    int ShiftRight(int amount = 1) { Shift(std::abs(amount)); return Get(); };

    int Shift(int amount) {
      _position += amount;

      if (_wrap) {
        while (_position < 0) _position += _length;
        _position %= _length;
      } else {
        _position = std::max(0, std::min(_length - 1, _position));
      }

      return Get();
    };

   private:
    const int _length;
    int _position;
    const bool _wrap;
  };
}
