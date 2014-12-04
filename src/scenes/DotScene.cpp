#include "DotScene.h"
#include "Functions.h"

void DotScene::enter()
{
	LOG_INFO("Enter");
	componentMgr = std::shared_ptr<ComponentManager>(new ComponentManager());

	this->mapKeys();
	this->mapMouseButtons();

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(sceneName))
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(sceneName);

	this->parseDotScene(sceneName + ".scene", "Scenes");

	Ogre::RTShader::ShaderGenerator::initialize();
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	Ogre::RTShader::ShaderGenerator::getSingletonPtr()->setTargetLanguage("glsles");
#else
	Ogre::RTShader::ShaderGenerator::getSingletonPtr()->setTargetLanguage("glsl");
#endif
	matListener = std::shared_ptr<Ogre::ShaderGeneratorTechniqueResolverListener>(new Ogre::ShaderGeneratorTechniqueResolverListener());
	Ogre::MaterialManager::getSingleton().addListener(matListener.get());

	Ogre::RTShader::ShaderGenerator::getSingletonPtr()->addSceneManager(sceneMgr);

	sceneMgr->addRenderQueueListener(overlaySystem);

	LOG_DEBUG("Initialize systems");
	systems.push_back(std::shared_ptr<TransformSystem>(new TransformSystem()));
	systems.push_back(std::shared_ptr<CameraSystem>(new CameraSystem()));
	systems.push_back(std::shared_ptr<LightSystem>(new LightSystem()));
	systems.push_back(std::shared_ptr<System>(new MeshSystem()));
	systems.push_back(std::shared_ptr<System>(new RenderSystem()));
	systems.push_back(std::shared_ptr<AnimationSystem>(new AnimationSystem()));
	systems.push_back(std::shared_ptr<PhysicsSystem>(new PhysicsSystem()));
	systems.push_back(std::shared_ptr<ScriptingSystem>(new ScriptingSystem()));
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    systems.push_back(std::shared_ptr<GUISystem>(new GUISystem()));
	touchInput = std::shared_ptr<TouchInputSystem>(new TouchInputSystem());
	systems.push_back(touchInput);
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
    systems.push_back(std::shared_ptr<iOSInputSystem>(new iOSInputSystem()));
#else
    systems.push_back(std::shared_ptr<GUISystem>(new GUISystem()));
    systems.push_back(std::shared_ptr<InputSystem>(new InputSystem()));
#endif

	LOG_DEBUG("Start systems");
	for (std::shared_ptr<System> system : systems)
	{
		system->setComponentManager(componentMgr);
		system->setSceneManager(sceneMgr);
		system->setRenderWindow(window);
		system->setConfig(config);
		system->start();
	}
}

void DotScene::exit()
{
	for (std::shared_ptr<System> system : systems)
	{
		system->stop();
	}
	systems.clear();

    Ogre::RTShader::ShaderGenerator::getSingletonPtr()->removeSceneManager(sceneMgr);
            Ogre::MaterialManager::getSingleton().removeListener(matListener.get());

    if(sceneMgr)
    {
    	sceneMgr->destroyAllMovableObjects();
    	sceneMgr->destroyAllManualObjects();
    	Ogre::Root::getSingleton().destroySceneManager(sceneMgr);
    }
    LOG_INFO("Dot Scene exit")
}

void DotScene::update(float deltaTime)
{
	for (std::shared_ptr<System> system : systems)
	{
		system->update(deltaTime);
	}
}

void DotScene::parseDotScene(const std::string name, const std::string groupName)
{
	rapidxml::xml_document<> XMLDoc;
	rapidxml::xml_node<>* XMLRoot;

	try
	{
		Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(name, groupName);
		char* scene = strdup(stream->getAsString().c_str());
		XMLDoc.parse<0>(scene);

		// Grab the scene node
		XMLRoot = XMLDoc.first_node("scene");

		// Validate the File
		if(this->getAttrib(XMLRoot, "formatVersion", "") == "")
		{
			Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Error: Invalid .scene File. Missing <scene>" );
			delete scene;
			return;
		}

		// Process the scene
		processScene(XMLRoot);

		delete scene;
	}
	catch (...)
	{
		LOG_DEBUG("Couldn't find scene")
	}

	config->setScripts(scripts);
}

