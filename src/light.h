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

#ifndef INDI_PROPERTIES_LIGHT_H
#define INDI_PROPERTIES_LIGHT_H

#include <indiapi.h>
#include <string>
#include <functional>
#include "indi_property.h"
namespace INDI {
namespace Properties {

class Light
{
public:
  typedef ILightVectorProperty vector_property;
  typedef ILight single_property;
  typedef IPState vtype;
  Light(Property<Light> &main);
  single_property new_property(const std::string& name, const std::string& label, vtype value);
  void fill_vector();
  void do_register() const;
  bool update(vtype *values, char *names[], int n);
  void send(const std::string &message = {});
  
private:
  Property<Light> &main;
};
}
}

#endif // INDI_PROPERTIES_LIGHT_H
