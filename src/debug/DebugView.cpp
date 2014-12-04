#include "DebugView.h"

DebugView::DebugView() :
	textBox(nullptr),
	info(nullptr),
	screenshotButton(nullptr),
	polygonModeButton(nullptr),
	offset(20, 20)
{
	const std::string layer = "Statistic";
	if (!MyGUI::LayerManager::getInstance().isExist(layer))
		return;

	textBox = MyGUI::Gui::getInstance().createWidget<MyGUI::EditBox>("WordWrapEmpty", MyGUI::IntCoord(), MyGUI::Align::Default, layer, "Console");
	textBox->setTextColour(MyGUI::Colour::White);
	textBox->setTextShadow(true);
	textBox->setFontName("DebugFont");

	info = MyGUI::Gui::getInstance().createWidget<MyGUI::TextBox>("TextBox_Dark", MyGUI::IntCoord(), MyGUI::Align::Default, layer);
	info->setTextColour(MyGUI::Colour::White);
	info->setTextShadow(true);
	info->setFontName("DebugFont");

	screenshotButton = MyGUI::Gui::getInstance().createWidget<MyGUI::Button>("Button_Dark", MyGUI::IntCoord(), MyGUI::Align::Default, "Info");
	screenshotButton->eventMouseButtonClick += MyGUI::newDelegate(this, &DebugView::screenshotButtonPressed);
	screenshotButton->setTextColour(MyGUI::Colour::White);
	screenshotButton->setTextShadow(true);
	screenshotButton->setCaption("Take Screenshot");
	screenshotButton->setFontName("DebugFont");

	polygonModeButton = MyGUI::Gui::getInstance().createWidget<MyGUI::Button>("Button_Dark", MyGUI::IntCoord(), MyGUI::Align::Default, "Info");
	polygonModeButton->eventMouseButtonClick += MyGUI::newDelegate(this, &DebugView::polygonModeButtonPressed);
	polygonModeButton->setTextColour(MyGUI::Colour::White);
	polygonModeButton->setTextShadow(true);
	polygonModeButton->setCaption("Change Polygon Mode");
	polygonModeButton->setFontName("DebugFont");

	physicsButton = MyGUI::Gui::getInstance().createWidget<MyGUI::Button>("Button_Dark", MyGUI::IntCoord(), MyGUI::Align::Default, "Info");
	physicsButton->eventMouseButtonClick += MyGUI::newDelegate(this, &DebugView::physicsButtonPressed);
	physicsButton->setTextColour(MyGUI::Colour::White);
	physicsButton->setTextShadow(true);
	physicsButton->setCaption("Show Physics Debug");
	physicsButton->setFontName("DebugFont");
}

DebugView::~DebugView()
{
	if (textBox != nullptr)
	{
		MyGUI::Gui::getInstance().destroyChildWidget(textBox);
		textBox = nullptr;
	}
	if (info != nullptr)
	{
		MyGUI::Gui::getInstance().destroyChildWidget(info);
		info = nullptr;
	}
	if (screenshotButton != nullptr)
	{
		MyGUI::Gui::getInstance().destroyChildWidget(screenshotButton);
		screenshotButton = nullptr;
	}
	if (polygonModeButton != nullptr)
	{
		MyGUI::Gui::getInstance().destroyChildWidget(polygonModeButton);
		polygonModeButton = nullptr;
	}
	if (physicsButton != nullptr)
	{
		MyGUI::Gui::getInstance().destroyChildWidget(physicsButton);
		physicsButton = nullptr;
	}
}