void DotScene::processScene(rapidxml::xml_node<>* XMLRoot)
{
	// Process the scene parameters
	Ogre::String version = getAttrib(XMLRoot, "formatVersion", "unknown");

	Ogre::String message = "[DotSceneLoader] Parsing dotScene file with version " + version;
	if(XMLRoot->first_attribute("ID"))
		message += ", id " + Ogre::String(XMLRoot->first_attribute("ID")->value());
	if(XMLRoot->first_attribute("sceneManager"))
		message += ", scene manager " + Ogre::String(XMLRoot->first_attribute("sceneManager")->value());
	if(XMLRoot->first_attribute("minOgreVersion"))
		message += ", min. Ogre version " + Ogre::String(XMLRoot->first_attribute("minOgreVersion")->value());
	if(XMLRoot->first_attribute("author"))
		message += ", author " + Ogre::String(XMLRoot->first_attribute("author")->value());

	Ogre::LogManager::getSingleton().logMessage(message);

	rapidxml::xml_node<>* element;

	sceneMgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, "CrateSceneMgr");
	sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	sceneMgr->setShadowColour(Ogre::ColourValue(0.1, 0.1, 0.1));
	sceneMgr->setShadowTextureSize(2048);
	attachNode = sceneMgr->getRootSceneNode();

    // Process environment
	element = XMLRoot->first_node("environment");
    if(element)
        processEnvironment(element);

    // Process camera (?)
	element = XMLRoot->first_node("camera");
	if(element)
		processCamera(element);

    // Process light
    element = XMLRoot->first_node("light");
    if(element)
        processLight(element);

    // Process nodes
    element = XMLRoot->first_node("nodes");
    if(element)
        processNodes(element);

//    // Process externals (?)
//    pElement = XMLRoot->first_node("externals");
//    if(pElement)
//        processExternals(pElement);
//
//    // Process userDataReference (?)
//    pElement = XMLRoot->first_node("userDataReference");
//    if(pElement)
//        processUserDataReference(pElement);
//
//    // Process octree (?)
//    pElement = XMLRoot->first_node("octree");
//    if(pElement)
//        processOctree(pElement);
//
//
//    // Process camera (?)
//    pElement = XMLRoot->first_node("camera");
//    if(pElement)
//        processCamera(pElement);
//
//    // Process terrain (?)
//    pElement = XMLRoot->first_node("terrain");
//    if(pElement)
//        processTerrain(pElement);
}

void DotScene::processEnvironment(rapidxml::xml_node<>* XMLNode)
{
	LOG_DEBUG("Process environment")

	rapidxml::xml_node<>* element;

	// Process skyDome (?)
	element = XMLNode->first_node("skyDome");
	if(element)
		processSkyDome(element);

	// Process scripts (?)
	element = XMLNode->first_node("scripts");
	if(element)
		processScripts(element);

//	// Process camera (?)
//	element = XMLNode->first_node("camera");
//	if(element)
//		processCamera(element);

//	// Process fog (?)
//	pElement = XMLNode->first_node("fog");
//	if(pElement)
//		processFog(pElement);
//
	// Process skyBox (?)
	element = XMLNode->first_node("skyBox");
	if(element)
		processSkyBox(element);

//	// Process skyDome (?)
//	pElement = XMLNode->first_node("skyDome");
//	if(pElement)
//		processSkyDome(pElement);
//
//	// Process skyPlane (?)
//	pElement = XMLNode->first_node("skyPlane");
//	if(pElement)
//		processSkyPlane(pElement);
//
//	// Process clipping (?)
//	pElement = XMLNode->first_node("clipping");
//	if(pElement)
//		processClipping(pElement);
//
//	// Process colourAmbient (?)
	element = XMLNode->first_node("colourAmbient");
	if(element)
		sceneMgr->setAmbientLight(parseColour(element));
//
//	// Process colourBackground (?)
//	//! @todo Set the background colour of all viewports (RenderWindow has to be provided then)
//	pElement = XMLNode->first_node("colourBackground");
//	if(pElement)
//		;//mSceneMgr->set(parseColour(pElement));
//
//	// Process userDataReference (?)
//	pElement = XMLNode->first_node("userDataReference");
//	if(pElement)
//		processUserDataReference(pElement);
}

void DotScene::processSkyBox(rapidxml::xml_node<>* XMLNode)
{
    // Process attributes
    Ogre::String material = getAttrib(XMLNode, "material", "BaseWhite");
    Ogre::Real distance = getAttribReal(XMLNode, "distance", 5000);
    bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);
    bool active = getAttribBool(XMLNode, "active", false);
    if(!active)
        return;

    rapidxml::xml_node<>* pElement;

    // Process rotation (?)
    Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
    pElement = XMLNode->first_node("rotation");
    if(pElement)
        rotation = parseQuaternion(pElement);

    // Setup the sky box
    sceneMgr->setSkyBox(true, material, distance, drawFirst, rotation, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}

// <skyDome active="true" material="Examples/CloudySky" />
void DotScene::processSkyDome(rapidxml::xml_node<>* XMLNode)
{
    // Process attributes
    Ogre::String material = XMLNode->first_attribute("material")->value();
    Ogre::Real curvature = getAttribReal(XMLNode, "curvature", 10);
    Ogre::Real tiling = getAttribReal(XMLNode, "tiling", 8);
    Ogre::Real distance = getAttribReal(XMLNode, "distance", 4000);
    bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);
    bool active = getAttribBool(XMLNode, "active", false);
    if(!active)
        return;

    rapidxml::xml_node<>* pElement;

    // Process rotation (?)
    Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
    pElement = XMLNode->first_node("rotation");
    if(pElement)
        rotation = parseQuaternion(pElement);

    // Setup the sky dome
    sceneMgr->setSkyDome(true, material, curvature, tiling, distance, drawFirst, rotation, 16, 16, -1, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}

