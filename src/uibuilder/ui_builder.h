#ifndef UI_BUILDER_H
#define UI_BUILDER_H

#include <QString>
class QMainWindow;
class QWidget;
class lua_State;

class UiBuilder
{
public:
    UiBuilder();
    UiBuilder(QMainWindow *target);
    ~UiBuilder();

    void init();
    void executeFile(const QString & filePath);

private:
    enum WIDGET_TYPE
    {
        DOCK,
        FRAME,
        BUTTON,
        LABEL
    };

    static UiBuilder & getUiBuilder(lua_State * state);
    static QWidget & getWidget(lua_State * state, const int argn);

    static int create(lua_State * state);

    QMainWindow * targetWindow;
    lua_State * state;
};

#endif
