#include "ui_builder.h"

#include <QMainWindow>
#include <QDebug>
#include <QDockWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

UiBuilder::UiBuilder()
    :
    targetWindow(NULL),
    state(NULL)
{
}

UiBuilder::UiBuilder(QMainWindow * window)
    :
    targetWindow(window),
    state(NULL)
{
}

UiBuilder::~UiBuilder()
{
    if(state)
    {
        lua_close(state);
    }
}

void UiBuilder::init()
{
    state = luaL_newstate();

    luaopen_io(state);
    luaopen_base(state);
    luaopen_table(state);
    luaopen_string(state);
    luaopen_math(state);

    lua_register(state, "createPanel", &UiBuilder::createPanel);
    lua_register(state, "addButton", &UiBuilder::addButton);

    lua_pushlightuserdata(state, this);
    lua_setglobal(state,"this");
}

void UiBuilder::executeFile(const QString & filePath)
{
    qDebug() << "UiBuilder: Executing" << filePath;

    int s = luaL_loadfile(state, filePath.toLatin1().constData());

    if(s==0)
    {
        s = lua_pcall(state, 0, LUA_MULTRET, 0);
    }
}

UiBuilder & UiBuilder::getUiBuilder(lua_State * state)
{
    lua_getglobal(state,"this");
    UiBuilder *uiBuilder = static_cast<UiBuilder*>(lua_touserdata(state, -1));
    lua_pop(state, 1);

    return *uiBuilder;
}

QWidget & UiBuilder::getWidget(lua_State * state, const int argn)
{
    return * static_cast<QWidget*>(lua_touserdata(state, argn));
}

int UiBuilder::createPanel(lua_State * state)
{
    UiBuilder & uiBuilder = getUiBuilder(state);
    const char * title = lua_tostring(state,1);

    QDockWidget *dock = new QDockWidget(uiBuilder.targetWindow);
    QWidget *panel = new QWidget(dock);
    QVBoxLayout *layout = new QVBoxLayout(dock);

    dock->setWidget(panel);
    panel->setLayout(layout);

    layout->addWidget(new QLabel(title));

    uiBuilder.targetWindow->addDockWidget(Qt::LeftDockWidgetArea, dock);

    lua_pushlightuserdata(state, panel);

    return 1;
}

int UiBuilder::addButton(lua_State * state)
{
    QWidget & panel = getWidget(state, 1);
    const char * title = lua_tostring(state,2);

    panel.layout()->addWidget(new QPushButton(title));

    return 0;
}