void DotScene::processScripts(rapidxml::xml_node<>* XMLNode)
{
	LOG_DEBUG("Process scripts")

	rapidxml::xml_node<>* pElement;

	// Process script
	pElement = XMLNode->first_node("script");
	while(pElement)
	{
		std::string name = getAttrib(pElement, "name", "");
		std::string file = getAttrib(pElement, "file", "");
		scripts[name] = file;
		pElement = pElement->next_sibling("script");
	}
}

void DotScene::processCamera(rapidxml::xml_node<>* XMLNode)
{
	LOG_DEBUG("Process camera")

	// Process attributes
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String id = getAttrib(XMLNode, "id");
	Ogre::Real fov = getAttribReal(XMLNode, "fov", 45);
	Ogre::Real aspectRatio = getAttribReal(XMLNode, "aspectRatio", 1.3333);
	Ogre::String projectionType = getAttrib(XMLNode, "projectionType", "perspective");

	rapidxml::xml_node<>* element;

	// Process clipping
	element = XMLNode->first_node("clipping");
	Ogre::Real nearDist = getAttribReal(element, "near");
	Ogre::Real farDist =  getAttribReal(element, "far");

	// Process position
	element = XMLNode->first_node("position");
	Ogre::Vector3 position = parseVector3(element);

	// Process rotation
	element = XMLNode->first_node("rotation");
	Ogre::Quaternion rotation = parseQuaternion(element);

	int cameraObject = componentMgr->createEntity();
	componentMgr->addComponent(cameraObject, *(new TransformComponent(position, rotation, Ogre::Vector3(1,1,1))));
	componentMgr->addComponent(cameraObject, *(new CameraComponent(name, Ogre::ColourValue(1.0f,1.0f,1.0f), nearDist, farDist, Ogre::Vector3(0,0,0))));
	componentMgr->addComponent(cameraObject, *(new RenderComponent(name + "Node")));
}

void DotScene::processLight(rapidxml::xml_node<>* XMLNode)
{
	LOG_DEBUG("Process light")

    // Process attributes
    Ogre::String name = getAttrib(XMLNode, "name");
    Ogre::String id = getAttrib(XMLNode, "id");

    Ogre::String sValue = getAttrib(XMLNode, "type");
    Ogre::Light::LightTypes type;
    if(sValue == "point")
    	type = Ogre::Light::LT_POINT;
    else if(sValue == "directional")
    	type = Ogre::Light::LT_DIRECTIONAL;
    else if(sValue == "spot")
    	type = Ogre::Light::LT_SPOTLIGHT;
    else if(sValue == "radPoint")
    	type = Ogre::Light::LT_POINT;

    bool castShadows = getAttribBool(XMLNode, "castShadows", true);

    rapidxml::xml_node<>* pElement;

    // Process position (?)
    pElement = XMLNode->first_node("position");
    Ogre::Vector3 position = parseVector3(pElement);

//    // Process normal (?)
//    pElement = XMLNode->first_node("normal");
//    if(pElement)
//        pLight->setDirection(parseVector3(pElement));
//
//    pElement = XMLNode->first_node("directionVector");
//    if(pElement)
//    {
//        pLight->setDirection(parseVector3(pElement));
//        mLightDirection = parseVector3(pElement);
//    }

    // Process colourDiffuse (?)
    pElement = XMLNode->first_node("colourDiffuse");
    Ogre::ColourValue diffuseColor = parseColour(pElement);

    // Process colourSpecular (?)
    pElement = XMLNode->first_node("colourSpecular");
    Ogre::ColourValue specularColor = parseColour(pElement);

//    if(sValue != "directional")
//    {
//        // Process lightRange (?)
//        pElement = XMLNode->first_node("lightRange");
//        if(pElement)
//            processLightRange(pElement, pLight);
//
//        // Process lightAttenuation (?)
//        pElement = XMLNode->first_node("lightAttenuation");
//        if(pElement)
//            processLightAttenuation(pElement, pLight);
//    }

    int lightObject = componentMgr->createEntity();
	componentMgr->addComponent(lightObject, *(new TransformComponent(position)));
	componentMgr->addComponent(lightObject, *(new LightComponent(name, type, diffuseColor, specularColor, Ogre::Vector3(0,0,0))));
	componentMgr->addComponent(lightObject, *(new RenderComponent(name + "Node")));
}

