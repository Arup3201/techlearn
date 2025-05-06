const axios = require("axios").default;

axios({
  method: "post",
  url: "https://example.com/",
  data: {
    firstName: "Fred",
    lastName: "Flintstone",
  },
});
