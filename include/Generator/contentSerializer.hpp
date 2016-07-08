/* 
 * File:   contentSerializer.hpp
 * Author: marie
 *
 * Created on 8 juillet 2016, 23:13
 */

#ifndef CONTENTSERIALIZER_HPP
#define CONTENTSERIALIZER_HPP

#include "Engine/Configuration.hpp"

namespace GenContent
{
class SceneContent;
}

namespace Engine
{
class Core;
}

namespace gen
{
///
/// \brief This class will convert the contents generated by the ContentGenerator
/// into Entities in the Engine
/// and add them to the current scene in the Engine::Core
///
class ENGINE_EXPORTS ContentSerializer
{
public:
    ContentSerializer(Engine::Core *engine);
    ~ContentSerializer();
    
    void launch(GenContent::SceneContent *contents);

private:
    GenContent::SceneContent  *_contents;
    
    Engine::Core *_engine;
};
}

#endif /* CONTENTSERIALIZER_HPP */

