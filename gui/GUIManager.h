#pragma once

using namespace std;

typedef int TEXTUREID;

enum SpriteType
{
 ST_LEAF = 1,
 ST_COMPOSITE = 2,
 ST_ROOT = 3,
 ST_LABEL = 4,
 ST_ITEMLIST = 5,
 ST_ITEM = 6
};

const SPRITEID GIU_ROOT_ELEMENT = 0;

class GUIManager: public cSingleton<GUIManager>
{
public:
 //конструктор класса, принимающий ссылку на контекст устройства
	GUIManager();
 //деструктор класса
	~GUIManager(); 
 //метод задает область для отрисовки интерфейса
	void setArrea(CURCOORD width, CURCOORD height);

 //метод для добавления нового элемента в интерфейс
	bool addElement(SPRITEID parent, Sprite* elem);
 //метод для удаления элемента интерфейса по его ИД
	bool removeElement(SPRITEID elem);
 //метод для получения элемента интерфейса по его ИД
	Sprite* getElement(SPRITEID elem);

 //метод для установки собственного обработчика событий
	bool setHandler(SPRITEID elem, EventHandler* handler);
 //метод для получения обработчика событий по ИД элемента
    EventHandler* getHandler(SPRITEID elem);
 //метод для удаления обработчика событий
 //ВАЖНО! Метод не удаляет обработчик из памяти, а просто стирает его
 //стирает его из внутренней таблицы. Рекомендуеться использовать его
 //для удаления обработчиков событий созданных пользователями, если требуеться
 //ручное удаление таких обьектов
    void removeHandler(SPRITEID elem);

 //группа методов - перехватчиков событий
 //нажатие ЛКМ в пределах элемента (oLButtonDowm)
	virtual void onLButtonDown(CURCOORD x, CURCOORD y);
//отпуск ЛКМ в пределах элемента (onLButtonUp)
	virtual void onLButtonUp(CURCOORD x, CURCOORD y);
//одинарный клик ЛКМ по элементу (onLButtonClick)
	virtual void onLButtonClick(CURCOORD x, CURCOORD y);
//двойной клик по ЛКМ элементу (onLButtonDbClick)
	virtual void onLButtonDbClick(CURCOORD x, CURCOORD y);

//нажатие ПКМ в пределах элемента (oLButtonDowm)
	virtual void onRButtonDown(CURCOORD x, CURCOORD y);
//отпуск ПКМ в пределах элемента (onLButtonUp)
	virtual void onRButtonUp(CURCOORD x, CURCOORD y);
//одинарный клик ПКМ по элементу (onLButtonClick)
	virtual void onRButtonClick(CURCOORD x, CURCOORD y);
//двойной клик по ПКМ элементу (onLButtonDbClick)
	virtual void onRButtonDbClick(CURCOORD x, CURCOORD y);
//перемещение курсора мыши
	virtual void onCursorMove(CURCOORD x, CURCOORD y, CURSTATE state);
//прокрутка колеса мыши
	virtual void onCursorWheel(CURCOORD x, CURCOORD y, CURSTATE state, CURWHEEL delta);

//метод для отрисовки интерфейса
	void draw(DEVICECONTEXT& device);
private:
 //метод для очистик текущего сотояния обьекта
    void destroyAll();
 //таблица элементов
	std::map<SPRITEID, Sprite*> mElements;
 //таблица обработчиков событий
	std::map<SPRITEID, EventHandler*> mHandlers;
 //корневой элемент
	RootSprite mRoot;
 //элемент, находящийся по курсором пользователя
	Sprite* mBlured;
 //элемент, находящийся в состоянии фокуса
	Sprite* mFocused;

 //матрица вида
   D3DXMATRIX mView;   
 //матрица проекции
   D3DXMATRIX  mProject;
};

#define TheGUI GUIManager::getInstance()