const s = require("../index")

;(async () => {
    const res = await s
        .parseFile({ filename: "t", type: "name.basics" })
        .catch(console.error)
    console.log(res)
})()
