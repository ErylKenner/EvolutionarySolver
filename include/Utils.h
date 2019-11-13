#ifndef UTILS_H
#define UTILS_H

template <class T>
T clamp(const T &value, const T &min, const T &max) {
  if (value > max) {
    return max;
  } else if (value < min) {
    return min;
  }
  return value;
}

#endif
