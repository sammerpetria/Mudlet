#include <QTest>
extern "C" {
#if defined(INCLUDE_VERSIONED_LUA_HEADERS)
#include <lua5.1/lauxlib.h>
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#else
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#endif
}

#include "TEvent.h" // needed for completeness

class TLuaInterpreter {
public:
    TLuaInterpreter() {
        L = luaL_newstate();
        luaL_openlibs(L);
    }
    ~TLuaInterpreter() { lua_close(L); }

    lua_State* getLuaGlobalState() { return L; }
    void signalMXPEvent(const QString& type, const QMap<QString, QString>& attrs,
                        const QStringList& actions, const QString& caption)
    {
        lua_getglobal(L, "mxp");
        if (!lua_istable(L, -1)) {
            lua_newtable(L);
            lua_setglobal(L, "mxp");
            lua_getglobal(L, "mxp");
            if (!lua_istable(L, -1)) {
                return;
            }
        }

        lua_newtable(L);
        QByteArray typeLower = type.toUtf8().toLower();
        lua_setfield(L, -2, typeLower.constData());
        lua_getfield(L, -1, typeLower.constData());
        if (!lua_istable(L, -1)) {
            return;
        }

        for (auto it = attrs.cbegin(); it != attrs.cend(); ++it) {
            lua_pushstring(L, it.value().toUtf8().constData());
            lua_setfield(L, -2, it.key().toUtf8().toLower().constData());
        }

        lua_newtable(L);
        lua_setfield(L, -2, "actions");
        lua_getfield(L, -1, "actions");
        for (int i = 0; i < actions.size(); ++i) {
            lua_pushstring(L, actions[i].toUtf8().constData());
            lua_rawseti(L, -2, i + 1);
        }
        lua_pop(L, 1);

        lua_pushstring(L, caption.toUtf8().constData());
        lua_setfield(L, -2, "caption");

        lua_pop(L, lua_gettop(L));
    }

private:
    lua_State* L = nullptr;
};

class TLuaMXPEventTest : public QObject {
    Q_OBJECT
private slots:
    void testCaptionPlacement() {
        TLuaInterpreter interp;
        lua_State* L = interp.getLuaGlobalState();

        QMap<QString, QString> attrs;
        QStringList actions;
        actions << "act1";
        interp.signalMXPEvent("send", attrs, actions, "cap");

        lua_getglobal(L, "mxp");
        lua_getfield(L, -1, "send");
        lua_getfield(L, -1, "caption");
        QCOMPARE(QString(lua_tostring(L, -1)), QString("cap"));
        lua_pop(L, 1);
        lua_getfield(L, -1, "actions");
        lua_getfield(L, -1, "caption");
        QVERIFY(lua_isnil(L, -1));
    }
};

#include "TLuaMXPEventTest.moc"
QTEST_MAIN(TLuaMXPEventTest)
