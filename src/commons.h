/*
    Copyright (C) 2016 Marco Gulino (marco AT gulinux.net)

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
*/

#ifndef INDI_PROPERTIES_LIB_INDI_PROPERTY_COMMONS
#define INDI_PROPERTIES_LIB_INDI_PROPERTY_COMMONS

#include <tuple>
#include <vector>
#include <string>
#include "c++/containers_streams.h"

namespace INDI {
namespace Properties {

  template<typename Tuple, typename A, typename B>
  Tuple tuple_for(int index, A *a, B *b) {
    return std::make_tuple(a[index], b[index]);
  }
  template<typename Tuple, typename A, typename B, typename C, typename D, typename E>
  Tuple tuple_for(int index, A *a, B *b, C *c, D *d, E *e) {
    return std::make_tuple(a[index], b[index], c[index], d[index], e[index]);
  }
  
  template<typename T, typename ...Args>
  std::vector<T> mkvector(int n, Args ...args) {
    std::vector<T> result(n);
    for(int index = 0; index < n; index++)
      result[index] = tuple_for<T>(index, args...);
    return result;
  }
}
}
#endif