void DebugView::update()
{
	if (textBox != nullptr)
	{
		const MyGUI::IntSize& size_view = MyGUI::RenderManager::getInstance().getViewSize();
		MyGUI::IntCoord coord(offset.left, size_view.height - 150 - offset.top, 400, 150);
		if (coord != textBox->getCoord())
			textBox->setCoord(coord);
		textBox->setVScrollPosition(textBox->getTextLength()-1);
	}

	if (info != nullptr)
	{
		std::ostringstream stream;
		for (MyGUI::VectorStringPairs::iterator iter = params.begin(); iter != params.end(); ++iter)
		{
			if (iter != params.begin()) stream << "\n";
			stream << iter->first << " : " << iter->second;
		}

		info->setCaption(stream.str());

		MyGUI::ISubWidgetText* text = info->getSubWidgetText();
		if (text != nullptr)
		{
			const MyGUI::IntSize& size = text->getTextSize() + info->getSize() - text->getSize();
			const MyGUI::IntSize& size_view = MyGUI::RenderManager::getInstance().getViewSize();
			MyGUI::IntCoord coord(size_view.width - size.width - offset.left, size_view.height - size.height - offset.top, size.width, size.height);
			if (coord != info->getCoord())
				info->setCoord(coord);
		}
	}

	int buttonHeight = 0;
	if (polygonModeButton != nullptr)
	{
		MyGUI::ISubWidgetText* text = polygonModeButton->getSubWidgetText();
		if (text != nullptr)
		{
			const MyGUI::IntSize& size = text->getTextSize() + polygonModeButton->getSize() - text->getSize();
			buttonHeight = size.height + offset.top;
			const MyGUI::IntSize& size_view = MyGUI::RenderManager::getInstance().getViewSize();
			MyGUI::IntCoord coord(offset.left, offset.top, size.width + offset.left, size.height + offset.top);
			if (coord != polygonModeButton->getCoord())
				polygonModeButton->setCoord(coord);
		}
	}

	if (screenshotButton != nullptr)
	{
		MyGUI::ISubWidgetText* text = screenshotButton->getSubWidgetText();
		if (text != nullptr)
		{
			const MyGUI::IntSize& size = text->getTextSize() + screenshotButton->getSize() - text->getSize();
			const MyGUI::IntSize& size_view = MyGUI::RenderManager::getInstance().getViewSize();
			MyGUI::IntCoord coord(offset.left, offset.top + buttonHeight + 10, size.width + offset.left, size.height + offset.top);
			if (coord != screenshotButton->getCoord())
				screenshotButton->setCoord(coord);
		}
	}

	if (physicsButton != nullptr)
	{
		MyGUI::ISubWidgetText* text = physicsButton->getSubWidgetText();
		if (text != nullptr)
		{
			const MyGUI::IntSize& size = text->getTextSize() + physicsButton->getSize() - text->getSize();
			const MyGUI::IntSize& size_view = MyGUI::RenderManager::getInstance().getViewSize();
			MyGUI::IntCoord coord(offset.left, offset.top + 2 * (buttonHeight + 10), size.width + offset.left, size.height + offset.top);
			if (coord != physicsButton->getCoord())
				physicsButton->setCoord(coord);
		}
	}
}

void DebugView::change(const std::string& key, const std::string& value)
{
	for (MyGUI::VectorStringPairs::iterator iter = params.begin(); iter != params.end(); ++iter)
	{
		if (iter->first == key)
		{
			iter->second = value;
			return;
		}
	}
	params.push_back(std::make_pair(key, value));
}

void DebugView::remove(const std::string& key)
{
	for (MyGUI::VectorStringPairs::iterator iter = params.begin(); iter != params.end(); ++iter)
	{
		if (iter->first == key)
		{
			params.erase(iter);
			return;
		}
	}
}

void DebugView::clear()
{
	params.clear();
}

void DebugView::clear(const std::string& key)
{
	for (MyGUI::VectorStringPairs::iterator iter = params.begin(); iter != params.end(); ++iter)
	{
		if (iter->first == key)
		{
			params.erase(iter);
			return;
		}
	}
}

void DebugView::setVisible(bool value)
{
	if (textBox != nullptr)
		textBox->setVisible(value);
	if (info != nullptr)
		info->setVisible(value);
	if (screenshotButton != nullptr)
		screenshotButton->setVisible(value);
	if (polygonModeButton != nullptr)
		polygonModeButton->setVisible(value);
	if (physicsButton != nullptr)
		physicsButton->setVisible(value);
}

bool DebugView::getVisible()
{
	if (textBox != nullptr)
		return textBox->getVisible();
	if (info != nullptr)
		return info->getVisible();
	return false;
}

void DebugView::setOffset(const MyGUI::IntPoint& value)
{
	offset = value;
}

void DebugView::screenshotButtonPressed(MyGUI::WidgetPtr sender)
{
	GameEventPtr event(new SystemEvent("TakeScreenshot"));
	safeQueueEvent(event);
}

void DebugView::polygonModeButtonPressed(MyGUI::WidgetPtr sender)
{
	GameEventPtr event(new SystemEvent("ChangePolygonMode"));
	safeQueueEvent(event);
}

void DebugView::physicsButtonPressed(MyGUI::WidgetPtr sender)
{
	GameEventPtr event(new SystemEvent("ShowDebugPhysics"));
	safeQueueEvent(event);
}
