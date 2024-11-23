import React, { Component } from './lib';

class View extends Component<{children: any; orientation?: 'horizontal' | 'vertical'}> {
  render() {
    return (
      <gtk-box orientation={this.props.orientation || "vertical"}>
        {this.props.children}
      </gtk-box>
    );
  }
}

export default View;