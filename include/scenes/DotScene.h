#pragma once

#include "Scene.h"
#include "RTShaderHelper.h"
#include "rapidxml.hpp"

/*
 * This class can read in a DotScene file and creates a scene with
 * entities and components.
 * It is based on the RapidXML DotScene Loader from the Ogre Wiki:
 * http://www.ogre3d.org/tikiwiki/tiki-index.php?page=RapidXML+Dotscene+Loader
 */
class DotScene : public Scene {
public:
	DotScene(const std::string sceneName) :sceneName(sceneName) { }
	void enter();
	void exit();

	void update(float deltaTime);
private:
	void mapKeys();
	void mapMouseButtons();
	void parseDotScene(const std::string name, const std::string groupName);
	void processScene(rapidxml::xml_node<>* XMLRoot);
	void processEnvironment(rapidxml::xml_node<>* XMLNode);
	void processSkyBox(rapidxml::xml_node<>* XMLNode);
	void processSkyDome(rapidxml::xml_node<>* XMLNode);
	void processScripts(rapidxml::xml_node<>* XMLNode);
	void processCamera(rapidxml::xml_node<>* XMLNode);
	void processLight(rapidxml::xml_node<>* XMLNode);
	void processNodes(rapidxml::xml_node<>* XMLNode);
	void processNode(rapidxml::xml_node<>* XMLNode);
	void processEntity(rapidxml::xml_node<>* XMLNode, int objectID);
	void processPlane(rapidxml::xml_node<>* XMLNode, int objectID);

	Ogre::String getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, const Ogre::String &defaultValue = "");
	Ogre::Real getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, Ogre::Real defaultValue = 0);
	bool getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, bool defaultValue = false);

	Ogre::Vector3 parseVector3(rapidxml::xml_node<>* XMLNode);
	Ogre::Quaternion parseQuaternion(rapidxml::xml_node<>* XMLNode);
	Ogre::ColourValue parseColour(rapidxml::xml_node<>* XMLNode);

	std::shared_ptr<Ogre::ShaderGeneratorTechniqueResolverListener> matListener;
	Ogre::SceneNode* attachNode;
	const std::string sceneName;
	Ogre::NameValuePairList scripts;
	std::map<std::string, OIS::KeyCode> keys;
	std::map<std::string, OIS::MouseButtonID> mouseButtons;
};
