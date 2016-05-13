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
#include <indiapi.h>

namespace INDI {
namespace Properties {
  template<typename T>
  class Property {
    typedef typename T::vector_property vector_property_t;
    typedef typename T::single_property single_property;
    friend T;
  public:
    struct BaseOptions {
      std::string name, device, label, group;
      IPerm permissions = IP_RW;
      IPState state = IPS_OK;
      double timeout = 60.;
    };
    template<typename ... Args>
    Property(const BaseOptions &base_options, Args ... args) : m_property_wrapper{*this, args...}, m_base_options{base_options} {}
    std::string name() const { return m_base_options.name; }
    std::string device() const { return m_base_options.device; }
    std::string label() const { return m_base_options.label; }
    std::string group() const { return m_base_options.group; }
    
    std::vector<single_property> properties() const { return m_properties; }
    
    vector_property_t &vector_property() { return m_vector_property; }
    template<typename ... Args> void add(Args ... args) {
      m_properties.push_back(m_property_wrapper.new_property(args...));
      m_property_wrapper.fill_vector();
    }
    
  private:
    T m_property_wrapper;
    BaseOptions m_base_options;
    std::vector<single_property> m_properties;
    vector_property_t m_vector_property;
  };
}
}

#endif