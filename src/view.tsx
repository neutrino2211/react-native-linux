import React, { Component } from './lib';

class View extends Component<any> {
  render() {
    return (
      <native-view>
        {this.props.children}
      </native-view>
    );
  }
}

export default View;