void DotScene::processNodes(rapidxml::xml_node<>* XMLNode)
{
	LOG_DEBUG("Process nodes")

    rapidxml::xml_node<>* pElement;

    // Process node (*)
    pElement = XMLNode->first_node("node");
    while(pElement)
    {
        processNode(pElement);
        pElement = pElement->next_sibling("node");
    }

//    // Process position (?)
//    pElement = XMLNode->first_node("position");
//    if(pElement)
//    {
//        mAttachNode->setPosition(parseVector3(pElement));
//        mAttachNode->setInitialState();
//    }
//
//    // Process rotation (?)
//    pElement = XMLNode->first_node("rotation");
//    if(pElement)
//    {
//        mAttachNode->setOrientation(parseQuaternion(pElement));
//        mAttachNode->setInitialState();
//    }
//
//    // Process scale (?)
//    pElement = XMLNode->first_node("scale");
//    if(pElement)
//    {
//        mAttachNode->setScale(parseVector3(pElement));
//        mAttachNode->setInitialState();
//    }
}

void DotScene::processNode(rapidxml::xml_node<>* XMLNode)
{
	LOG_DEBUG("Process node")

	rapidxml::xml_node<>* element;

    // Construct the node's name
    Ogre::String name = getAttrib(XMLNode, "name");

    int renderObject = componentMgr->createEntity(name);

    // Process other attributes
    Ogre::String id = getAttrib(XMLNode, "id");
    bool isTarget = getAttribBool(XMLNode, "isTarget");

    Ogre::Vector3 position, scale;
    Ogre::Quaternion rotation;

    // Process position (?)
    element = XMLNode->first_node("position");
    if(element)
    {
        position = parseVector3(element);
    }

    // Process rotation (?)
    element = XMLNode->first_node("rotation");
    if(element)
    {
        rotation = parseQuaternion(element);
    }

    // Process scale (?)
    element = XMLNode->first_node("scale");
    if(element)
    {
        scale = parseVector3(element);
    }

    componentMgr->addComponent(renderObject, *(new TransformComponent(position, rotation, scale)));

//    // Process lookTarget (?)
//    pElement = XMLNode->first_node("lookTarget");
//    if(pElement)
//        processLookTarget(pElement, pNode);
//
//    // Process trackTarget (?)
//    pElement = XMLNode->first_node("trackTarget");
//    if(pElement)
//        processTrackTarget(pElement, pNode);

    // Process node
    element = XMLNode->first_node("node");
    while(element)
    {
        processNode(element);
        element = element->next_sibling("node");
    }

    // Process entity
    element = XMLNode->first_node("entity");
    while(element)
    {
        processEntity(element, renderObject);
        element = element->next_sibling("entity");
    }

	// Process plane
    element = XMLNode->first_node("plane");
	while(element)
	{
		processPlane(element, renderObject);
		element = element->next_sibling("plane");
	}


	componentMgr->addComponent(renderObject, *(new RenderComponent(name)));


	element = XMLNode->first_node("userData");
	if(element)
	{
		rapidxml::xml_node<>* element2 = element->first_node("property");
		while(element2)
		{
			if (getAttrib(element2, "name") == "Animation")
				componentMgr->addComponent(renderObject, *(new AnimationComponent(getAttrib(element2, "data"))));
			if (getAttrib(element2, "name") == "GUI")
			{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
				std::map<std::string, std::string> buttonPressedMap;
				std::istringstream stream(getAttrib(element2, "data"));
				std::vector<std::string> input{std::istream_iterator<std::string>{stream},
												std::istream_iterator<std::string>{}};
				for (int i = 1; i < input.size(); i=i+2)
				{
					std::string button = input[i];
					std::string event = input[i+1];
					buttonPressedMap.insert(std::make_pair(button, event));
				}
				componentMgr->addComponent(renderObject, *(new GUIComponent(input[0], buttonPressedMap)));
#endif
			}
			if (getAttrib(element2, "name") == "Input")
			{
				std::map<OIS::KeyCode, std::string> keyPressedMap;
				std::map<OIS::KeyCode, std::string> keyReleasedMap;
				std::map<OIS::MouseButtonID, std::string> mousePressedMap;
				std::map<OIS::MouseButtonID, std::string> mouseReleasedMap;
				std::string mouseMoved;
				std::string touchPressed;
				std::string touchReleased;
				std::string touchMoved;
				std::istringstream stream(getAttrib(element2, "data"));
				std::vector<std::string> input{std::istream_iterator<std::string>{stream},
												std::istream_iterator<std::string>{}};
				for (int i = 0; i < input.size(); i=i+3)
				{
					std::string key = input[i+1];
					std::string event = input[i+2];
					if (input[i] == "keyPressed")
						keyPressedMap.insert(std::make_pair(keys[key], event));
					else if (input[i] == "keyReleased")
						keyReleasedMap.insert(std::make_pair(keys[key], event));
					else if (input[i] == "mousePressed")
						mousePressedMap.insert(std::make_pair(mouseButtons[key], event));
					else if (input[i] == "mouseReleased")
						mouseReleasedMap.insert(std::make_pair(mouseButtons[key], event));
					else if (input[i] == "mouseMoved")
						mouseMoved = event;
					else if (input[i] == "touchPressed")
						touchPressed = event;
					else if (input[i] == "touchReleased")
						touchReleased = event;
					else if (input[i] == "touchMoved")
						touchMoved = event;
				}
				componentMgr->addComponent(renderObject, *(new InputComponent(keyPressedMap, keyReleasedMap, mousePressedMap, mouseReleasedMap, mouseMoved, touchPressed, touchReleased, touchMoved)));
			}
			if (getAttrib(element2, "name") == "Collider")
			{
				std::istringstream stream(getAttrib(element2, "data"));
				std::vector<std::string> collider{std::istream_iterator<std::string>{stream},
												std::istream_iterator<std::string>{}};

				if (collider.size() == 5)
				{
					float x = std::atof(collider[0].c_str());
					float y = std::atof(collider[1].c_str());
					float z = std::atof(collider[2].c_str());
					float mass = std::atof(collider[3].c_str());
					bool isTrigger = false;
					if (collider[4] == "true")
						isTrigger = true;
					componentMgr->addComponent(renderObject, *(new PhysicsComponent(btVector3(x,y,z), mass, btVector3(0,0,0), isTrigger)));
				}
				else if (collider.size() == 4)
				{
					float radius = std::atof(collider[0].c_str());
					float height = std::atof(collider[1].c_str());
					float mass = std::atof(collider[2].c_str());
					bool isTrigger = false;
					if (collider[3] == "true")
						isTrigger = true;
					componentMgr->addComponent(renderObject, *(new PhysicsComponent(radius, height, mass, btVector3(0,0,0), isTrigger)));
				}
				else if (collider.size() == 3)
				{
					float radius = std::atof(collider[0].c_str());
					float mass = std::atof(collider[1].c_str());
					bool isTrigger = false;
					if (collider[2] == "true")
						isTrigger = true;
					componentMgr->addComponent(renderObject, *(new PhysicsComponent(radius, mass, btVector3(0,0,0), isTrigger)));
				}
			}
			element2 = element2->next_sibling("property");
		}

	}
    // Process light (*)
    //pElement = XMLNode->first_node("light");
    //while(pElement)
    //{
    //    processLight(pElement, pNode);
    //    pElement = pElement->next_sibling("light");
    //}

//    // Process camera (*)
//    pElement = XMLNode->first_node("camera");
//    while(pElement)
//    {
//        processCamera(pElement, pNode);
//        pElement = pElement->next_sibling("camera");
//    }
//
//    // Process particleSystem (*)
//    pElement = XMLNode->first_node("particleSystem");
//    while(pElement)
//    {
//        processParticleSystem(pElement, pNode);
//        pElement = pElement->next_sibling("particleSystem");
//    }
//
//    // Process billboardSet (*)
//    pElement = XMLNode->first_node("billboardSet");
//    while(pElement)
//    {
//        processBillboardSet(pElement, pNode);
//        pElement = pElement->next_sibling("billboardSet");
//    }
//
}

