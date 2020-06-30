#ifndef OMNI_SHADOW_MAP_H
#define OMNI_SHADOW_MAP_H

#include "ShadowMap.h"

class OmniShadowMap : public ShadowMap {
    public:
        OmniShadowMap();

        bool init(unsigned int width, unsigned int height);
        void write();
        void read(GLenum texUnit);

        ~OmniShadowMap();
    private:

};

#endif