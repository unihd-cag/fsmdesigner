var g = new dagreD3.graphlib.Graph().setGraph({});
var states = [
             {id: 0, reflexive: false, text: "IDLE", start_state: true},
             {id: 1, reflexive: true, text: "H", start_state: false},
             {id: 2, reflexive: false, text: "D", start_state: false},
             {id: 3, reflexive: false, text: "READ", start_state: false},
             {id: 4, reflexive: false, text: "Done", start_state: false},
             {id: 5, reflexive: false, text: "WRITE", start_state: false},
             {id: 6, reflexive: false, text: "Done", start_state: false}
           ];
//           lastNodeId = 2,
var links = [
             {source: states[0], target: states[1], left: false, right: true },
             {source: states[1], target: states[2], left: false, right: true },
             {source: states[2], target: states[3], left: false, right: true },
             {source: states[3], target: states[4], left: false, right: true },
             {source: states[2], target: states[5], left: false, right: true },
             {source: states[5], target: states[6], left: false, right: true },
             {source: states[4], target: states[0], left: false, right: true },
             {source: states[6], target: states[0], left: false, right: true }
           ];

for (i = 0; i < states.length; i++)
	g.setNode(states[i].text, { shape: "circle" });
//states.forEach(function(state) { g.setNode(state.id, { label: state.text }); }););




var svg = d3.select("svg"),
    inner = svg.select("g");

// Set up zoom support
var zoom = d3.behavior.zoom().on("zoom", function() {
      inner.attr("transform", "translate(" + d3.event.translate + ")" +
                                  "scale(" + d3.event.scale + ")");
    });
svg.call(zoom);

// Create the renderer
var render = new dagreD3.render();

// Run the renderer. This is what draws the final graph.
render(inner, g);

// Center the graph
var initialScale = 0.75;
zoom
  .translate([(svg.attr("width") - g.graph().width * initialScale) / 2, 20])
  .scale(initialScale)
  .event(svg);
svg.attr('height', g.graph().height * initialScale + 40);