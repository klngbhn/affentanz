#pragma once

#include "OgreMaterialManager.h"
#include "OgreOverlayManager.h"
#include "OgreTechnique.h"
#include "OgreBlendMode.h"

#include "IEventManager.h"
#include "Log.h"

/*
 * The fader controls an overlay that can be faded in and out over a period of time.
 */
 class Fader
 {
 public:
     Fader(const char* overlayName, const char* materialName);
     ~Fader(void);

     void startFadeIn(float duration = 1.0f);
     void startFadeOut(float duration = 1.0f);
     void fade(float deltaTime);

 protected:
     float alpha;
     float currentDuration;
     float totalDuration;
     Ogre::TextureUnitState* textureUnit;
     Ogre::Pass* pass;
     Ogre::Overlay* overlay;

     enum FadeOption {
         FADE_NONE,
         FADE_IN,
         FADE_OUT,
     };

     FadeOption fadeOption;
 };
