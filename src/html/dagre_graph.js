var g = new dagreD3.graphlib.Graph().setGraph({});
var states = [ {
	id : 0,
	reflexive : false,
	text : "IDLE",
	start_state : true,
	ui : null,
	transitions : [ {
		to : "H"
	} ]

}, {
	id : 1,
	reflexive : true,
	text : "H",
	start_state : false,
	ui : null,
	transitions : [ {
		to : "D"
	} ]
}, {
	id : 2,
	reflexive : false,
	text : "D",
	start_state : false,
	ui : null,
	transitions : [ {
		to : "WRITE"
	}, {
		to : "READ"
	} ]
}, {
	id : 3,
	reflexive : false,
	text : "READ",
	start_state : false,
	ui : null,
	transitions : [ {
		to : "Done_R"
	}

	]
}, {
	id : 4,
	reflexive : false,
	text : "Done_R",
	start_state : false,
	ui : null,
	transitions : [ {
		to : "IDLE"
	}

	]
}, {
	id : 5,
	reflexive : false,
	text : "WRITE",
	start_state : false,
	ui : null,
	transitions : [ {
		to : "Done_W"
	}

	]
}, {
	id : 6,
	reflexive : false,
	text : "Done_W",
	start_state : false,
	ui : null,
	transitions : [ {
		to : "IDLE"
	}

	]
} ];

// Place states
$(states).each(function(i, state) {

	g.setNode(state.text, {
		shape : "circle"
	});
	
	if (state.start_state) {
		 //g.node(state.text).style = "stroke-width: 5px";
		g.node(state.text).class = "start-state";
	}

});

// Place edges
$(states).each(function(i, state) {

	$(state.transitions).each(function(i, transition) {

		// g.node(transition.to)
		g.setEdge(state.text, transition.to, {
			label : ""
		});
	});

});

// states.forEach(function(state) { g.setNode(state.id, { label: state.text });
// }););

// Edges must be set by the text of the states. Names should consequently be
// unique.
/*
 * g.setEdge(states[0].text, states[1].text, { label: "" });
 * g.setEdge(states[1].text, states[2].text, { label: "" });
 * g.setEdge(states[2].text, states[3].text, { label: "" });
 * g.setEdge(states[3].text, states[4].text, { label: "" });
 * g.setEdge(states[2].text, states[5].text, { label: "" });
 * g.setEdge(states[5].text, states[6].text, { label: "" });
 * g.setEdge(states[4].text, states[0].text, { label: "" });
 * g.setEdge(states[6].text, states[0].text, { label: "" });
 */


//var svg = d3.select("svg"), inner = svg.select("g");

d3.select("#interface") 
	.append("svg")
		.attr("width", 960)
		.attr("height", 600)
	.append("g");

var svg = d3.select("#interface").select("svg");
var inner = svg.select("g");

// Set up zoom support
var zoom = d3.behavior.zoom().on(
		"zoom",
		function() {
			inner.attr("transform", "translate(" + d3.event.translate + ")"
					+ "scale(" + d3.event.scale + ")");
		});
svg.call(zoom);

// Create the renderer
var render = new dagreD3.render();

// Run the renderer. This is what draws the final graph.
render(inner, g);

// Center the graph
var initialScale = 0.75;
zoom
		.translate(
				[ (svg.attr("width") - g.graph().width * initialScale) / 2, 20 ])
		.scale(initialScale).event(svg);
svg.attr('height', g.graph().height * initialScale + 50);

//$(states).each(function(i,state){
//	state.click(function() {
//		$("#table").html("test");
//	});
//});

$(".node").click(function(state) {
//	alert(this.childNodes[1].childNodes[0].childNodes[0].childNodes[0].childNodes[0]);
	var a = this.childNodes[1].childNodes[0].childNodes[0].childNodes[0].childNodes[0]
	$("#table").html(a);
});

$("#table").click(function() {
	$("#table").html(this.id);
});