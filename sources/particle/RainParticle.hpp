#ifndef RAINPARTICLE_H
#define RAINPARTICLE_H

#include "Particle.hpp"

class RainParticle : public Particle
{
public:
    RainParticle(const glm::vec3 &position = glm::vec3(0.0),
                const glm::vec3 &rotation = glm::vec3(0.0),
                const glm::vec3 &scale = glm::vec3(1.0),
                ParticleType type = SNOW_PARTICLE);
};

#endif // RAINPARTICLE_H
