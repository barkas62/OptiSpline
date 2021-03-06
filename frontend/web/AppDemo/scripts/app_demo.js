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
    constructor(state, pointerDown, dispatch) {
        this.dom = elt("canvas", {
            onmousedown: event => this.mouse(event, pointerDown),
            ontouchstart: event => this.touch(event, pointerDown)
        });
        this.dispatch = dispatch; 
        this.syncState(state);
    }

    syncState(state) {
      this.state = state;

      drawStroke(this.state.stroke, this.dom, this.state.draw_src, this.state.draw_app, this.state.draw_rsm, this.state.draw_bez);
    }
}

function drawStroke(stroke, canvas, draw_src, draw_app, draw_rsm, draw_bez) {
    canvas.width  = 748;
    canvas.height = 748;

    let cx = canvas.getContext("2d");
    //console.log("in drawStroke: what2draw: " + what2draw.toString());

    if (draw_src && stroke.org_points.length != 0){
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
      cx.strokeStyle = "blue";
      cx.stroke();
    }

    if (draw_app && stroke.app_points.length != 0){
      cx.beginPath();
      let x0 = stroke.app_points[0];
      let y0 = stroke.app_points[1];
      cx.lineTo(x0, y0);
      for (let i = 1; i < stroke.app_points.length; i++) {
          let x = stroke.app_points[2*i+0];
          let y = stroke.app_points[2*i+1];
          //console.log("in drawStroke: " + x.toString() + " " + y.toString());
          cx.lineTo(x, y);
      }
      cx.strokeStyle = "red";
      cx.stroke();
    }

    if (draw_rsm && stroke.rsm_points.length != 0){
      let radius = 1.5
      for (let i = 0; i < stroke.rsm_points.length; i++) {
          let x = stroke.rsm_points[2*i+0];
          let y = stroke.rsm_points[2*i+1];
          cx.beginPath();
          cx.arc(x, y, radius, 0, 2*Math.PI, false);
          cx.strokeStyle = "black";
          cx.stroke();
      }
    }

}

StrokeCanvas.prototype.mouse = function(downEvent, onDown) {
    if (downEvent.button != 0) return;
    this.state.stroke.reset();
    console.log("in mouse: " + this.state.stroke.sam());

    if (this.state.source != "freehand")
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
      console.log("in mouseUp: stroke length: " + this.state.stroke.org_points.length.toString());
      this.dom.removeEventListener("mouseup", mouseUp);
      this.state.stroke.init_approx(this.state.resam, this.state.ord);
      drawStroke(this.state.stroke, this.dom, this.state.draw_src, this.state.draw_app, this.state.draw_rsm, this.state.draw_bez);
      this.dispatch({max_err: this.state.stroke.max_err, rms_err: this.state.stroke.rms_err })
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
  
      this.canvas = new StrokeCanvas( state, 
        pos => {
        let onMove = draw(pos, this.state, dispatch);
        if (onMove) return pos => onMove(pos, this.state);
      }, dispatch);
      
      console.log("Setting controls");
      this.controls = controls.map( Control => new Control(state, config) );
      console.log("Controls set");

      this.dom = elt("div", {}, this.canvas.dom, elt("br"));
    }

    syncState(state) {
      //console.log("in AppDemo.syncState: draw_src: " + state.draw_src.toString()); 
      // console.log("resam: " + state.resam.toString());
      if (state.resam != this.state.resam || state.ord != this.state.ord){
        state.iter = 0;
        state.stroke.init_approx(state.resam, state.ord);
        state.max_err = state.stroke.max_err;
        state.rms_err = state.stroke.rms_err;
      }

      if (state.source != this.state.source){
        // Source changed
        let D = 250.0;
        switch(state.source){
          case "freehand":
            state.stroke = new AppStroke([]);
          break;
          case "square":
            state.stroke = new AppStroke( 
              [  D,   D, 
               2*D,   D,
               2*D, 2*D,
                 D, 2*D,
                 D,   D]);
          break;  
          case "triangle":
            state.stroke = new AppStroke( 
              [3*D/2,  D, 
               2*D,  2*D,
                 D,  2*D,
               3*D/2,  D]);
            break;
            case "lambda":
              state.stroke = new AppStroke( 
                [  D/2, 2*D, 
                   D,   2*D,
                 3*D/2,   D,
                 2*D,   2*D, 
                 5*D/2, 2*D]);
              break;
        }
        state.iter = 0;
        state.stroke.init_approx(state.resam, state.ord); 
      }

      if (state.iter != this.state.iter){
        if (state.iter > this.state.iter){
          state.stroke.param_app(state.iter - this.state.iter, -1);
          state.max_err = state.stroke.max_err;
          state.rms_err = state.stroke.rms_err;
        }
        else
        {
          state.stroke.init_approx(state.resam, state.ord);
          state.max_err = state.stroke.max_err;
          state.rms_err = state.stroke.rms_err;
        }
      }

      this.state = state;
      this.canvas.syncState(state);
      for (let ctrl of this.controls) ctrl.syncState(state);
    }
  }

