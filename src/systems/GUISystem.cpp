#include "GUISystem.h"


void GUISystem::start()
{
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ShowDebugView"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ChangeWidgetCaption"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ShowWidget"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("HideWidget"));

	debugView = std::shared_ptr<DebugView>(new DebugView());
	debugView->setVisible(false);

	LOG_DEBUG("GUI system started");
}

void GUISystem::update(float deltaTime)
{
	MyGUI::MapString statistic = this->getStatistics();
	for (std::pair<std::string, std::string> stringPair : statistic)
		debugView->change(stringPair.first, stringPair.second);
	debugView->update();
}

void GUISystem::stop()
{
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ShowDebugView"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ChangeWidgetCaption"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ShowWidget"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("HideWidget"));

	std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(GUIComponent));
	for (std::shared_ptr<Component> component : components)
	{
		MyGUI::LayoutManager::getInstance().unloadLayout(dynamic_cast<GUIComponent*>(component.get())->guiLayout);
	}

	LOG_DEBUG("GUI system stopped");
}

bool GUISystem::handleEvent(const GameEvent& event)
{
	if (event.getEventType() == EventType("ComponentAdded"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(GUIComponent))
			initGUIComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(GUIComponent)));
	}
	else if (event.getEventType() == EventType("EntityDestroyed"))
	{
		const EntityEvent& entityEvent = dynamic_cast<const EntityEvent&>(event);
		if (componentMgr->getComponent(entityEvent.entityID, &typeid(GUIComponent)))
			removeGUIComponent(componentMgr->getComponent(entityEvent.entityID, &typeid(GUIComponent)));
	}
	else if (event.getEventType() == EventType("RemoveComponent"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(GUIComponent))
			removeGUIComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(GUIComponent)));
	}
	else if (event.getEventType() == EventType("ShowDebugView"))
	{
		bool visible = debugView->getVisible();
		debugView->setVisible(!visible);
	}
	else if (event.getEventType() == EventType("ChangeWidgetCaption"))
	{
		const GUIEvent& guiEvent = dynamic_cast<const GUIEvent&>(event);
		try
		{
			if (guiEvent.widgetName != "Console")
			{
				MyGUI::TextBox* textBox = MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::TextBox>(guiEvent.widgetName);
				textBox->setCaption(guiEvent.value);
			}
		}
		catch (...)
		{
			LOG_ERROR("Couldn't find widget with name: ");
		}
	}
	else if (event.getEventType() == EventType("ShowWidget"))
	{
		const GUIEvent& guiEvent = dynamic_cast<const GUIEvent&>(event);
		try
		{
			MyGUI::Gui::getInstancePtr()->findWidgetT(guiEvent.widgetName)->setVisible(true);
		}
		catch (...)
		{
			LOG_ERROR("Couldn't find widget with name: ");
		}
	}
	else if (event.getEventType() == EventType("HideWidget"))
	{
		const GUIEvent& guiEvent = dynamic_cast<const GUIEvent&>(event);
		try
		{
			MyGUI::Gui::getInstancePtr()->findWidgetT(guiEvent.widgetName)->setVisible(false);
		}
		catch (...)
		{
			LOG_ERROR("Couldn't find widget with name: ");
		}
	}

	return true;
}

void GUISystem::initGUIComponent(std::shared_ptr<Component> component)
{
	GUIComponent* guiComponent = dynamic_cast<GUIComponent*>(component.get());

	guiComponent->guiLayout = MyGUI::LayoutManager::getInstance().loadLayout(guiComponent->layoutName);
	MyGUI::LayerManager::getInstancePtr()->resizeView(MyGUI::RenderManager::getInstancePtr()->getViewSize());

	guiComponent->initDelegates();
}

void GUISystem::removeGUIComponent(std::shared_ptr<Component> component)
{
	GUIComponent* guiComponent = dynamic_cast<GUIComponent*>(component.get());

	MyGUI::LayoutManager::getInstance().unloadLayout(guiComponent->guiLayout);

	componentMgr->removeComponent(guiComponent->entityID, &typeid(GUIComponent));
}

MyGUI::MapString GUISystem::getStatistics()
{
	MyGUI::MapString result;

	try
	{
		const Ogre::RenderTarget::FrameStats& stats = window->getStatistics();
		result["FPS"] = MyGUI::utility::toString(stats.lastFPS) + " (" + MyGUI::utility::toString(stats.avgFPS) + ")";
		result["B/W FPS"] = MyGUI::utility::toString(stats.bestFPS) + " / " + MyGUI::utility::toString(stats.worstFPS);
		result["B/W Time"] = MyGUI::utility::toString(stats.bestFrameTime) + " / " + MyGUI::utility::toString(stats.worstFrameTime);
		result["Triangles"] = MyGUI::utility::toString(stats.triangleCount);
		result["Batch"] = MyGUI::utility::toString(stats.batchCount);
		result["Batch Gui"] = MyGUI::utility::toString(MyGUI::OgreRenderManager::getInstance().getBatchCount());
	}
	catch (...)
	{
		LOG_ERROR("Couldn't get statistics");
	}

	return result;
}
