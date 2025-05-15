const axios = require("axios").default;

function get(resource: string) {
  return axios({
    method: "get",
    url: `127.0.0.1${resource}`,
  });
}

function post(resource: string, data) {
  return axios({
    method: "post",
    url: `127.0.0.1${resource}`,
    data: data,
  });
}

try {
  const users = await get("/users");
  console.log(users);
} catch (err) {
  console.log("error fetching users:", err);
}
