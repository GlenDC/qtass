#include "ui_builder.h"

#include <QMainWindow>
#include <QDebug>
#include <QDockWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#define DECLARE_GLOBAL(x) lua_pushnumber(state, x); lua_setglobal(state, #x);

UiBuilder::UiBuilder()
    :
    targetWindow(nullptr),
    state(nullptr)
{
}

UiBuilder::UiBuilder(QMainWindow * window)
    :
    targetWindow(window),
    state(nullptr)
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

    lua_register(state, "create", &UiBuilder::create);

    lua_pushlightuserdata(state, this);
    lua_setglobal(state,"this");

    DECLARE_GLOBAL(DOCK);
    DECLARE_GLOBAL(FRAME);
    DECLARE_GLOBAL(BUTTON);
    DECLARE_GLOBAL(LABEL);
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

int UiBuilder::create(lua_State * state)
{
    UiBuilder & uiBuilder = getUiBuilder(state);
    WIDGET_TYPE widget_type = static_cast<WIDGET_TYPE>(lua_tonumber(state,1));
    const char *title = lua_tostring(state,2);
    QWidget *parent = static_cast<QWidget*>(lua_touserdata(state,3));
    QWidget *result;

    switch(widget_type)
    {
        case DOCK:
        {
            QDockWidget *dock = new QDockWidget(uiBuilder.targetWindow);
            result = new QWidget(dock);
            QVBoxLayout *layout = new QVBoxLayout(dock);

            dock->setTitleBarWidget(new QLabel(title));
            dock->setWidget(result);
            result->setLayout(layout);

            uiBuilder.targetWindow->addDockWidget(Qt::LeftDockWidgetArea, dock);

            if(parent)
            {
                qDebug() << "UiBuilder: DOCK <" << title << "> cannot have parent.";
                parent = nullptr;
            }
        }
        break;

        case FRAME:
        {
            QFrame * frame = new QFrame();

            frame->setFrameShape(QFrame::Panel);
            frame->setLayout(new QVBoxLayout(frame));

            result = frame;
        }
        break;

        case BUTTON:
        {
            result = new QPushButton(title);
        }
        break;

        case LABEL:
        {
            result = new QLabel(title);
        }
        break;
    }

    if(parent)
    {
        parent->layout()->addWidget(result);
    }

    lua_pushlightuserdata(state, result);

    return 1;
}