void DotScene::processEntity(rapidxml::xml_node<>* XMLNode, int objectID)
{
	LOG_DEBUG("Process entity")

    // Process attributes
    Ogre::String name = getAttrib(XMLNode, "name");
    Ogre::String id = getAttrib(XMLNode, "id");
    Ogre::String meshFile = getAttrib(XMLNode, "meshFile");
    Ogre::String materialFile = getAttrib(XMLNode, "materialFile");
    bool isStatic = getAttribBool(XMLNode, "static", false);;
    bool castShadows = getAttribBool(XMLNode, "castShadows", true);

    componentMgr->addComponent(objectID, *(new MeshComponent(name, meshFile, castShadows)));

    // Process userDataReference
    rapidxml::xml_node<>* element = XMLNode->first_node("userData");
	if(element)
	{
		rapidxml::xml_node<>* element2 = element->first_node("property");
		while(element2)
		{
			if (getAttrib(element2, "name") == "Animation")
				componentMgr->addComponent(objectID, *(new AnimationComponent(getAttrib(element2, "data"))));
			if (getAttrib(element2, "name") == "GUI")
			{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
				std::map<std::string, std::string> buttonPressedMap;
				std::istringstream stream(getAttrib(element2, "data"));
				std::vector<std::string> input{std::istream_iterator<std::string>{stream},
												std::istream_iterator<std::string>{}};
				for (int i = 1; i < input.size(); i=i+2)
				{
					std::string button = input[i];
					std::string event = input[i+1];
					buttonPressedMap.insert(std::make_pair(button, event));
				}
				componentMgr->addComponent(objectID, *(new GUIComponent(input[0], buttonPressedMap)));
#endif
			}
			if (getAttrib(element2, "name") == "Input")
			{
				std::map<OIS::KeyCode, std::string> keyPressedMap;
				std::map<OIS::KeyCode, std::string> keyReleasedMap;
				std::map<OIS::MouseButtonID, std::string> mousePressedMap;
				std::map<OIS::MouseButtonID, std::string> mouseReleasedMap;
				std::string mouseMoved;
				std::string touchPressed;
				std::string touchReleased;
				std::string touchMoved;
				std::istringstream stream(getAttrib(element2, "data"));
				std::vector<std::string> input{std::istream_iterator<std::string>{stream},
												std::istream_iterator<std::string>{}};
				for (int i = 0; i < input.size(); i=i+3)
				{
					std::string key = input[i+1];
					std::string event = input[i+2];
					if (input[i] == "keyPressed")
						keyPressedMap.insert(std::make_pair(keys[key], event));
					else if (input[i] == "keyReleased")
						keyReleasedMap.insert(std::make_pair(keys[key], event));
					else if (input[i] == "mousePressed")
						mousePressedMap.insert(std::make_pair(mouseButtons[key], event));
					else if (input[i] == "mouseReleased")
						mouseReleasedMap.insert(std::make_pair(mouseButtons[key], event));
					else if (input[i] == "mouseMoved")
						mouseMoved = event;
					else if (input[i] == "touchPressed")
						touchPressed = event;
					else if (input[i] == "touchReleased")
						touchReleased = event;
					else if (input[i] == "touchMoved")
						touchMoved = event;
				}
				componentMgr->addComponent(objectID, *(new InputComponent(keyPressedMap, keyReleasedMap, mousePressedMap, mouseReleasedMap, mouseMoved, touchPressed, touchReleased, touchMoved)));
			}
			if (getAttrib(element2, "name") == "Collider")
			{
				std::istringstream stream(getAttrib(element2, "data"));
				std::vector<std::string> collider{std::istream_iterator<std::string>{stream},
												std::istream_iterator<std::string>{}};

				if (collider.size() == 5)
				{
					float x = std::atof(collider[0].c_str());
					float y = std::atof(collider[1].c_str());
					float z = std::atof(collider[2].c_str());
					float mass = std::atof(collider[3].c_str());
					bool isTrigger = false;
					if (collider[4] == "true")
						isTrigger = true;
					componentMgr->addComponent(objectID, *(new PhysicsComponent(btVector3(x,y,z), mass, btVector3(0,0,0), isTrigger)));
				}
				else if (collider.size() == 4)
				{
					float radius = std::atof(collider[0].c_str());
					float height = std::atof(collider[1].c_str());
					float mass = std::atof(collider[2].c_str());
					bool isTrigger = false;
					if (collider[3] == "true")
						isTrigger = true;
					componentMgr->addComponent(objectID, *(new PhysicsComponent(radius, height, mass, btVector3(0,0,0), isTrigger)));
				}
				else if (collider.size() == 3)
				{
					float radius = std::atof(collider[0].c_str());
					float mass = std::atof(collider[1].c_str());
					bool isTrigger = false;
					if (collider[2] == "true")
						isTrigger = true;
					componentMgr->addComponent(objectID, *(new PhysicsComponent(radius, mass, btVector3(0,0,0), isTrigger)));
				}
			}
			element2 = element2->next_sibling("property");
		}

	}
}

