#pragma once

#include <list>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "minecraft/Xbox.h"
#include "browser.h"
#include "async_result_util.h"

class GamepadMapperV8Handler;

class GamepadMapperBrowserClient : public BrowserClient {

private:
    static AsyncResult<bool> resultState;

public:
    static void OpenBrowser();

    GamepadMapperBrowserClient();

    ~GamepadMapperBrowserClient();

    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message) override;

    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

    void CallbackButtonPressed(int gamepadId, int button, bool pressed);

};

class GamepadMapperRenderHandler : public MyRenderProcessHandler {

private:
    CefRefPtr<GamepadMapperV8Handler> externalInterfaceHandler;

public:
    GamepadMapperRenderHandler(CefRefPtr<CefBrowser> browser) : MyRenderProcessHandler(browser) {}

    static const std::string Name;

    virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                  CefRefPtr<CefV8Context> context) override;

    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message) override;

};

class GamepadMapperV8Handler : public CefV8Handler {

private:
    GamepadMapperRenderHandler& handler;
    std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value>> gamepadButtonCallback;

public:

    void CallGamepadButtonCallback(int gamepadId, int button, bool pressed);

    GamepadMapperV8Handler(GamepadMapperRenderHandler& handler) : handler(handler) {}

    virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& args,
                         CefRefPtr<CefV8Value>& retval, CefString& exception) override;

private:
IMPLEMENT_REFCOUNTING(GamepadMapperV8Handler);
};