function draw(pos, state, dispatch) {
    function drawPoint({x, y}, state) {
      let new_pnt = [x, y];
      //console.log("in draw: " + x.toString() + " " + y.toString());
      dispatch({iter: 0, stroke: state.stroke.draw(new_pnt)});
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
    this.dom = elt("p", null, "Points in stroke: " + state.stroke.sam().toString());
    document.getElementById(this.parent_id).appendChild(this.dom);
  }
  syncState(state) { 
    this.dom.innerHTML = "Points in stroke: " + state.stroke.sam().toString();
   }
}

class MaxErrText{
  constructor(state, {stroke, dispatch}){
    this.parent_id = "panel-max-err";
    this.dom = elt("p", null, "Max Error: N/a");
    document.getElementById(this.parent_id).appendChild(this.dom);
  }
  syncState(state) { 
    this.dom.innerHTML = "Max Error: " + state.max_err.toFixed(2).toString();
   }
}

class RmsErrText{
  constructor(state, {stroke, dispatch}){
    this.parent_id = "panel-rms-err";
    this.dom = elt("p", null, "RMS Error: N/a");
    document.getElementById(this.parent_id).appendChild(this.dom);
  }
  syncState(state) { 
    this.dom.innerHTML = "RMS Error: " + state.rms_err.toFixed(2).toString();
   }
}

class StepButton {
  constructor(state, {dispatch}) {
    this.parent_id = "panel-step";
    this.iter = state.iter
    this.dom = elt("button", { onclick: () => dispatch({iter: this.iter+1}) }, "Iterations: " + state.iter.toString());
    document.getElementById(this.parent_id).appendChild(this.dom);
  }

  syncState(state) { 
    this.iter = state.iter;
    this.dom.innerHTML = "Iterations: " + this.iter.toString(); }
}

class ResetButton {
  constructor(state, {dispatch}) {
    this.parent_id = "panel-reset";
    this.dom = elt("button", { onclick: () => dispatch({iter: 0}) }, "Reset");
    document.getElementById(this.parent_id).appendChild(this.dom);
  }

  syncState(state) {}
}

class ResamInput {
  constructor(state, {dispatch}) {
    this.parent_id = "panel-resam";
    this.resam = state.resam
    this.inp = elt("input", {type: "number", style: "width: 3em", value: this.resam.toString(), 
                                onchange: () => dispatch({resam: parseInt(this.inp.value)}) });
    this.dom = elt("label", null, "Resam: ", this.inp);

    document.getElementById(this.parent_id).appendChild(this.dom);
  }

  syncState(state) { 
    this.resam = state.resam;
    this.inp.value = this.resam.toString(); }
}

class OrdInput {
  constructor(state, {dispatch}) {
    this.parent_id = "panel-ord";
    this.ord = state.ord
    this.inp = elt("input", {type: "number", style: "width: 3em", value: this.ord.toString(), 
                                onchange: () => dispatch({ord: parseInt(this.inp.value)}) });
    this.dom = elt("label", null, "Order: ", this.inp);

    document.getElementById(this.parent_id).appendChild(this.dom);
  }

  syncState(state) { 
    this.ord = state.ord;
    this.inp.value = this.ord.toString(); }
}