void DotScene::processPlane(rapidxml::xml_node<>* XMLNode, int objectID)
{
	LOG_DEBUG("Process plane")

    // Process attributes
    Ogre::String name = getAttrib(XMLNode, "name");
    Ogre::String id = getAttrib(XMLNode, "id");
    int width = getAttribReal(XMLNode, "width");
    int height = getAttribReal(XMLNode, "height");
    int xSegs = getAttribReal(XMLNode, "xSegments");
    int ySegs = getAttribReal(XMLNode, "ySegments");
    int distance = getAttribReal(XMLNode, "distance");
    int numTexCoordSets = getAttribReal(XMLNode, "numTexCoordSets");
    float uTile = getAttribReal(XMLNode, "uTile");
    float vTile = getAttribReal(XMLNode, "vTile");
    Ogre::String material = getAttrib(XMLNode, "material");
    if (material == "DefaultPlaneMaterial")
    	material = "BaseWhiteNoLighting";
    bool isStatic = getAttribBool(XMLNode, "static", false);;
    bool castShadows = getAttribBool(XMLNode, "castShadows", false);
    bool hasNormals = getAttribBool(XMLNode, "hasNormals", false);

    Ogre::Vector3 normal = Ogre::Vector3::UNIT_Y;
    Ogre::Vector3 upVector = Ogre::Vector3::UNIT_Z;

    // Process userData
    rapidxml::xml_node<>* element = XMLNode->first_node("normal");
	if(element)
	{
		normal = parseVector3(element);
	}
	element = XMLNode->first_node("upVector");
	if(element)
	{
		upVector = parseVector3(element);
	}

	componentMgr->addComponent(objectID, *(new MeshComponent(name, "plane", material, width, height, xSegs, ySegs, normal, upVector, castShadows, hasNormals, distance, numTexCoordSets, uTile, vTile)));
}

