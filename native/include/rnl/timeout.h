#ifndef RNL_TIMEOUT_H
#define RNL_TIMEOUT_H
#include <iostream>
#include <thread>
#include <jsc/jsc.h>
#include <map>

namespace timeout {
  typedef struct Task {
    JSCValue* fn;
    bool should_run;
  } task_t;
  std::map<size_t, task_t> tasks;
  GMainLoop* loop;

  static void TimerExecuteCallback(task_t task) {
    std::printf("TIMER %x\n", task.fn);
    if (task.should_run)
      jsc_value_function_call(task.fn, G_TYPE_NONE);

    std::printf("TIMER\n");

    return;
  }

  size_t _SetTimeout(JSCValue* fn, guint time) {
    task_t task = task_t{
      .fn = fn,
      .should_run = true,
    };

    std::printf("TIMEOUT ONCE: %d\n", time);
    auto t = std::thread([&]() {
      auto delta = std::chrono::steady_clock::now() + std::chrono::milliseconds(time);
      std::this_thread::sleep_until(delta);
      TimerExecuteCallback(task);
    });
    t.detach();

    std::hash<std::thread::id> hasher;
    size_t id = hasher(t.get_id());
    // guint id = g_timeout_add(time, G_SOURCE_FUNC(TimerExecuteCallback), &task);
    tasks.insert(std::make_pair(id, task));
    std::printf("TIMEOUT ID: %d\n", id);
  }

  static size_t SetTimeout(GPtrArray* args, gpointer user_data) {
    std::printf("ST LEN: %d\n", args->len);
    auto* fn = static_cast<JSCValue*>(g_ptr_array_index(args, 0));
    auto timeout = static_cast<int*>(g_ptr_array_index(args, 1));

    std::printf("ST DONE\n");

    return _SetTimeout(fn, *timeout);
  }

  static void ClearTimeout(GPtrArray* args, gpointer user_data) {
    std::printf("CT LEN: %d\n", args->len);
    auto id = static_cast<guint*>(g_ptr_array_index(args, 0));
    tasks.at(*id).should_run = false;
    std::printf("CT DONE\n");
  }

  static void SetImmediate(GPtrArray* args, gpointer user_data) {
    auto* fn = static_cast<JSCValue*>(g_ptr_array_index(args, 0));
    std::printf("SI LEN: %d\n", args->len);
    _SetTimeout(fn, 0);
    std::printf("SI DONE\n");
  }

  void prepareLoop() {
    loop = g_main_loop_new(NULL, FALSE);
  }

  void registerFunctions(JSCContext* ctx, JSCValue* globalObject) {
    auto* setTimeoutBinding = jsc_value_new_function_variadic(ctx, "setTimeout", G_CALLBACK(SetTimeout), NULL, NULL, G_TYPE_UINT);
    auto* setImmediateBinding = jsc_value_new_function_variadic(ctx, "setImmediate", G_CALLBACK(SetImmediate), NULL, NULL, G_TYPE_NONE);
    auto* clearTimeoutBinding = jsc_value_new_function_variadic(ctx, "clearTimeout", G_CALLBACK(ClearTimeout), NULL, NULL, G_TYPE_NONE);

    jsc_value_object_set_property(globalObject, "_setTimeout", setTimeoutBinding);
    jsc_value_object_set_property(globalObject, "_clearTimeout", clearTimeoutBinding);
    jsc_value_object_set_property(globalObject, "_setImmediate", setImmediateBinding);
  }

  void cleanup() {
    g_main_loop_unref(loop);
  }
} // namespace timeout


#endif