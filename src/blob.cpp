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

#include "blob.h"
#include "commons.h"
using namespace std;
using namespace INDI::Properties;

Blob::Blob(Property< Blob >& main, OnUpdate on_update) : main {main}, on_update {on_update}
{
}


Blob::single_property Blob::new_property(const string& name, const string& label, const string &format)
{
    single_property s;
    IUFillBLOB(&s, name.c_str(), label.c_str(), format.c_str());
    return s;
}

void Blob::fill_vector()
{
    IUFillBLOBVector(&main.m_vector_property, main.m_properties.data(), main.m_properties.size(),
                     UNPACK_IDENTITY(main), main.m_base_options.permissions, main.m_base_options.timeout, main.m_base_options.state);
}

void Blob::send(const string& message)
{
    IDSetBLOB(&main.m_vector_property, message.empty() ? message.c_str() : nullptr);
}

void Blob::do_register() const
{
    main.m_device->defineBLOB(&main.m_vector_property);
}

bool Blob::update(int sizes[], int blobsizes[], char *blobs[], char *formats[], char *names[], int n)
{
    if(! on_update(mkvector<UpdateArgs>(n, sizes, blobsizes, blobs, formats, names) ))
        return false;
    return IUUpdateBLOB(&main.m_vector_property, sizes, blobsizes, blobs, formats, names, n) == 0;
}