Ogre::String DotScene::getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, const Ogre::String &defaultValue)
{
    if(XMLNode->first_attribute(attrib.c_str()))
        return XMLNode->first_attribute(attrib.c_str())->value();
    else
        return defaultValue;
}

Ogre::Real DotScene::getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, Ogre::Real defaultValue)
{
    if(XMLNode->first_attribute(attrib.c_str()))
        return Ogre::StringConverter::parseReal(XMLNode->first_attribute(attrib.c_str())->value());
    else
        return defaultValue;
}

bool DotScene::getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, bool defaultValue)
{
    if(!XMLNode->first_attribute(attrib.c_str()))
        return defaultValue;

    if(Ogre::String(XMLNode->first_attribute(attrib.c_str())->value()) == "true")
        return true;

    return false;
}

Ogre::Vector3 DotScene::parseVector3(rapidxml::xml_node<>* XMLNode)
{
    return Ogre::Vector3(
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value()),
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value()),
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value())
    );
}

Ogre::Quaternion DotScene::parseQuaternion(rapidxml::xml_node<>* XMLNode)
{
    //! @todo Fix this crap!

    Ogre::Quaternion orientation;

    if(XMLNode->first_attribute("qx"))
    {
        orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qx")->value());
        orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qy")->value());
        orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qz")->value());
        orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qw")->value());
    }
    if(XMLNode->first_attribute("qw"))
    {
        orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qw")->value());
        orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qx")->value());
        orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qy")->value());
        orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qz")->value());
    }
    else if(XMLNode->first_attribute("axisX"))
    {
        Ogre::Vector3 axis;
        axis.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisX")->value());
        axis.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisY")->value());
        axis.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisZ")->value());
        Ogre::Real angle = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angle")->value());;
        orientation.FromAngleAxis(Ogre::Angle(angle), axis);
    }
    else if(XMLNode->first_attribute("angleX"))
    {
        Ogre::Vector3 axis;
        axis.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angleX")->value());
        axis.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angleY")->value());
        axis.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angleZ")->value());
        //orientation.FromAxes(&axis);
        //orientation.F
    }
    else if(XMLNode->first_attribute("x"))
    {
        orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value());
        orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value());
        orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value());
        orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("w")->value());
    }
    else if(XMLNode->first_attribute("w"))
    {
        orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("w")->value());
        orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value());
        orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value());
        orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value());
    }

    return orientation;
}

Ogre::ColourValue DotScene::parseColour(rapidxml::xml_node<>* XMLNode)
{
    return Ogre::ColourValue(
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("r")->value()),
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("g")->value()),
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("b")->value()),
        XMLNode->first_attribute("a") != NULL ? Ogre::StringConverter::parseReal(XMLNode->first_attribute("a")->value()) : 1
    );
}

