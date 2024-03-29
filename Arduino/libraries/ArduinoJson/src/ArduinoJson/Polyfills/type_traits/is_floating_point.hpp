// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "integral_constant.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename>
struct is_floating_point : false_type {};

template <>
struct is_floating_point<float> : true_type {};

template <>
struct is_floating_point<double> : true_type {};
}  // namespace Internals
}  // namespace ArduinoJson
