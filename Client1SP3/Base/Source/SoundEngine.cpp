#include "SoundEngine.h"
#include <iostream>

using namespace std;

// Constructor
CSoundEngine::CSoundEngine()
	: theSoundEngine(NULL)
{
}

// Destructor
CSoundEngine::~CSoundEngine()
{
	// Clear out the sound map
	soundMap.clear();

	// Delete the sound engine
	if (theSoundEngine)
	{
		delete theSoundEngine;
		theSoundEngine = NULL;
	}
}

// Init this class and it will create the Sound Engine
bool CSoundEngine::Init(void)
{
	// Create the sound engine
	theSoundEngine = createIrrKlangDevice();
	if (!theSoundEngine)
		return false;	// error starting up the sound engine




	return true;
}

void CSoundEngine::setVolume(float value)
{
	theSoundEngine->setSoundVolume(value);
}

// Get the handler to the sound engine
ISoundEngine* CSoundEngine::GetSoundEngine(void)
{
	if (theSoundEngine == NULL)
	{
		if (Init() == false)
		{
			cout << "SoundEngine::GetSoundEngine() - The Sound Engine has not been initialised yet." << endl;
			return NULL;
		}
	}

	return theSoundEngine;
}

// Add a sound to this library
void CSoundEngine::AddSound(const std::string& _soundIndex, const std::string& _soundSource)
{
	if (theSoundEngine == NULL)
	{
		if (Init() == false)
		{
			cout << "SoundEngine::AddSound() - The Sound Engine has not been initialised yet." << endl;
			return;
		}
	}

	// Clean up first if there is an existing goodie with the same name
	RemoveSound(_soundIndex);

	// Add the mesh now
	soundMap[_soundIndex] = _soundSource;
}

// Get a sound from this map 
std::string CSoundEngine::GetSound(const std::string& _soundIndex)
{
	if (theSoundEngine == NULL)
	{
		if (Init() == false)
		{
			cout << "SoundEngine::GetSound() - The Sound Engine has not been initialised yet." << endl;
			return std::string();
		}
	}

	if (soundMap.count(_soundIndex) != 0)
	{
		return soundMap[_soundIndex];
	}

	return std::string();
}

// Remove a sound from this map
bool CSoundEngine::RemoveSound(const std::string& _soundIndex)
{
	if (theSoundEngine == NULL)
	{
		if (Init() == false)
		{
			cout << "SoundEngine::RemoveSound() - The Sound Engine has not been initialised yet." << endl;
			return NULL;
		}
	}

	std::string aSoundName = GetSound(_soundIndex);
	if (!aSoundName.empty())
	{
		soundMap.erase(_soundIndex);
		return true;
	}
	return false;
}

// Play a sound from this map
void CSoundEngine::PlayASound2D(const std::string& _soundIndex)
{
	std::string aSound = GetSound(_soundIndex);

	if (!theSoundEngine->isCurrentlyPlaying(aSound.c_str()))
	{
		// Play a sound
		theSoundEngine->play2D(aSound.c_str(), false, false);
	}
}

void CSoundEngine::PlayASound3D(const std::string& _soundIndex, Vector3 position)
{
	std::string aSound = GetSound(_soundIndex);

	if (!theSoundEngine->isCurrentlyPlaying(aSound.c_str()))
	{
		// Play a sound
		irrklang::ISound* sound = theSoundEngine->play3D(aSound.c_str(), irrklangVec3(position), false, false, true);
		sound->setVolume(100.0f);
	}
}

void CSoundEngine::setListenerPosition(Vector3 position , Vector3 direction , Vector3 up)
{
	theSoundEngine->setListenerPosition(irrklangVec3(position), irrklangVec3(direction));
}

irrklang::vec3df CSoundEngine::irrklangVec3(const Vector3 & position)
{
	return irrklang::vec3df(position.x, position.y, -position.z);
}
