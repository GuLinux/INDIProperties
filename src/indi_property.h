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
#include "c++/containers_streams.h"
#include <tuple>
namespace INDI {
namespace Properties {

struct Identity {
    std::string device, name, label, group;
    IPerm permissions;
    IPState state;
    double timeout;
    Identity(const std::string &device = {}, const std::string &name = {},
             const std::string &label = {}, const std::string &group = {},
             IPerm permissions = IP_RW, IPState state = IPS_OK, double timeout = 60.)
        : device {device}, name {name}, label {label}, group {group}, permissions {permissions}, state {state}, timeout {timeout} {}
};

template<typename T>
class Property {
    typedef typename T::vector_property T_vector_property;
    typedef typename T::single_property T_single_property;
    friend T;
public:
    typedef std::shared_ptr<Property<T>> ptr;
    template<typename ... Args>
    Property(INDI::DefaultDevice *device, const Identity &base_options, Args ... args)
        : m_device {device}, m_property_wrapper {*this, args...}, m_base_options {base_options}
    {
    }

    ~Property() {
        unregister();
    }

    void unregister() {
        if(m_device /* && m_device->getProperty(m_base_options.name.c_str()) TODO: test this */)
            m_device->deleteProperty(m_base_options.name.c_str());
    }
    Identity identity() const {
        return m_base_options;
    }

    std::vector<T_single_property> properties() const {
        return m_properties;
    }

    T *operator->() {
        return &m_property_wrapper;
    }
    T &get() {
        return **this;
    }

    T_vector_property &vector_property() {
        return m_vector_property;
    }
    template<typename ... Args> Property<T> &add(Args ... args) {
        m_properties.push_back(m_property_wrapper.new_property(args...));
        m_property_wrapper.fill_vector();
        return *this;
    }

    template<typename ... Args>
    bool update(const std::string &device, const std::string &name, const Args... args) {
        if( (device != m_base_options.device || name != m_base_options.name) || ! m_property_wrapper.update(args...) )
            return false;
        m_vector_property.s = IPS_OK;
        m_property_wrapper.send();
        return true;
    }

    GuLinux::optional<T_single_property> first() {
        return GuLinux::make_stream(m_properties).first();
    }
    template<typename UnaryFunction>
    GuLinux::optional<T_single_property> first(UnaryFunction f) {
        return GuLinux::make_stream(m_properties).first(f);
    }
    GuLinux::optional<T_single_property> find(const std::string &name) {
        return first([&](const T_single_property &p) {
            return name == p.name;
        });
    }
private:
    T m_property_wrapper;
    Identity m_base_options;
    std::vector<T_single_property> m_properties;
    T_vector_property m_vector_property;
    INDI::DefaultDevice *m_device;
};
}
}

#define CSTR(s) s.c_str()
#define UNPACK_IDENTITY(dev) CSTR(dev.identity().device), CSTR(dev.identity().name), CSTR(dev.identity().label), CSTR(dev.identity().group)

#endif
