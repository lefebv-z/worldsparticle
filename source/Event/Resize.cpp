//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; If not, see <http://www.gnu.org/licenses/>.
//
// Copyright (C) 2016 Martin-Pierrat Louis (louismartinpierrat@gmail.com)
//

#include    "Engine/Event/Resize.hpp"

namespace   Engine
{
    namespace   Event
    {
        Resize::Resize(const glm::ivec2 &size) :
            Event(Type::RESIZE),
            m_size(size)
        {

        }

        Resize::~Resize(void)
        {

        }



        const glm::ivec2 &
        Resize::size(void) const
        {
             return this->m_size;
        }
    }
}
