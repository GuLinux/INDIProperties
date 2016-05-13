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

#ifndef INDI_PROPERTIES_LIB_INDI_PROPERTY
#define INDI_PROPERTIES_LIB_INDI_PROPERTY

#include <string>
#include <vector>

namespace INDI {
namespace Properties {
  template<typename T>
  class Property {
  public:
    typedef T::vector_property vector_property;
    typedef T::single_property single_property;
    std::string name() const { return m_name; }
    std::string device() const { return m_device; }
    std::string m_label() const { return m_label; }
    std::string m_group() const { return m_group; }
    template<typename ... Args> add(Args ... args) {
      vector_property property;
      T::fill_property(&args...);
      m_properties.push_back(property);
    }
  private:
    const std::string m_device;
    const std::string m_name;
    const std::string m_label;
    const std::string m_group;
    std::vector<single_property> m_properties;
  };
}
}

#endif