void DotScene::mapKeys()
{
   keys["KC_UNASSIGNED"]= OIS::KC_UNASSIGNED;
   keys["KC_ESCAPE"]= OIS::KC_ESCAPE;
   keys["KC_1"]= OIS::KC_1;
   keys["KC_2"]= OIS::KC_2;
   keys["KC_3"]= OIS::KC_3;
   keys["KC_4"]= OIS::KC_4;
   keys["KC_5"]= OIS::KC_5;
   keys["KC_6"]= OIS::KC_6;
   keys["KC_7"]= OIS::KC_7;
   keys["KC_8"]= OIS::KC_8;
   keys["KC_9"]= OIS::KC_9;
   keys["KC_0"]= OIS::KC_0;
   keys["KC_MINUS"]= OIS::KC_MINUS;
   keys["KC_EQUALS"]= OIS::KC_EQUALS;
   keys["KC_BACK"]= OIS::KC_BACK;
   keys["KC_TAB"]= OIS::KC_TAB;
   keys["KC_Q"]= OIS::KC_Q;
   keys["KC_W"]= OIS::KC_W;
   keys["KC_E"]= OIS::KC_E;
   keys["KC_R"]= OIS::KC_R;
   keys["KC_T"]= OIS::KC_T;
   keys["KC_Y"]= OIS::KC_Y;
   keys["KC_U"]= OIS::KC_U;
   keys["KC_I"]= OIS::KC_I;
   keys["KC_O"]= OIS::KC_O;
   keys["KC_P"]= OIS::KC_P;
   keys["KC_LBRACKET"]= OIS::KC_LBRACKET;
   keys["KC_RBRACKET"]= OIS::KC_RBRACKET;
   keys["KC_RETURN"]= OIS::KC_RETURN;
   keys["KC_LCONTROL"]= OIS::KC_LCONTROL;
   keys["KC_A"]= OIS::KC_A;
   keys["KC_S"]= OIS::KC_S;
   keys["KC_D"]= OIS::KC_D;
   keys["KC_F"]= OIS::KC_F;
   keys["KC_G"]= OIS::KC_G;
   keys["KC_H"]= OIS::KC_H;
   keys["KC_J"]= OIS::KC_J;
   keys["KC_K"]= OIS::KC_K;
   keys["KC_L"]= OIS::KC_L;
   keys["KC_SEMICOLON"]= OIS::KC_SEMICOLON;
   keys["KC_APOSTROPHE"]= OIS::KC_APOSTROPHE;
   keys["KC_GRAVE"]= OIS::KC_GRAVE;
   keys["KC_LSHIFT"]= OIS::KC_LSHIFT;
   keys["KC_BACKSLASH"]= OIS::KC_BACKSLASH;
   keys["KC_Z"]= OIS::KC_Z;
   keys["KC_X"]= OIS::KC_X;
   keys["KC_C"]= OIS::KC_C;
   keys["KC_V"]= OIS::KC_V;
   keys["KC_B"]= OIS::KC_B;
   keys["KC_N"]= OIS::KC_N;
   keys["KC_M"]= OIS::KC_M;
   keys["KC_COMMA"]= OIS::KC_COMMA;
   keys["KC_PERIOD"]= OIS::KC_PERIOD;
   keys["KC_SLASH"]= OIS::KC_SLASH;
   keys["KC_RSHIFT"]= OIS::KC_RSHIFT;
   keys["KC_MULTIPLY"]= OIS::KC_MULTIPLY;
   keys["KC_LMENU"]= OIS::KC_LMENU;
   keys["KC_SPACE"]= OIS::KC_SPACE;
   keys["KC_CAPITAL"]= OIS::KC_CAPITAL;
   keys["KC_F1"]= OIS::KC_F1;
   keys["KC_F2"]= OIS::KC_F2;
   keys["KC_F3"]= OIS::KC_F3;
   keys["KC_F4"]= OIS::KC_F4;
   keys["KC_F5"]= OIS::KC_F5;
   keys["KC_F6"]= OIS::KC_F6;
   keys["KC_F7"]= OIS::KC_F7;
   keys["KC_F8"]= OIS::KC_F8;
   keys["KC_F9"]= OIS::KC_F9;
   keys["KC_F10"]= OIS::KC_F10;
   keys["KC_F11"]= OIS::KC_F11;
   keys["KC_F12"]= OIS::KC_F12;
   keys["KC_NUMLOCK"]= OIS::KC_NUMLOCK;
   keys["KC_SCROLL"]= OIS::KC_SCROLL;
   keys["KC_NUMPAD7"]= OIS::KC_NUMPAD7;
   keys["KC_NUMPAD8"]= OIS::KC_NUMPAD8;
   keys["KC_NUMPAD9"]= OIS::KC_NUMPAD9;
   keys["KC_SUBTRACT"]= OIS::KC_SUBTRACT;
   keys["KC_NUMPAD4"]= OIS::KC_NUMPAD4;
   keys["KC_NUMPAD5"]= OIS::KC_NUMPAD5;
   keys["KC_NUMPAD6"]= OIS::KC_NUMPAD6;
   keys["KC_ADD"]= OIS::KC_ADD;
   keys["KC_NUMPAD1"]= OIS::KC_NUMPAD1;
   keys["KC_NUMPAD2"]= OIS::KC_NUMPAD2;
   keys["KC_NUMPAD3"]= OIS::KC_NUMPAD3;
   keys["KC_NUMPAD0"]= OIS::KC_NUMPAD0;
   keys["KC_DECIMAL"]= OIS::KC_DECIMAL;
   keys["KC_RCONTROL"]= OIS::KC_RCONTROL;
   keys["KC_DIVIDE"]= OIS::KC_DIVIDE;
   keys["KC_SYSRQ"]= OIS::KC_SYSRQ;
   keys["KC_RMENU"]= OIS::KC_RMENU;
   keys["KC_PAUSE"]= OIS::KC_PAUSE;
   keys["KC_HOME"]= OIS::KC_HOME;
   keys["KC_UP"]= OIS::KC_UP;
   keys["KC_PGUP"]= OIS::KC_PGUP;
   keys["KC_LEFT"]= OIS::KC_LEFT;
   keys["KC_RIGHT"]= OIS::KC_RIGHT;
   keys["KC_END"]= OIS::KC_END;
   keys["KC_DOWN"]= OIS::KC_DOWN;
   keys["KC_PGDOWN"]= OIS::KC_PGDOWN;
   keys["KC_INSERT"]= OIS::KC_INSERT;
   keys["KC_DELETE"]= OIS::KC_DELETE;
   keys["KC_LWIN"]= OIS::KC_LWIN;
   keys["KC_RWIN"]= OIS::KC_RWIN;
   keys["KC_APPS"]= OIS::KC_APPS;
}

void DotScene::mapMouseButtons()
{
	mouseButtons["LEFT"] = OIS::MB_Left;
	mouseButtons["RIGHT"] = OIS::MB_Right;
	mouseButtons["MIDDLE"] = OIS::MB_Middle;
	//MB_Button3, MB_Button4,	MB_Button5, MB_Button6,	MB_Button7
}
