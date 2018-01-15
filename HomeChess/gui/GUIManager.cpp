#include "stdafx.h"
#include "../structs/singleton.h"
#include "GUICommon.h"

using namespace std;


//конструктор класса, принимающий ссылку на контекст устройства
GUIManager::GUIManager():
cSingleton(*this), mBlured(NULL), mFocused(NULL)
{
 mElements[GIU_ROOT_ELEMENT] = (Sprite*)&mRoot;
 mHandlers[GIU_ROOT_ELEMENT] = new EventHandler();
 mHandlers[GIU_ROOT_ELEMENT]->setGUI(this);
}

//деструктор класса
GUIManager::~GUIManager()
{
 destroyAll();
}

void 
GUIManager::destroyAll()
{
 //удалить обработчики событий
 for(map<SPRITEID, EventHandler*>::iterator it = mHandlers.begin();
		it != mHandlers.end(); ++it)
 {
	 EventHandler* handler = it->second;
	 delete handler;
 }
 

 //очистить карту обработчиков
 mHandlers.clear();
 //очистить карту элементов
 mElements.clear();
 
}
/*
 метод для установки области отрисвоки интерфейса,
 вызываеться при каждом изменении размеров окна
*/
void 
GUIManager::setArrea(CURCOORD width, CURCOORD height)
{
 D3DXVECTOR3 eye((FLOAT)(width / 2), (FLOAT)(height / 2), -1.0f);
 D3DXVECTOR3 at((FLOAT)(width / 2), (FLOAT)(height / 2), 0.0f);
 D3DXVECTOR3 up(0.0f, -1.0f, 0.0f);

 D3DXMatrixLookAtRH(&mView, &eye, &at, &up);
 D3DXMatrixOrthoRH(&mProject, (FLOAT)width, (FLOAT)height, 0.0f, 10.0f);

 mRoot.setSize((SCOORD)width, (SCOORD)height);
}



//метод для добавления нового элемента в интерфейс
bool 
GUIManager::addElement(SPRITEID parent, Sprite* elem)
{
 //проверяем есть ли соотвествующий родительский узел 
 map<SPRITEID, Sprite*>::iterator parent_it = mElements.find(parent);
 //если нет - выходим
 if(parent_it == mElements.end())
  return false;
 //проверяем нет ли в таблице элементов элемента с ИД elem
 map<SPRITEID, Sprite*>::iterator elem_it = mElements.find(elem->getID());
 if(elem_it == mElements.end())
 {
  //если нет  добавляем элемент в таблицу
  SPRITEID key = elem->getID();
  mElements[key] = elem;

  setHandler(key, new EventHandler());
 }else
 {
  //если есть удалим элемент из списка потомков его родителя
  Sprite* pParentSprite = elem->getParent();
  pParentSprite->removeChild(elem);
 }
 //присоединяем элемент к соответсвующему узлу
 mElements[parent]->addChild(elem);
 return true;
}

//метод для удаления элемента интерфейса по его ИД
bool 
GUIManager::removeElement(SPRITEID elem)
{
 map<SPRITEID, Sprite*>::iterator elem_it = mElements.find(elem);
 if(elem_it != mElements.end())
 {
	 Sprite* pElement = mElements[elem];
	 Sprite* pParent = pElement->getParent();
	 //удалим его из родительского элемента
	 pParent->removeChild(pElement);
	 //а также из таблицы элементов
	 mElements.erase(elem_it);
     //обнулим ссылки на текущий подсвеченый элемент
     //или элемент, находящийся в фокусе, если они
     //они в данный момент указывают на удаляемый обьект
     //чтобы менеджер не вызвал методы удаленных из памяти обьектов
     if(mBlured == pElement)
     {
         mBlured = NULL;
     }
     if(mFocused == pElement)
     {
         mFocused = NULL;
     }
	 //из памяти его сотрем
	 delete pElement;
	 //удалить обрабочик событий для элемента
	 map<SPRITEID, EventHandler*>::iterator handle_it = mHandlers.find(elem);
	 if(handle_it != mHandlers.end())
	 {
		 EventHandler* handler = handle_it->second;
         if(handler != NULL)
         {
             delete handler;
         }
		 mHandlers.erase(handle_it);
	 }
 }
 return false;
}

