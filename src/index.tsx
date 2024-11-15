import { createRenderer } from 'solid-js/universal';
import { JSX, Component, createSignal, createEffect } from 'solid-js';

// Extend JSX namespace to include custom elements
declare module 'solid-js' {
    namespace JSX {
        // Define your custom elements here
        interface IntrinsicElements {
            'AdwSplitOverlay': any;
            'native-view': any;
        }
    }
}

interface NativeBridge<T, R> {
    createNode(type: string): R;
    setAttribute(node: T, name: string, value: string | number);
    insertBefore(parent: T, node: T, anchor: T);
    appendChild(parent: T, child: T);
    removeNode(parent: T, node: T);
    setText(node: T, value: string);
}

declare const SolidNative: NativeBridge<CNode, number>

if (typeof SolidNative !== 'object') throw new Error("SolidNative object not found");

// Helper to manage C node pointers
class CNode {
  ptr: any;
  
  constructor(type: string) {
    this.ptr = SolidNative.createNode(type);
  }
}

const AdwSplitOverlay: Component<any> = (props) => {
    let buttonNode: CNode;
  
    createEffect(() => {
      // Map props to native calls
      if (buttonNode) {
        // buttonNode.setNativeProperty('color', props.color);
        // buttonNode.setNativeProperty('size', props.size);
      }
    });
  
    return (
      <native-view
        onClick={props.onClick}
        nativeType="button"
      >
        {props.children}
      </native-view>
    );
  };

// Create custom renderer
const renderer = createRenderer<CNode>({
  // Create DOM Node
  createElement(type) {
    return new CNode(type);
  },

  // Set properties/attributes
  setProperty(node, name, value) {
    if (value == null) return;
    SolidNative.setAttribute(node.ptr, name, value.toString());
  },

  // Insert node into parent before anchor
  insertNode(parent, node, anchor) {
    if (anchor) {
      SolidNative.insertBefore(parent.ptr, node.ptr, anchor.ptr);
    } else {
      SolidNative.appendChild(parent.ptr, node.ptr);
    }
  },

  // Remove node from parent
  removeNode(parent, node) {
    SolidNative.removeNode(parent.ptr, node.ptr);
  },

  // Set text content
  replaceText(node, text) {
    SolidNative.setText(node.ptr, text);
  },

  isTextNode(node) {
    return true
  },

  createTextNode(value) {
    return new CNode(value)
  },

  // Get parent node
  getParentNode(node) {
    // Implementation depends on C-side parent tracking
    throw new Error("Not implemented");
  },

  getFirstChild(node) {
    throw new Error("Not implemented");
  },

  getNextSibling(node) {
    throw new Error("Not implemented");
  }
});

// Export render function
function render(code: () => any, container: CNode) {
  renderer.render(code, container);
}

render(() => <AdwSplitOverlay/>, new CNode('root'))