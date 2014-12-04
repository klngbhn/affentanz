#include "Fader.h"

Fader::Fader(const char* overlayName, const char* materialName)
{
	try
	{
		fadeOption = FadeOption::FADE_NONE;
		alpha = 0.0;

		// Get the material by name
		Ogre::ResourcePtr resptr = Ogre::MaterialManager::getSingleton().getByName(materialName);
		if (!resptr.isNull())
		{
			Ogre::Material * mat = dynamic_cast<Ogre::Material*>(resptr.getPointer());

			Ogre::Technique *tech = mat->getTechnique(0);
			pass = tech->getPass(0);
			textureUnit = pass->getTextureUnitState(0);
		}

		// Get the overlay
		overlay = Ogre::OverlayManager::getSingleton().getByName(overlayName);
		if (overlay)
			overlay->hide();

	}
	catch(Ogre::Exception e)
	{
		LOG_ERROR(e.getFullDescription().c_str())
	}
	catch(...)
	{
		LOG_ERROR("An unknown exception has occured while setting up the fader.  Scene fading will not be supported.")
	}
}

Fader::~Fader(void)
{
}

void Fader::startFadeIn(float duration)
{
	if(duration < 0)
		duration = -duration;
	if(duration < 0.000001f)
		duration = 1.0f;

	alpha = 1.0f;
	totalDuration = duration;
	currentDuration = duration;
	fadeOption = FadeOption::FADE_IN;

	if (overlay)
		overlay->show();
}

void Fader::startFadeOut(float duration)
{
	if(duration < 0)
		duration = -duration;
	if(duration < 0.000001f)
		duration = 1.0f;

	alpha = 0.0f;
	totalDuration = duration;
	currentDuration = 0.0f;
	fadeOption = FadeOption::FADE_OUT;

	if (overlay)
		overlay->show();
}

void Fader::fade(float deltaTime)
{
	if(fadeOption != FadeOption::FADE_NONE && textureUnit)
	{
		// Set the alpha value of the overlay
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
		Ogre::GpuProgramParametersSharedPtr constants = pass->getFragmentProgramParameters();
		constants->setNamedConstant("ALPHA", Ogre::Vector4(1,1,1,alpha));
#else
		textureUnit->setAlphaOperation(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_TEXTURE, alpha);
#endif

		// If fading in, decrease the alpha until it reaches 0.0
		if(fadeOption == FadeOption::FADE_IN)
		{
			currentDuration -= deltaTime;
			alpha = currentDuration / totalDuration;
			if(alpha < 0.0f)
			{
				if (overlay)
					overlay->hide();
				fadeOption = FadeOption::FADE_NONE;
				GameEventPtr event(new SystemEvent("FadedIn"));
				safeQueueEvent(event);
			}
		}

		// If fading out, increase the alpha until it reaches 1.0
		else if(fadeOption == FadeOption::FADE_OUT)
		{
			currentDuration += deltaTime;
			alpha = currentDuration / totalDuration;
			if(alpha > 1.0f)
			{
				fadeOption = FadeOption::FADE_NONE;
				GameEventPtr event(new SystemEvent("FadedOut"));
				safeQueueEvent(event);
			}
		}
	}
}
