import { CNode } from "./util.ts"

// Extend JSX namespace to include custom elements
declare module 'react' {
  namespace JSX {
    // Define your custom elements here
    interface IntrinsicElements {
      'gtk-box': any;
      'gtk-button': any;
    }
  }
}

export interface NativeBridge<T, R> {
  rootNode(): R;
  createNode(type: string): R;
  createText(text: string): R;
  setAttribute(node: T, name: string, value: string | number);
  insertBefore(parent: T, node: T, anchor: T);
  appendChild(parent: T, child: T);
  addToRoot(child: T);
  removeNode(parent: T, node: T);
  setText(node: T, value: string);
}

declare const LinuxNativeModule: NativeBridge<CNode, number>

if (typeof LinuxNativeModule !== 'object') throw new Error("LinuxNativeModule object not found");

export const LinuxNativeModule = LinuxNativeModule;