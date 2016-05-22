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

#include "text.h"
#include "commons.h"

using namespace std;
using namespace INDI::Properties;

Text::Text(Property< Text >& main, OnUpdate on_update) : main {main}, on_update {on_update}
{
}


Text::single_property Text::new_property(const string& name, const string& label, vtype value)
{
    single_property s;
    IUFillText(&s, name.c_str(), label.c_str(), value);
    return s;
}

void Text::fill_vector()
{
    IUFillTextVector(&main.m_vector_property, main.m_properties.data(), main.m_properties.size(),
                     UNPACK_IDENTITY(main), main.m_base_options.permissions, main.m_base_options.timeout, main.m_base_options.state);
}

void Text::send(const string& message)
{
    IDSetText(&main.m_vector_property, message.empty() ? message.c_str() : nullptr);
}

void Text::do_register() const
{
    main.m_device->defineText(&main.m_vector_property);
}

bool Text::update(vtype* values, char* names[], int n)
{
    if(! on_update(mkvector<UpdateArgs>(n, values, names)))
        return false;
    return IUUpdateText(&main.m_vector_property, const_cast<char**>(values), names, n) == 0;
}

void Text::save_config(FILE* fp) const
{
  return IUSaveConfigText(fp, &main.m_vector_property);
}
