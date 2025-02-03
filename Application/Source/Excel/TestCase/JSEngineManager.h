#ifndef JSENGINEMANAGER_H
#define JSENGINEMANAGER_H

#include <QJSEngine>

class JSEngineManager {
public:
    static JSEngineManager& instance() {
        static JSEngineManager instance;
        return instance;
    }

    QJSEngine& getEngine() {
        return engine;
    }

private:
    JSEngineManager() = default;  // private constructor
    ~JSEngineManager() = default;
    JSEngineManager(const JSEngineManager&) = delete;
    JSEngineManager& operator=(const JSEngineManager&) = delete;

    QJSEngine engine;
};

#endif  // JSENGINEMANAGER_H