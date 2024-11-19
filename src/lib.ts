import React, { Component as ReactComponent } from 'react';
import ReactReconciler from 'react-reconciler';

// Extend JSX namespace to include custom elements
declare module 'react' {
  namespace JSX {
      // Define your custom elements here
      interface IntrinsicElements {
          'native-view': any;
      }
  }
}

interface NativeBridge<T, R> {
  rootNode(): R;
  createNode(type: string): R;
  createText(text: string): R;
  setAttribute(node: T, name: string, value: string | number);
  insertBefore(parent: T, node: T, anchor: T);
  appendChild(parent: T, child: T);
  removeNode(parent: T, node: T);
  setText(node: T, value: string);
}

declare const LinuxNativeModule: NativeBridge<CNode, number>

if (typeof LinuxNativeModule !== 'object') throw new Error("LinuxNativeModule object not found");

function traceWrap(hostConfig: any): any {
  let traceWrappedHostConfig = {};
  Object.keys(hostConfig).map(key => {
    const func = hostConfig[key];
    traceWrappedHostConfig[key] = (...args) => {
      console.trace(key);
      return func(...args);
    };
  });
  return traceWrappedHostConfig;
}

function camel2Dash(str) {
  if (str === '') {
    return '';
  }

  str = str[0].toLowerCase() + str.substr(1);

  return str.replace(/([A-Z])/g, function($1) {
    return '-' + $1.toLowerCase();
  });
}

function convertCamelCasetoInlineStyle(style) {
  const transformedStyles = {};
  if (style) {
    Object.keys(style).forEach(key => {
      const dashedKey = camel2Dash(key);
      transformedStyles[dashedKey] = style[key];
      if (key === 'height') {
        transformedStyles[dashedKey] = style[key] + 'px';
      }
    });
  }
  const styleString = Object.keys(transformedStyles).map(key => {
    return `${key}:${transformedStyles[key]};`;
  });
  return styleString.join('');
}

const rootHostContext = {};
const childHostContext = {};

const hostConfig = {
  now: Date.now,
  getRootHostContext: () => {
    return rootHostContext;
  },
  prepareForCommit: () => {},
  resetAfterCommit: () => {},
  getChildHostContext: () => {
    return childHostContext;
  },
  shouldSetTextContent: (type, props) => {
    return false;
  },
  createInstance: (type: string, newProps: any, rootContainerInstance: any, _currentHostContext: any, workInProgress: any) => {
    // (print as any)(type)
    const node = LinuxNativeModule.createNode(type);
    Object.keys(newProps).forEach(propName => {
      const propValue = newProps[propName];
      LinuxNativeModule.setAttribute(CNode.fromPtr(node), propName, propValue)
    });
    return node;
  },
  createTextInstance: (text, rootContainerInstance, hostContext, internalInstanceHandle) => {
    return LinuxNativeModule.createText(text);
  },
  appendInitialChild: (parent, child) => {
    parent.addChildView(child);
  },
  appendChild(parent, child) {
    parent.addChildView(child);
  },
  finalizeInitialChildren: (yueElement, type, props) => {},
  supportsMutation: true,
  appendChildToContainer: (parent, child) => {
    // parent.setContentView(child);
    console.log(JSON.stringify(parent) + " " + JSON.stringify(child))
  },
  prepareUpdate(yueElement, oldProps, newProps) {
    return true;
  },
  commitUpdate(yueElement, updatePayload, type, oldProps, newProps) {},
  commitTextUpdate(textInstance, oldText, newText) {},
  removeChild(parentInstance, child) {},
  clearContainer(){},
  setContentView(){}
};
const ReactReconcilerInst = ReactReconciler(traceWrap(hostConfig));
export default React;

export const render = (reactElement: any, callback: any) => {
  const container = ReactReconcilerInst.createContainer(CNode.fromPtr(LinuxNativeModule.rootNode()), 0, null, false, null, "RN", console.log, null);
    // update the root Container
    return ReactReconcilerInst.updateContainer(reactElement, container, null, callback);
}

export const rootNode = LinuxNativeModule.rootNode;
export const Component = ReactComponent;

// Helper to manage C node pointers
class CNode {
  ptr: number;
  
  static fromType(type: string): CNode {
    const n = new CNode();
    n.ptr = LinuxNativeModule.createNode(type);
    return n;
  }

  static fromPtr(ptr: number): CNode {
    const n = new CNode();
    n.ptr = ptr;
    return n
  }
}