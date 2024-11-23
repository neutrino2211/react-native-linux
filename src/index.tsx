import Button from "./button";
import React, {render} from "./lib";
import View from "./view";

function App() {
  return (
    <>
      <View>
        Hello World!!
        <Button>
          Test Button
        </Button>
        <Button>
          Another Test Button
        </Button>
        <View orientation="horizontal">
          <Button>
            On the left
          </Button>
          <Button>
            On the right
          </Button>
        </View>
      </View>
      <View>
        The bottom
      </View>
    </>
  )
}

render(<App/>, null);