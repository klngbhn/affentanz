#pragma once

#include <MyGUI.h>

#include "IEventManager.h"

/*
 * This class draws the DebugView on the screen, including
 * output console, infos about frames-per-second and buttons
 * for taking screenshots or showing collision shapes.
 */
class DebugView
{
public:

	DebugView();
	~DebugView();

	void update();
	void change(const std::string& key, const std::string& value);
	void remove(const std::string& key);
	void clear();
	void clear(const std::string& key);
	void setVisible(bool value);
	bool getVisible();
	void setOffset(const MyGUI::IntPoint& value);

private:
	void screenshotButtonPressed(MyGUI::WidgetPtr sender);
	void polygonModeButtonPressed(MyGUI::WidgetPtr sender);
	void physicsButtonPressed(MyGUI::WidgetPtr sender);

	MyGUI::Button* screenshotButton;
	MyGUI::Button* polygonModeButton;
	MyGUI::Button* physicsButton;
	MyGUI::EditBox* textBox;
	MyGUI::TextBox* info;
	MyGUI::VectorStringPairs params;
	MyGUI::IntPoint offset;
};
