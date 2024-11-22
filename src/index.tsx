import React, {render} from "./lib";
import View from "./view";

function App() {
  return (
    <>
      <View>
        Hello World, this window is being controlled by React!
      </View>
    </>
  )
}

render(<App/>, null);