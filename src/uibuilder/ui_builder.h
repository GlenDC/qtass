#ifndef UI_BUILDER_H
#define UI_BUILDER_H

#include <QString>
class QMainWindow;
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
    QMainWindow * targetWindow;
    lua_State * state;
};

#endif
