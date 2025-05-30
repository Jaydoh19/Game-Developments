#include "TextureHolder.h"
//include the assert feature
#include <assert.h>
TextureHolder* TextureHolder::m_s_Instance = nullptr;
TextureHolder::TextureHolder() {

	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

Texture& TextureHolder::GetTexture(string const& filename) {
	//Get a reference to m_Textures using m_s_Instance
	auto& m = m_s_Instance -> m_Textures;
	//auto = map<string, Texture>
	//Create an iterator to hold key value pair and search for required kvp using filename passed in

	auto keyValuePair = m.find(filename);
	//auto is equivalent of map<string, Texture>::iterator

	//Did we find match?
	if (keyValuePair != m.end()) {

		//Yes? return texture
		return keyValuePair->second;
	}
	else {
		//File name not found
		//Create a new value pair using the filename
		auto& texture = m[filename];
		//Load the texture from file
		texture.loadFromFile(filename);

		//return texture to calling code
		return texture;
	}
 }
