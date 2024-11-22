import React, { Component } from './lib';

class View extends Component<any> {
  render() {
    return (
      <gtk-box>
        {this.props.children}
      </gtk-box>
    );
  }
}

export default View;