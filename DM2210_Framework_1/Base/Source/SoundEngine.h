#pragma once

#include "SingletonTemplate.h"
#include "../../irrklang/include/irrKlang.h"
using namespace irrklang;
#include <map>
#include "Vector3.h"

class CSoundEngine : public Singleton<CSoundEngine>
{
protected:
	// Destructor
	ISoundEngine* theSoundEngine;

	// The library map of all the sounds created
	std::map<std::string, std::string> soundMap;

public:
	// Constructor
	CSoundEngine();
	virtual ~CSoundEngine();

	// Init this class and it will create the Sound Engine
	bool Init(void);

	// Get the handler to the sound engine
	ISoundEngine* GetSoundEngine(void);
	void setVolume(float value);

	// Add a sound to this library
	void AddSound(const std::string& _soundIndex, const std::string& _soundSource);
	// Get a sound from this map 
	std::string GetSound(const std::string& _soundIndex);
	// Remove a sound from this map
	bool RemoveSound(const std::string& _soundIndex);
	// Play a sound from this map
	void PlayASound3D(const std::string& _soundIndex , Vector3 position);
	void PlayASound2D(const std::string& _soundIndex);

	void setListenerPosition(Vector3 position, Vector3 direction, Vector3 up);

private:

	irrklang::vec3df irrklangVec3(const Vector3& position);

};

