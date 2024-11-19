// src/main.cpp
#include <jsc/jsc.h>
#include <iostream>
#include <string.h>
#include <adwaita.h>
#include "namespace.h"
#include "JSNativeInterface.h"
#include "bundle.h"
#include "timeout.h"

class NativeModule : public JSNativeInterface {
private:
    GtkWidget* window;
    std::map<size_t, GtkWidget*> widgets;
public:
    NativeModule(JSCContext* ctx, GtkWidget* w) : JSNativeInterface(ctx, "LinuxNativeModule") {
        window = w;
    }

    static uint64_t createNode(GPtrArray* args, gpointer user_data) {
        GString* typeBuilder = g_string_new(nullptr);
        auto* arg = static_cast<char*>(g_ptr_array_index(args, 0));
        g_string_append(typeBuilder, arg);

        char* type = g_string_free(typeBuilder, false);

        std::printf("attempting to create a node of type \"%s\"\n", type);
        return 1;
    }

    static size_t rootNode(GPtrArray* args, gpointer user_data) {
        auto* self = static_cast<NativeModule*>(user_data);
        std::hash<GtkWidget*> hasher;
        size_t id = hasher(self->window);

        self->widgets.insert({id, self->window});

        return id;
    }

    static void setAttribute(GPtrArray* args, gpointer user_data) {
        auto* self = static_cast<NativeModule*>(user_data);

        JSCValue* nodeObj = static_cast<JSCValue*>(g_ptr_array_index(args, 0));
        JSCValue* nodePtrValue = jsc_value_object_get_property(nodeObj, "ptr");

        std::printf("Ptr is 0x%x\n", jsc_value_to_int32(nodePtrValue));
    }

    void init() override {
        JSCValue* createNodeBinding = jsc_value_new_function_variadic(this->getContext(), "createNode", G_CALLBACK(this->createNode), this, NULL, G_TYPE_UINT64);
        JSCValue* rootNodeBinding = jsc_value_new_function_variadic(this->getContext(), "rootNode", G_CALLBACK(this->rootNode), this, NULL, G_TYPE_UINT64);
        JSCValue* setAttribueBinding = jsc_value_new_function_variadic(this->getContext(), "setAttribute", G_CALLBACK(this->setAttribute), this, NULL, G_TYPE_NONE);

        this->registerProperty("rootNode", rootNodeBinding);
        this->registerProperty("createNode", createNodeBinding);
        this->registerProperty("setAttribute", setAttribueBinding);
        std::printf("My init\n");
        this->setupComplete();
    }

    void deinit() override {
        std::printf("My deinit\n");
    }
};

// class ReactBridge : public JSNativeInterface {
// public:
//     ReactBridge(JSCContext* ctx) : JSNativeInterface(ctx, "ReactOld") {}

//     // Static wrapper function to handle the this pointer
//     static JSCValue* createElementWrapper(GPtrArray* args, gpointer user_data) {
//         ReactBridge* self = static_cast<ReactBridge*>(user_data);
//         std::printf("COUNT: %d\n", args->len);
//         if (args->len < 2) return jsc_value_new_null(self->getContext());
        
//         if (args->len == 3) {
//             const char* el = static_cast<const char*>(g_ptr_array_index(args, 0));
//             JSCValue* node = static_cast<JSCValue*>(g_ptr_array_index(args, 1));
//             JSCValue* other = static_cast<JSCValue*>(g_ptr_array_index(args, 2));
//             return self->createElementWithProps(el, node, other);
//         } else if (args->len == 2) {

//         }

//         return jsc_value_new_null(self->getContext());
//     }

//     void init() override {
//         JSCValue* createElementBinding = jsc_value_new_function_variadic(this->getContext(), "createElement", G_CALLBACK(this->createElementWrapper), this, NULL, JSC_TYPE_VALUE);
//         this->registerProperty("createElement", createElementBinding);
//         this->setupComplete();


//     }

//     JSCValue* createElementWithProps(const char* element, JSCValue* props, JSCValue* children) {
//         if (!props || !jsc_value_is_object(props)) {
//             return jsc_value_new_null(context);
//         }

//         JSCContext* ctx = jsc_value_get_context(props);
//         auto object = jsc_value_new_object(ctx, NULL, NULL);

//         gchar** props_enum = jsc_value_object_enumerate_properties(props);
//         if (props_enum) {
//             for (gchar** ptr = props_enum; *ptr != NULL; ptr++) {
//                 g_print("Property: %s\n", *ptr);
//             }
//             g_strfreev(props_enum);
//         }

//         jsc_value_object_set_property(object, "type", 
//                                      jsc_value_new_string(ctx, "test"));
//         return object;
//     }
// };


static void PrintFunction(const char* log) {
    std::printf("JS: %s\n", log);
}

void global_exception_user(JSCContext* ctx, JSCException* ex, gpointer user_data) {
    auto* window = static_cast<GtkWidget*>(user_data);
    auto report = jsc_exception_report(ex);

    GtkWidget* label = gtk_label_new(report);
    gtk_window_set_child(GTK_WINDOW(window), label);

    std::printf("Exception:\n%s", report);
}

static void activate(GtkApplication* app) {
    GtkWidget* window = gtk_application_window_new(app);

    gtk_window_set_title(GTK_WINDOW(window), "React Native Linux");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    gtk_window_present(GTK_WINDOW(window));

    // Create a JS context and global object
    JSCContext* context = jsc_context_new();
    JSCValue* globalObject = jsc_context_get_global_object(context);

    auto module = new NativeModule(context, window);
    module->init();

    // Run some JavaScript code
    std::string script = bundle::content();

    JSCValue* printFunction = jsc_value_new_function(context, "print", G_CALLBACK(PrintFunction), NULL, NULL, G_TYPE_NONE, 1, G_TYPE_STRING);
    jsc_value_object_set_property(globalObject, "print", printFunction);

    timeout::registerFunctions(context, globalObject);

    jsc_context_push_exception_handler(context, (JSCExceptionHandler)global_exception_user, window, NULL);
    std:printf("Execution %d\n", script.size());
    jsc_context_evaluate(context, script.c_str(), script.size());

    module->deinit();
}

int main(int argc, char* argv[]) {
    AdwApplication* app = NULL;

    app = adw_application_new("io.github.neutrino2211.react-native-linux", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    return g_application_run(G_APPLICATION(app), argc, argv);
}
