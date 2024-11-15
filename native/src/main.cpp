// src/main.cpp
#include <JavaScriptCore/JavaScriptCore.h>
#include <iostream>
#include "namespace.h"

// JS wrapper function for MyNamespace::greet
JSValueRef GreetFunction(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject,
                         size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
    if (argumentCount < 1) {
        return JSValueMakeUndefined(ctx); // Error handling if no arguments
    }

    // Convert the argument to std::string
    JSStringRef jsStr = JSValueToStringCopy(ctx, arguments[0], exception);
    size_t strLength = JSStringGetMaximumUTF8CStringSize(jsStr);
    char* buffer = new char[strLength];
    JSStringGetUTF8CString(jsStr, buffer, strLength);
    std::string name(buffer);
    delete[] buffer;
    JSStringRelease(jsStr);

    // Call the C++ function
    std::string result = MyNamespace::greet(name);

    // Convert the result back to JS string and return
    JSStringRef resultStr = JSStringCreateWithUTF8CString(result.c_str());
    JSValueRef returnValue = JSValueMakeString(ctx, resultStr);
    JSStringRelease(resultStr);

    return returnValue;
}

// Function to register the namespace to JavaScript
void ExposeNamespaceToJS(JSContextRef context, JSObjectRef globalObject) {
    // Create a new JS object to represent MyNamespace
    JSObjectRef namespaceObject = JSObjectMake(context, nullptr, nullptr);

    // Create a JS function for 'greet'
    JSStringRef greetFunctionName = JSStringCreateWithUTF8CString("greet");
    JSObjectRef greetFunction = JSObjectMakeFunctionWithCallback(context, greetFunctionName, GreetFunction);

    // Attach the 'greet' function to the namespace
    JSObjectSetProperty(context, namespaceObject, greetFunctionName, greetFunction, kJSPropertyAttributeNone, nullptr);
    JSStringRelease(greetFunctionName);

    // Add the namespace object to the global context
    JSStringRef namespaceName = JSStringCreateWithUTF8CString("MyNamespace");
    JSObjectSetProperty(context, globalObject, namespaceName, namespaceObject, kJSPropertyAttributeNone, nullptr);
    JSStringRelease(namespaceName);
}

int main() {
    // Create a JS context and global object
    JSContextGroupRef contextGroup = JSContextGroupCreate();
    JSContextRef context = JSContextCreate(contextGroup);
    JSObjectRef globalObject = JSContextGetGlobalObject(context);

    // Expose the C++ namespace and its function to JavaScript
    ExposeNamespaceToJS(context, globalObject);

    // Run some JavaScript code
    const char* script = R"(
        var result = MyNamespace.greet('World');
        console.log(result);  // Expected: "Hello, World"
    )";

    JSStringRef scriptJS = JSStringCreateWithUTF8CString(script);
    JSEvaluateScript(context, scriptJS, nullptr, nullptr, 0, nullptr);
    JSStringRelease(scriptJS);

    // Clean up
    JSContextGroupRelease(contextGroup);

    return 0;
}
