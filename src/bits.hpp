#pragma once

namespace bits
{

template <typename T> T SetBit(T value, int n)
{
  return value | (T(1) << n);
}

template <typename T> T ClearBit(T value, int n)
{
  return value & ~(T(1) << n);
}

template <typename T> bool GetBit(T value, int n)
{
  return (value >> n) & 1;
}

} // namespace bits