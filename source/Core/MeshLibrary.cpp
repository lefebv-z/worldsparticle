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

#include    <log4cpp/Category.hh>

#include    "Engine/Core/MeshLibrary.hpp"
#include    "Engine/Core/MaterialLibrary.hpp"

using namespace     log4cpp;

namespace   Engine
{
    MeshLibrary::MeshLibrary(void) :
        Library<Mesh *>()
    {
        // nothing to do.
    }

    MeshLibrary::MeshLibrary(const MaterialLibrary &materials,
            aiMesh **assimpMeshes, unsigned int size) :
        Library<Mesh *>()
    {
        Category    &root = Category::getRoot();

        root << Priority::DEBUG << "MeshLibrary constructor with size = " << size;
        this->m_resources.reserve(size);
        for (unsigned int i = 0 ; i < size ; ++i)
        {
            const aiMesh *amesh = assimpMeshes[i];
            Material *material = materials.get(amesh->mMaterialIndex);
            this->m_resources.push_back(new Mesh(amesh, material));
	    if (this->m_resources.back()->hasNormals())
		root << Priority::DEBUG << "mesh has normals";
	    if (this->m_resources.back()->hasPositions())
		root << Priority::DEBUG << "mesh has positions";
	    if (this->m_resources.back()->hasUVs())
		root << Priority::DEBUG << "mesh has UVs";
	    if (this->m_resources.back()->hasIndices())
		root << Priority::DEBUG << "mesh has indices";
        }
    }

    MeshLibrary::MeshLibrary(const MeshLibrary &other) :
        Library<Mesh *>()
    {
        this->m_resources.reserve(other.m_resources.size());
        for (Mesh *resource : other.m_resources)
        {
             this->m_resources.push_back(new Mesh(*resource));
        }
    }

    MeshLibrary::~MeshLibrary(void)
    {
        for (Mesh *resource : this->m_resources)
        {
            delete resource;
        }
    }



    MeshLibrary &
    MeshLibrary::operator=(const MeshLibrary &other)
    {
         this->m_resources.reserve(other.m_resources.size());
         for (Mesh *resource : other.m_resources)
         {
             this->m_resources.push_back(new Mesh(*resource));
         }
         return *this;
    }
}
