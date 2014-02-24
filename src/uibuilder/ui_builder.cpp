#include "ui_builder.h"

#include <QMainWindow>
#include <QDebug>

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

    lua_register(state, "emptyFunction", &UiBuilder::emptyFunction);
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

int UiBuilder::emptyFunction(lua_State * /*state*/)
{
    qDebug() << "emptyFunction() called.";
    return 0;
}