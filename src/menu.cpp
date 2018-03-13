#include "menu.h"

MenuItem::MenuItem(std::string text)
{
    _text = text;
}

MenuItem::~MenuItem()
{

}

void MenuItem::paint(Paint &blackPaint, Paint &redPaint, int x0, int y0, bool isSelected)
{
    blackPaint.DrawStringAt(x0 + 3, y0, _text.c_str(), &Font16, 1);
    if(isSelected) {
        redPaint.DrawRectangle(x0, y0 -4, x0 + 164, y0 + 18, 1);
    }
}

void MenuItem::addSubmenu(SubMenu* subMenu)
{
    _childSubMenu = subMenu;
    _action = nullptr;
}

void MenuItem::addAction(void (*action)())
{
    _action = action;
    _childSubMenu = nullptr;
}

SubMenu::SubMenu(std::string text)
{
    _text = text;
}

SubMenu::SubMenu(std::string text, SubMenu* parentMenu)
{
    _text = text;

    _firstItem = new MenuItem("Back");
    _firstItem->addSubmenu(parentMenu);

    _currentItem = _firstItem;
}

SubMenu::~SubMenu()
{

}

void SubMenu::paint(Paint &blackPaint, Paint &redPaint, int x0, int y0)
{
    MenuItem* item = _firstItem;
    int yOffset = 30;

    redPaint.DrawStringAt(x0, y0, _text.c_str(), &Font24, 1);

    while(item != nullptr) {
        item->paint(blackPaint, redPaint, x0, y0 + yOffset, item == _currentItem);
        yOffset += 25;
        item = item->_nextItem;
    }
}

void SubMenu::selectNextItem()
{
    if(_currentItem != nullptr) {
        if(_currentItem->_nextItem != nullptr) {
            _currentItem = _currentItem->_nextItem;
        }
    }
}

void SubMenu::selectPreviousItem()
{
    if(_currentItem != nullptr) {
        if(_currentItem->_previousItem != nullptr) {
            _currentItem = _currentItem->_previousItem;
        }
    }
}

void SubMenu::addItem(MenuItem* item)
{
    if(_firstItem != nullptr) {
        item->_nextItem = _firstItem;
        _firstItem->_previousItem = item;
    }

    _firstItem = item;
    _currentItem = _firstItem;
}

Menu::Menu()
{
    MenuItem* item;

    _currentSubMenu = new SubMenu("Main menu");

    item = new MenuItem("Action item");
    _currentSubMenu->addItem(item);

    item = new MenuItem("Option 2");
    item->addSubmenu(new SubMenu("Option 2", _currentSubMenu));
    _currentSubMenu->addItem(item);

    item = new MenuItem("Option 1");
    item->addSubmenu(new SubMenu("Option 1", _currentSubMenu));
    _currentSubMenu->addItem(item);
}

Menu::~Menu()
{

}

void Menu::paint(Paint &blackPaint, Paint &redPaint, int x0, int y0)
{
    _currentSubMenu->paint(blackPaint, redPaint, x0, y0);
}

void Menu::action()
{
    if(_currentSubMenu->_currentItem != nullptr) {
        if(_currentSubMenu->_currentItem->_childSubMenu != nullptr) {
            _currentSubMenu = _currentSubMenu->_currentItem->_childSubMenu;
        }

        else if(_currentSubMenu->_currentItem->_action != nullptr) {
            _currentSubMenu->_currentItem->_action();
        }
    }
}

void Menu::previous()
{
    _currentSubMenu->selectPreviousItem();
}

void Menu::next()
{
    _currentSubMenu->selectNextItem();
}