//метод для получения элемента интерфейса по его ИД
Sprite* 
GUIManager::getElement(SPRITEID elem)
{
 map<SPRITEID, Sprite*>::iterator elem_it = mElements.find(elem);
 if(elem_it != mElements.end())
	 return mElements[elem];
 return NULL;
}

//метод для установки собственного обработчика событий
bool 
GUIManager::setHandler(SPRITEID elem, EventHandler* handler)
{
 map<SPRITEID, EventHandler*>::iterator handle_it = mHandlers.find(elem);
 if(handle_it != mHandlers.end())
 {
	 EventHandler* oldHandler = handle_it->second;
	 delete oldHandler;	 
 }

 mHandlers[elem] = handler;
 mHandlers[elem]->setGUI(this);
 
 return true;
}

//метод для получения обработчика событий по ИД элемента
EventHandler* 
GUIManager::getHandler(SPRITEID elem)
{
 map<SPRITEID, EventHandler*>::iterator handle_it = mHandlers.find(elem);
 if(handle_it != mHandlers.end())
	 return handle_it->second;
 else
	 return NULL;
}

void 
GUIManager::removeHandler(SPRITEID elem)
{
    map<SPRITEID, EventHandler*>::iterator handle_it = mHandlers.find(elem);
    if(handle_it != mHandlers.end())
    {
        mHandlers.erase(handle_it);
    }
}
 
 //группа методов - перехватчиков событий
 
//нажатие ЛКМ в пределах элемента (oLButtonDowm)
void 
GUIManager::onLButtonDown(CURCOORD x, CURCOORD y)
{
 //получаем ссылку на элемент, в пределах которого кликнули
 Sprite* newFocus =	mRoot.findSprite(x, y);
 //если такого элемента нет, значит определнно ошибка - выходим
 if(NULL == newFocus)
	 return;
 //получаем обработчик событий соотвествующий элементу
 EventHandler* NFhandler = getHandler(newFocus->getID());
 //если нет такого, значит что-то не так
 if(NULL == NFhandler)
	 return;
 //если фокус получил новый элемент
 if(mFocused != newFocus)
 {
  //вызовем событие "потеря фокуса" для старого элемент, если таковой есть
  if(NULL != mFocused)
  {
	  EventHandler* OFhandler = getHandler(mFocused->getID());
	  if(NULL!=OFhandler)
		  OFhandler->onKillFocus(mFocused, x, y, CS_LBUTTONDOWN, newFocus);
  }//if(NULL != mFocused)
  //вызовем событие "получение фокуса" для нового элемента
  NFhandler->onSetFocus(newFocus, x, y, CS_LBUTTONDOWN, mFocused);
  //сохраним ссылку на элемент в кеше
  mFocused = newFocus;
 }
 //вызовем событие "нажатие левой кнопки мыши" для элемента
 NFhandler->onLButtonDown(newFocus, x, y);
}

//отпуск ЛКМ в пределах элемента (onLButtonUp)
void 
GUIManager::onLButtonUp(CURCOORD x, CURCOORD y)
{
 //получим ссылку на элемент в пределах которого находиться курсор
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //получим обработчик события для него
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //если обработчик есть, то вызовем событие
 handler->onLButtonUp(control, x, y);
}

//одинарный клик ЛКМ по элементу (onLButtonClick)
void 
GUIManager::onLButtonClick(CURCOORD x, CURCOORD y)
{
 //получим ссылку на элемент в пределах которого находиться курсор
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //получим обработчик события для него
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //если обработчик есть, то вызовем событие
 handler->onLButtonClick(control, x, y);
}

//двойной клик по ЛКМ элементу (onLButtonDbClick)
void 
GUIManager::onLButtonDbClick(CURCOORD x, CURCOORD y)
{
 //получим ссылку на элемент в пределах которого находиться курсор
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //получим обработчик события для него
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //если обработчик есть, то вызовем событие
 handler->onLButtonDbClick(control, x, y);
}

