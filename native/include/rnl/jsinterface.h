#ifndef RNL_JS_NATIVE_INTERFACE_H
#define RNL_JS_NATIVE_INTERFACE_H

#include <jsc/jsc.h>
#include <string>
#include <stdio.h>

class JSNativeInterface {
private:
  /* data */
  JSCContext* ctx;
  JSCValue* interfaceObject;
  JSCValue* globalObject;
  std::string name;

protected:
  JSCContext* getContext();

  void setupComplete();
  void registerProperty(std::string, JSCValue*);

public:
  JSNativeInterface(JSCContext*, std::string);
  ~JSNativeInterface();

  virtual void init() {};
  virtual void deinit() {};
};

JSNativeInterface::JSNativeInterface(JSCContext* ctx, std::string name) {
  this->ctx = ctx;
  this->name = name;
  this->globalObject = jsc_context_get_global_object(ctx);
  this->interfaceObject = jsc_value_new_object(ctx, nullptr, nullptr);
}

JSNativeInterface::~JSNativeInterface() {
  deinit();
}

JSCContext* JSNativeInterface::getContext() {
  return this->ctx;
}

void JSNativeInterface::setupComplete() {
  jsc_value_object_set_property(this->globalObject, name.c_str(), this->interfaceObject);
}

void JSNativeInterface::registerProperty(std::string name, JSCValue* value) {
  jsc_value_object_set_property(this->interfaceObject, name.c_str(), value);
}

#endif
