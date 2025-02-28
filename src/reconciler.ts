import React, { Component as ReactComponent } from 'react';
import ReactReconciler from 'react-reconciler';
import { LinuxNativeModule, NativeBridge } from './native.ts';
import { CNode } from './util.ts';

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
    console.log(JSON.stringify({parent, child}));
    LinuxNativeModule.appendChild(CNode.fromPtr(parent), CNode.fromPtr(child));
  },
  appendChild(parent, child) {
    console.log(parent);
    LinuxNativeModule.appendChild(CNode.fromPtr(parent), CNode.fromPtr(child));
  },
  finalizeInitialChildren: (yueElement, type, props) => {},
  supportsMutation: true,
  appendChildToContainer: (parent, child) => {
    // parent.setContentView(child);
    LinuxNativeModule.addToRoot(CNode.fromPtr(child));
    console.log(JSON.stringify(parent) + " " + JSON.stringify(child))
  },
  prepareUpdate(yueElement, oldProps, newProps) {
    return true;
  },
  commitUpdate(yueElement, updatePayload, type, oldProps, newProps) {},
  commitTextUpdate(textInstance, oldText, newText) {},
  removeChild(parentInstance, child) {},
  clearContainer(){},
  setContentView(){
    console.log("CONTENT VIEW")
  }
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

