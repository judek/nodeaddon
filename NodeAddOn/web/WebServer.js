// To create HTTP server, we need to load the http module from NodeJS.
var http = require('http');
var addon = require('./NodeAddOn')
var obj = new addon.MyObject(10);

// Configure our HTTP server to respond with Hello World to all requests.
var server = http.createServer(
function (request, response) {
  response.writeHead(200, {"Content-Type": "text/html"});
  response.end("<html><body><h1>From C++ " + obj.plusOne() + "</h1></body></html>");
});

// Listen on port 6148, IP defaults to 127.0.0.1
server.listen(6148);

// Put a friendly message on the terminal
console.log("Server running at http://127.0.0.1:6148/");