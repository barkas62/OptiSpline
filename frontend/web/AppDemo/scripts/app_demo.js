class BStroke {
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

    draw(points) {
        let copy_points = this.org_points.concat(points);
        return new BStroke(this.resam, this.order, copy_points);
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
    constructor(stroke, pointerDown) {
        this.dom = elt("canvas", {
            onmousedown: event => this.mouse(event, pointerDown),
            ontouchstart: event => this.touch(event, pointerDown)
        });
        this.syncState(stroke);
    }

    syncState(stroke) {
        if (this.stroke == stroke) 
            return;

        this.stroke = stroke;
        drawStroke(this.stroke, this.dom);
    }
}

function drawStroke(stroke, canvas) {
    canvas.width  = 748;
    canvas.height = 748;

    let cx = canvas.getContext("2d");
    //console.log("in drawStroke: length: " + stroke.org_points.length.toString());

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
    console.log("in mouse: " + downEvent.x.toString() + " " + downEvent.y.toString());
    let pos = pointerPosition(downEvent, this.dom);
    let onMove = onDown(pos);
    if (!onMove) return;

    let move = moveEvent => {
      if (moveEvent.buttons == 0) {
        this.dom.removeEventListener("mousemove", move);
      } else {
        let newPos = pointerPosition(moveEvent, this.dom);
        if (newPos.x == pos.x && newPos.y == pos.y) return;
        pos = newPos;
        onMove(newPos);
      }
    };
    this.dom.addEventListener("mousemove", move);
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
        let {tools, controls, dispatch} = config;
        this.state = state;
  
      this.canvas = new StrokeCanvas( state.stroke, pos => {
        let tool = tools[this.state.tool];
        let onMove = tool(pos, this.state, dispatch);
        if (onMove) return pos => onMove(pos, this.state);
      });
      
    
      this.controls = [] //controls.map( Control => new Control(state, config) );

      this.dom = elt("div", {}, this.canvas.dom, elt("br"));
    }

    syncState(state) {
      this.state = state;
      this.canvas.syncState(state.stroke);
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


