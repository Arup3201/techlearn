const jayson = require("jayson");
const cors = require("cors");
const jsonParser = require("body-parser").json;
const connect = require("connect");

app = connect();

// JSON-RPC methods
const methods = {
  addNumbers: (args, cb) => {
    cb(null, args[0] + args[1]);
  },
};

// Create the server
const server = new jayson.Server(methods);

app.use(cors({ methods: "POST" })); // allow CORS for POST
app.use(jsonParser()); // enable the server to parse JSON data
app.use(server.middleware()); // enable CORS for jayson server

app.listen(3000, () => {
  console.log("JSON-RPC is listening at port 3000");
});