//нажатие ПКМ в пределах элемента (oLButtonDowm)
//будем считать, что нажатие правой кнопки мыши не ведет к
//передаче фокуса элементу
void 
GUIManager::onRButtonDown(CURCOORD x, CURCOORD y)
{
 //получим ссылку на элемент в пределах которого находиться курсор
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //получим обработчик события для него
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //если обработчик есть, то вызовем событие
 handler->onRButtonDown(control, x, y);
}

//отпуск ПКМ в пределах элемента (onLButtonUp)
void 
GUIManager::onRButtonUp(CURCOORD x, CURCOORD y)
{
 //получим ссылку на элемент в пределах которого находиться курсор
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //получим обработчик события для него
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //если обработчик есть, то вызовем событие
 handler->onRButtonUp(control, x, y);
}

//одинарный клик ПКМ по элементу (onLButtonClick)
void 
GUIManager::onRButtonClick(CURCOORD x, CURCOORD y)
{
 //получим ссылку на элемент в пределах которого находиться курсор
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //получим обработчик события для него
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //если обработчик есть, то вызовем событие
 handler->onRButtonClick(control, x, y);
}

//двойной клик по ПКМ элементу (onLButtonDbClick)
void 
GUIManager::onRButtonDbClick(CURCOORD x, CURCOORD y)
{
 //получим ссылку на элемент в пределах которого находиться курсор
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //получим обработчик события для него
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //если обработчик есть, то вызовем событие
 handler->onRButtonDbClick(control, x, y);
}

//перемещение курсора мыши
void 
GUIManager::onCursorMove(CURCOORD x, CURCOORD y, CURSTATE state)
{
 //получаем элемент в пределах которого находиться курсор
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //получим обработчик событий, соотвествующий элементу
 EventHandler* curHandler = getHandler(control->getID());
 if(NULL == curHandler)
	 return;
 //если курсор находиться над другим элементом
 if(control != mBlured)
 {
  //вызовем событие "потеря курсора" для предыдущего элемента
  if(NULL != mBlured)
  {
	  EventHandler* oldHandler = getHandler(mBlured->getID());
	  if(NULL != oldHandler)
		  oldHandler->onHower(mBlured, x, y, state);
  }
  //вызовем событие "наведение курсора" для нового элемента
  curHandler->onBlur(control, x, y, state);
  mBlured = control;
 }
 //вызовем событие "перемещение курсора" для элемента, над которым находиться курсор
 curHandler->onCursorMove(control, x, y, state);
 //вызовем событие "перемещение курсора" для элемента находящегося в фокусе
 if((NULL != mFocused)&&(mFocused != control))
 {
	 EventHandler* focHandler = getHandler(mFocused->getID());
	 if(NULL != focHandler)
		 focHandler->onCursorMove(mFocused, x, y, state);
 }
}

//прокрутка колеса мыши
//вызываеться для элемента над которым находиться курсор
//и для элемента находящегося в фокусе
void 
GUIManager::onCursorWheel(CURCOORD x, CURCOORD y, CURSTATE state, CURWHEEL delta)
{
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 EventHandler* curHandler = getHandler(control->getID());
 if(NULL == curHandler)
	 return;

 curHandler->onCursorWheel(control, x, y, state, delta);
 if((NULL != mFocused)&&(mFocused != control))
 {
	 EventHandler* focHandler = getHandler(mFocused->getID());
	 if(NULL != focHandler)
		 focHandler->onCursorWheel(mFocused, x, y, state, delta);
 }
}

//метод для отрисовки интерфейса
void 
GUIManager::draw(DEVICECONTEXT& device)
{
 /*
 if(NULL == mDevice)
	 return;
 */
 //включаем смешивание цветов по альфа-каналу
 device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
 device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
 device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
 //оключаем работу с освещенностью
 device->SetRenderState(D3DRS_LIGHTING, FALSE);
 //отключаем отсечение
 device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
 //устанавливаем матрицы
 device->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&mView);
 device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&mProject);
 //устанавливаем формат вершин
 device->SetFVF(D3DFVF_XYZ|D3DFVF_TEX1);
 //рисуем...
 mRoot.draw(device);
}
