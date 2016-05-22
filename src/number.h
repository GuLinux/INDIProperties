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

#ifndef INDI_PROPERTIES_NUMBER_H
#define INDI_PROPERTIES_NUMBER_H

#include <indiapi.h>
#include <string>
#include <functional>
#include "indi_property.h"
namespace INDI {
namespace Properties {

class Number
{
public:
  typedef INumberVectorProperty vector_property;
  typedef INumber single_property;
  typedef double vtype;
  typedef std::tuple<vtype, std::string> UpdateArgs;
  typedef std::function<bool(std::vector<UpdateArgs>)> OnUpdate;
  Number(Property<Number> &main, OnUpdate on_update);
  single_property new_property(const std::string& name, const std::string& label, vtype min, vtype max, vtype step, vtype value, const std::string format = "%f");
  void fill_vector();
  void do_register() const;
  bool update(vtype *values, char *names[], int n);
  void send(const std::string &message = {});
  
private:
  Property<Number> &main;
  OnUpdate on_update;
};
}
}

#endif // INDI_PROPERTIES_NUMBER_H
