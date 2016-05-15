/*
 * Copyright (C) 2016 Marco Gulino (marco AT gulinux.net)
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#include "light.h"
using namespace std;
using namespace INDI::Properties;

Light::Light(Property< Light >& main) : main{main}
{
}


Light::single_property Light::new_property(const string& name, const string& label, vtype value)
{
  single_property s;
  IUFillLight(&s, name.c_str(), label.c_str(), value);
  return s;
}

void Light::fill_vector()
{
  IUFillLightVector(&main.m_vector_property, main.m_properties.data(), main.m_properties.size(), 
		     main.device().c_str(), main.name().c_str(), main.label().c_str(),
		     main.group().c_str(), main.m_base_options.state);
  send();
}

void Light::send(const string& message)
{
  IDSetLight(&main.m_vector_property, message.empty() ? message.c_str() : nullptr);
}

void Light::do_register() const
{
  main.m_device->defineLight(&main.m_vector_property);
}

bool Light::update(vtype* values, char* names[], int n)
{
  return false; // Read only widget
}
