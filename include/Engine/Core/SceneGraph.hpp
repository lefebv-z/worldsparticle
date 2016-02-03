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

#ifndef     __ENGINE_SCENE_GRAPH_HPP__
# define    __ENGINE_SCENE_GRAPH_HPP__

#include    <assimp/scene.h>

namespace   Engine
{
    class       Scene;
    class       SceneGraphNode;
    class       AssimpScene;

    class       SceneGraph final
    {
        public:
            ///
            /// \brief Default constructor for the scene graph.
            ///
            /// This constructor will generate a empty scene graph.
            ///
            SceneGraph(Scene *scene);

            ///
            /// \brief Constructor from an assimp scene graph.
            ///
            /// TODO GSL NOT NULL
            ///
            SceneGraph(const AssimpScene &assimpScene, Scene *scene);

            ///
            /// \brief Copy constructor.
            ///
            SceneGraph(const SceneGraph &other);

            ///
            /// \brief Move constructor.
            ///
            SceneGraph(SceneGraph &&other) noexcept;

            ///
            /// \brief Destructor
            ///
            virtual ~SceneGraph(void) noexcept;

        public:
            ///
            /// \brief Copy assignment operator.
            ///
            SceneGraph  &operator=(const SceneGraph &other);

            ///
            /// \brief Move assignment operator.
            ///
            SceneGraph  &operator=(SceneGraph &&other) noexcept;

        public:
            ///
            /// \brief This method is used to recursively update the node in the scene graph.
            ///
            void    update(void);

        public:
            ///
            /// \brief Getter for the scene attribute.
            ///
            Scene   *getScene(void) const;

        private:
            ///
            /// \brief The Scene represented by the scene graph.
            ///
            Scene   *m_scene; // TODO GSL NOT NULL

            ///
            /// \brief The root node of the graph.
            ///
            SceneGraphNode  *m_rootNode; // TODO GSL OWNER

    };
}

#endif /* !__ENGINE_SCENE_GRAPH_HPP__ */
