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
#include <defaultdevice.h>
#include <algorithm>
#include <memory>

namespace INDI {
namespace Properties {
  template<typename T>
  class Property {
    typedef typename T::vector_property T_vector_property;
    typedef typename T::single_property T_single_property;
    typedef typename T::value_type T_value_type;
    friend T;
  public:
    struct BaseOptions {
      std::string name, device, label, group;
      IPerm permissions;
      IPState state;
      double timeout;
      BaseOptions(const std::string &name = {}, const std::string &device = {}, 
		  const std::string &label = {}, const std::string &group = {}, 
		  IPerm permissions = IP_RW, IPState state = IPS_OK, double timeout = 60.)
	: name{name}, device{device}, label{label}, group{group}, permissions{permissions}, state{state}, timeout{timeout} {}
    };
    template<typename ... Args>
    Property(INDI::DefaultDevice *device, const BaseOptions &base_options, Args ... args) 
      : m_device{device}, m_property_wrapper{*this, args...}, m_base_options{base_options} 
    {
      if(m_device)
	m_property_wrapper.do_register();
    }
    
    ~Property() {
      unregister();
    }
    
    void unregister() {
      if(m_device /* && m_device->getProperty(m_base_options.name.c_str()) TODO: test this */)
	m_device->removeProperty(m_base_options.name.c_str(), nullptr);
    }
    std::string name() const { return m_base_options.name; }
    std::string device() const { return m_base_options.device; }
    std::string label() const { return m_base_options.label; }
    std::string group() const { return m_base_options.group; }
    
    std::vector<T_single_property> properties() const { return m_properties; }
    
    T &operator*() { return m_property_wrapper; }
    T &get() { return **this; }
    
    T_vector_property &vector_property() { return m_vector_property; }
    template<typename ... Args> void add(Args ... args) {
      m_properties.push_back(m_property_wrapper.new_property(args...));
      m_property_wrapper.fill_vector();
    }
    
    template<typename ... Args>
    bool update(const std::string &device, const std::string &name, const Args... args) {
      if(device != m_base_options.device || name != m_base_options.name)
	return false;
      return m_property_wrapper.update(args...);
    }
  private:
    T m_property_wrapper;
    BaseOptions m_base_options;
    std::vector<T_single_property> m_properties;
    T_vector_property m_vector_property;
    INDI::DefaultDevice *m_device;
  };
}
}

#endif