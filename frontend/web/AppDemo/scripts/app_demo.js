class AppStroke {
    constructor(resam, ord, points) {
        this.reset();

        this.resam = resam;
        this.order = ord;
        this.org_points = points;
    }

    reset(){
        this.org_points = new Array();
        this.rsm_points = [];
        this.app_points = [];
        
        this.err = 0.0;
        this.lam = 0.0;
    }

    sam() {
      return this.org_points.length/2;
    }

    draw(points) {
        let copy_points = this.org_points.concat(points);
        return new AppStroke(this.resam, this.order, copy_points);
      }

}

function updateState(state, action) {
    return Object.assign({}, state, action);
  }

function elt(type, props, ...children) {
    let dom = document.createElement(type);
    if (props) Object.assign(dom, props);
    for (let child of children) {
      if (typeof child != "string") dom.appendChild(child);
      else dom.appendChild(document.createTextNode(child));
    }

    return dom;
  }
  
class StrokeCanvas {
    constructor(source, stroke, pointerDown) {
        this.dom = elt("canvas", {
            onmousedown: event => this.mouse(event, pointerDown),
            ontouchstart: event => this.touch(event, pointerDown)
        });
        this.syncState(source, stroke);
    }

    syncState(source, stroke) {
      this.source = source;
      if (this.stroke != stroke){
          this.stroke = stroke;
        }
        drawStroke(this.stroke, this.dom);
    }
}

function drawStroke(stroke, canvas) {
    canvas.width  = 748;
    canvas.height = 748;

    let cx = canvas.getContext("2d");
    //console.log("in drawStroke: length: " + stroke.org_points.length.toString());
    let nOrg = stroke.org_points.length; 
    if( nOrg == 0)
      return;

    cx.beginPath();
    let x0 = stroke.org_points[0];
    let y0 = stroke.org_points[1];
    cx.lineTo(x0, y0);
    for (let i = 1; i < stroke.org_points.length; i++) {
        let x = stroke.org_points[2*i+0];
        let y = stroke.org_points[2*i+1];
        //console.log("in drawStroke: " + x.toString() + " " + y.toString());
        cx.lineTo(x, y);
    }
    cx.stroke();
}

StrokeCanvas.prototype.mouse = function(downEvent, onDown) {
    if (downEvent.button != 0) return;
    this.stroke.reset();
    console.log("in mouse: " + this.stroke.sam());

    if (this.source != "freehand")
      return;
  
    let pos = pointerPosition(downEvent, this.dom);
    let onMove = onDown(pos);
    if (!onMove) return;

    let mouseMove = moveEvent => {
      if (moveEvent.buttons == 0) {
        this.dom.removeEventListener("mousemove", mouseMove);
      } else {
        let newPos = pointerPosition(moveEvent, this.dom);
        if (newPos.x == pos.x && newPos.y == pos.y) return;
        pos = newPos;
        onMove(newPos);
      }
    };

    let mouseUp = upEvent => {
      console.log("in mouseUp: stroke length: " + this.stroke.org_points.length.toString());
      this.dom.removeEventListener("mouseup", mouseUp);
    };

    this.dom.addEventListener("mousemove", mouseMove);
    this.dom.addEventListener("mouseup"  , mouseUp  );
};
  
function pointerPosition(pos, domNode) {
    let rect = domNode.getBoundingClientRect();
    return {x: (pos.clientX - rect.left),
            y: (pos.clientY - rect.top)};
}

StrokeCanvas.prototype.touch = function(startEvent, onDown) {
    let pos = pointerPosition(startEvent.touches[0], this.dom);
    let onMove = onDown(pos);
    startEvent.preventDefault();
    if (!onMove) return;

    let move = moveEvent => {
        let newPos = pointerPosition(moveEvent.touches[0], this.dom);
        if (newPos.x == pos.x && newPos.y == pos.y) return;
        pos = newPos;
        onMove(newPos);
    };

    let end = () => {
        this.dom.removeEventListener("touchmove", move);
        this.dom.removeEventListener("touchend", end);
    };

    this.dom.addEventListener("touchmove", move);
    this.dom.addEventListener("touchend", end);
};

class AppDemo {
    constructor(state, config) {
        let {sources, controls, dispatch} = config;
        this.state = state;
  
      this.canvas = new StrokeCanvas( state.source, state.stroke, pos => {
        let onMove = draw(pos, this.state, dispatch);
        if (onMove) return pos => onMove(pos, this.state);
      });
      
      console.log("Setting controls");
      this.controls = controls.map( Control => new Control(state, config) );
      console.log("Controls set");

      this.dom = elt("div", {}, this.canvas.dom, elt("br"));
    }

    syncState(state) {
      console.log("in AppDemo.syncState: " + state.source); 
      if (state.source != this.state.source){
        // Source changed
        let D = 250.0;
        switch(state.source){
          case "freehand":
            state.stroke = new AppStroke(32, 7, []);
          break;
          case "square":
            state.stroke = new AppStroke(32, 7, 
              [  D,   D, 
               2*D,   D,
               2*D, 2*D,
                 D, 2*D,
                 D,   D]);
          break;  
          case "triangle":
            state.stroke = new AppStroke(32, 7, 
              [3*D/2,  D, 
               2*D,  2*D,
                 D,  2*D,
               3*D/2,  D]);
            break;
            case "lambda":
              state.stroke = new AppStroke(32, 7, 
                [  D/2, 2*D, 
                   D,   2*D,
                 3*D/2,   D,
                 2*D,   2*D, 
                 5*D/2, 2*D]);
              break;
        }
         
      }
      this.state = state;
      this.canvas.syncState(state.source, state.stroke);
      for (let ctrl of this.controls) ctrl.syncState(state);
    }
  }

function draw(pos, state, dispatch) {
    function drawPoint({x, y}, state) {
      let new_pnt = [x, y];
      //console.log("in draw: " + x.toString() + " " + y.toString());
      dispatch({stroke: state.stroke.draw(new_pnt)});
    }
    drawPoint(pos, state);
    return drawPoint;
}

class SourceSelect {
  constructor(state, {sources, dispatch}) {
    this.parent_id = "panel-source";
    this.select = elt("select", {
      onchange: () => dispatch({source: this.select.value})
    }, ...sources.map(name => elt("option", {
      selected: name == state.source
    }, name)));
    this.dom = elt("label", null, "Source: ", this.select);

    console.log("SourceSelect.parent_id: " + this.parent_id);
    document.getElementById(this.parent_id).appendChild(this.dom);
  }
  syncState(state) { this.select.value = state.source; }
}

class SamText{
  constructor(state, {stroke, dispatch}){
    this.parent_id = "panel-sam";
    this.dom = elt("p", null, "Sam: " + state.stroke.sam().toString());
    document.getElementById(this.parent_id).appendChild(this.dom);
  }
  syncState(state) { 
    console.log("SamText: stroke length: " + state.stroke.sam().toString());
    this.dom.innerHTML = "Sam: " + state.stroke.sam().toString();
   }
}

const startState = {
  source: "freehand",
  iter: 0,
  stroke: new AppStroke(32, 7, [])
};

const baseSources = ["freehand", "square", "triangle", "lambda"];

const baseControls = [
  SourceSelect,
  SamText 
];  

function startAppDemo({state    = startState,
                       sources  = baseSources,
                       controls = baseControls}) {
  let app = new AppDemo(state, {
    sources,
    controls,
    dispatch(action) {
      state = updateState(state, action);
      app.syncState(state);
  }
  });
  return app.dom;
}





