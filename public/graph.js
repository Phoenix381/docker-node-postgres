
const loadBtn = document.getElementById('load-button');
const runBtn = document.getElementById('run-button');

const graphEditor = document.getElementById('graph-editor');
const graphContainer = document.getElementById('graph-container');
const resultsContainer = document.getElementById('results-container');

let graph;
let vertices;

// Run button
loadBtn.addEventListener('click', () => {
    // vertex vertex interval
    // a b 1.1;2.2

    // reset data
    graph = [];
    vertices = new Set();

    // parsing strings
    const rows = graphEditor.value.split('\n');
    for(let row of rows) {
        try {
            const [a, b, c] = row.split(' ');

            // valiidating capacity format 1.1;2.2
            let format = /^([0-9]*[.]?[0-9]?);([0-9]*[.]?[0-9]?)$/;
            if(format.test(c)) {
                graph.push({source: a, target: b, capacity: c});
                vertices.add(a);
                vertices.add(b);
            } else {
                console.log(`Invalid interval format: ${c}`);
            }
        } catch(e) {
            console.log(`Invalid row format: ${row}`);   
        }
    }

    vertices = Array.from(vertices, (x) => { return {title: x} });

    drawGraph();

    if(graph.length > 0 && vertices.length > 0) {
        runBtn.disabled = false;
    } else {
        runBtn.disabled = true;
    }
});

function clamp(x, lo, hi) {
  return x < lo ? lo : x > hi ? hi : x;
}

// Drawing graph
function drawGraph() {
    let width = graphContainer.offsetWidth;
    let height = graphContainer.offsetHeight;

    // reset canvas
    graphContainer.innerHTML = "";

    const color = d3.scaleOrdinal(d3.schemeCategory10);

    const links = graph.map(d => ({...d}));
    const nodes = vertices.map(d => ({...d}));

    // Create a simulation with several forces.
    const simulation = d3.forceSimulation(nodes)
      .force("link", d3.forceLink(links).id(d => d.title).distance(50))
      .force("charge", d3.forceManyBody().strength(-300))
      .force("center", d3.forceCenter(width / 2, height / 2))
      .on("tick", ticked);

    // Create an SVG element.
    const svg = d3.select("#graph-container")
        .append("svg")
        .attr("width", width)
        .attr("height", height);

    // Add a line for each link, and a circle for each node.
    const link = svg.append("g")
        .attr("stroke", "#999")
        .attr("stroke-opacity", 0.6)
        .selectAll()
        .data(links)
        .join("line")

    // Create the link labels
    const linkLabel = svg.append("g")
        .attr("class", "link-label")
        .selectAll()
        .data(links)
        .join("text")
        .attr("x", d => d.source.x + (d.target.x - d.source.x) / 2)
        .attr("y", d => d.source.y + (d.target.y - d.source.y) / 2)
        .attr("dy", 15)
        .attr("text-anchor", "middle")
        .text(d => d.capacity);


    // Create node groups.
    const node = svg.append("g")
        .attr("class", "node")
        .selectAll("g")
        .data(nodes)
        .join("g");

    // Add a circle for each node.
    node.append("circle")
        .attr("r", 6)
        .attr("fill", d => color(d.title));

    // Add id labels for each node.
    node.append("text")
        .attr("dy", 24)
        .attr("text-anchor", "middle")
        .text(d => d.title);


    // Add drag behaviors to each node.
    node.call(d3.drag()
        .on("start", dragstarted)
        .on("drag", dragged)
        .on("end", dragended));

    // Set the position attributes of links and nodes each time the simulation ticks.
    function ticked() {
    link
        .attr("x1", d => d.source.x)
        .attr("y1", d => d.source.y)
        .attr("x2", d => d.target.x)
        .attr("y2", d => d.target.y);
    linkLabel
        .attr("x", d => d.source.x + (d.target.x - d.source.x) / 2)
        .attr("y", d => d.source.y + (d.target.y - d.source.y) / 2);
    node
        .attr("transform", d => `translate(${d.x},${d.y})`);
    }

    // Reheat the simulation when drag starts, and fix the subject position.
    function dragstarted(event) {
        if (!event.active) simulation.alphaTarget(0.3).restart();
        event.subject.fx = event.subject.x;
        event.subject.fy = event.subject.y;
    }

    // Update the subject (dragged node) position during drag.
    function dragged(event, d) {
        d.fx = clamp(event.x, 0, width);
        d.fy = clamp(event.y, 0, height);
        simulation.alpha(1).restart();
    }

    // Restore the target alpha so the simulation cools after dragging ends.
    // Unfix the subject position now that it’s no longer being dragged.
    function dragended(event) {
        if (!event.active) simulation.alphaTarget(0);
    }
}

// Run button
runBtn.addEventListener('click', () => {
    console.log('sending data...');
    console.log(vertices);
    console.log(graph);

    fetch('/api/run', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        // body: JSON.stringify({ vertices: vertices, edges: graph })
        body: JSON.stringify({ vertices: vertices, graph: graph})
    })
    .then(response => response.json())
    .then(data => console.log(data))
    .catch(error => console.error('Error:', error));
});