#include "stdafx.h"
#include "../structs/singleton.h"
#include "GUICommon.h"

using namespace std;

//наведение курсора на элемент (onBlur)
void 
EventHandler::onBlur(Sprite* sender,
					 CURCOORD x, 
					 CURCOORD y, 
					 CURSTATE state)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onBlur(pParent, x, y, state);
} 

//потеря курсора элементом (onHower)
void 
EventHandler::onHower(Sprite* sender,
					  CURCOORD x,
					  CURCOORD y,
					  CURSTATE state)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onHower(pParent, x, y, state);
}

//захват фокуса элементом (клик по элементу)(onSetFocus)
void 
EventHandler::onSetFocus(Sprite* sender,
						 CURCOORD x, 
						 CURCOORD y, 
						 CURSTATE state, 
						 Sprite* oldFocus)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onSetFocus(pParent, x, y, state, oldFocus);
}

//потеря фокуса элементом (клик по другому элементу) (onKillFocus)
void 
EventHandler::onKillFocus(Sprite* sender,
						  CURCOORD x, 
						  CURCOORD y, 
						  CURSTATE state, 
						  Sprite* newFocus)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onKillFocus(pParent, x, y, state, newFocus);
}

//нажатие ЛКМ в пределах элемента (oLButtonDowm)
void 
EventHandler::onLButtonDown(Sprite* sender, 
							CURCOORD x, 
							CURCOORD y)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onLButtonDown(pParent, x, y);
}

//отпуск ЛКМ в пределах элемента (onLButtonUp)
void 
EventHandler::onLButtonUp(Sprite* sender, 
						  CURCOORD x, 
						  CURCOORD y)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onLButtonUp(pParent, x, y);
}

//одинарный клик ЛКМ по элементу (onLButtonClick)
void 
EventHandler::onLButtonClick(Sprite* sender, 
							 CURCOORD x, 
							 CURCOORD y)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onLButtonClick(pParent, x, y);
}

//двойной клик по ЛКМ элементу (onLButtonDbClick)
void 
EventHandler::onLButtonDbClick(Sprite* sender, 
							   CURCOORD x, 
							   CURCOORD y)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onLButtonDbClick(pParent, x, y);
}

//нажатие ПКМ в пределах элемента (oLButtonDowm)
void 
EventHandler::onRButtonDown(Sprite* sender, 
							CURCOORD x, 
							CURCOORD y)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onRButtonDown(pParent, x, y);
}

//отпуск ПКМ в пределах элемента (onLButtonUp)
void 
EventHandler::onRButtonUp(Sprite* sender, 
						  CURCOORD x, 
						  CURCOORD y)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onRButtonUp(pParent, x, y);
}

//одинарный клик ПКМ по элементу (onLButtonClick)
void 
EventHandler::onRButtonClick(Sprite* sender, 
							 CURCOORD x, 
							 CURCOORD y)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onRButtonClick(pParent, x, y);
}

//двойной клик по ПКМ элементу (onLButtonDbClick)
void 
EventHandler::onRButtonDbClick(Sprite* sender, 
							   CURCOORD x, 
							   CURCOORD y)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onRButtonDbClick(pParent, x, y);
}

//перемещение курсора мыши
void 
EventHandler::onCursorMove(Sprite* sender,
						   CURCOORD x, 
						   CURCOORD y, 
						   CURSTATE state)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onCursorMove(pParent, x, y, state);
}

//прокрутка колеса мыши
void 
EventHandler::onCursorWheel(Sprite* sender,
							CURCOORD x, 
							CURCOORD y, 
							CURSTATE state, 
							CURWHEEL delta)
{
 //получаем указатель на обьект ГУИ
 GUIManager* gui = getGUI();
 //получаем указатель на родительский элемент
 Sprite* pParent = sender->getParent();
 //если укаитель не найден выходим
 if(pParent==NULL)
	 return;
 //получаем ИД элемента
 SPRITEID id = pParent->getID();
 //запрашиваем ГУИ на выдачу обработчика для элемента
 EventHandler* handler = gui->getHandler(id);
 //если не найден выходим
 if(handler==NULL)
	 return;
 //вызваем соответсвующий метод обработчика
 handler->onCursorWheel(pParent, x, y, state, delta); 
}