class SetParamsButton {
  constructor(state, {dispatch}) {
    this.parent_id = "panel-set-params";
    this.iter = state.iter
    this.dom = elt("button", { onclick: () => dispatch({resam: this.iter+1}) }, "Set");
    document.getElementById(this.parent_id).appendChild(this.dom);
  }

  syncState(state) { 
    this.iter = state.iter;
    this.dom.innerHTML = "Iterations: " + this.iter.toString(); }
}

class SrcCheckbox {
  constructor(state, {dispatch}) {
    this.parent_id = "panel-draw-src";
    this.checked = true;
    this.chk = document.getElementById("draw-src-chk") 
    Object.assign(this.chk, { onchange: () => dispatch({draw_src: !this.checked}) });
  }

  syncState(state) { 
    this.checked = state.draw_src;
    }
}

class AppCheckbox {
  constructor(state, {dispatch}) {
    this.parent_id = "panel-draw-app";
    this.checked = true;
    this.chk = document.getElementById("draw-app-chk") 
    Object.assign(this.chk, { onchange: () => dispatch({draw_app: !this.checked}) });
  }

  syncState(state) { 
    this.checked = state.draw_app;
    }
}

class RsmCheckbox {
  constructor(state, {dispatch}) {
    this.parent_id = "panel-draw-rsm";
    this.checked = true;
    this.chk = document.getElementById("draw-rsm-chk") 
    Object.assign(this.chk, { onchange: () => dispatch({draw_rsm: !this.checked}) });
  }

  syncState(state) { 
    this.checked = state.draw_rsm;
    }
}

class InfoText{
  constructor(state, {stroke, dispatch}){
    this.parent_id = "panel-info";
    this.dom = elt("p", null, "");
    document.getElementById(this.parent_id).appendChild(this.dom);
    if (state.source == "freehand")
      this.dom.innerHTML = "Draw a single stroke you want to approximate.<br><br>Or select a predefined shape from the <b>Source</b> drop-down list.";
    else
      this.dom.innerHTML = "Click <b>Iterations</b> button. <br> Or change the number of sampling points (<b>Resam</b>), or approximation order (<b>Order</b>)";
  }
  syncState(state) { 
    let info = "";
    if (state.iter == 0){
      if (state.source == "freehand")
        if (state.stroke.sam() == 0)
          info = "Draw a single stroke you want to approximate.<br>Or select a predefined shape from a Source drop-down list.";
        else
          if (state.stroke.resam == 0)
            info = "";
          else 
          info = init_info;
      else 
        info = init_info;
    }
    else{
      info = iter_info;
    }
    this.dom.innerHTML = info;
   }
}

const init_info = `
You see the initial approximation (red line).<br> 
Sampling points are distributed equidistantly along the stroke (black dots).<br>
The number of sampling points is defined by <b>Resam</b> value.<br>
The stroke is approximated by cosine polynomials, the order of approximation is defined by <b>Order</b> value.<br>   
Approximation quality: <b>RMS Error</b> and <b>Max Error.</b> <br><br>
Click <b>Iterations</b> button. <br><br> 
You may change the number of sampling points (<b>Resam</b>), or approximation order (<b>Order</b>).<br>
`;

const iter_info = `
Continue to iterate by clicking the <b>Iterations</b> button. (Or press and hold ENTER key.)<br> 
See how approximation is improving and both <b>RMS Error</b> and <b>Max Error</b> are decreasing.<br><br>
Clicking <b>Reset</b> button restarts approximating iterations.<br>
Changing <b>Resam</b> or <b>Order</b> also restarts approximation with new parameters.
`

const startState = {
  source    : "freehand",
  draw_src  : true,
  draw_app  : true,
  draw_rsm  : true,
  draw_bez  : false,  
  iter      : 0,
  rms_err   : 0,
  max_err   : 0,
  resam     : 32,
  ord       : 9,
  stroke: new AppStroke([])
};

const baseSources = ["freehand", "square", "triangle", "lambda"];

const baseControls = [
  SourceSelect,
  SamText, 
  ResamInput,
  OrdInput,
  StepButton, 
  ResetButton,
  MaxErrText,
  RmsErrText,
  SrcCheckbox, 
  AppCheckbox,
  RsmCheckbox,
  InfoText 
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





