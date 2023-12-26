const httpServer = require("http").createServer();
const io = require("socket.io")(httpServer, {

});
var i = 0;
io.on("connection", (socket) => {

        console.log('connected')
        setInterval(()=>{
                i++;
                io.emit("hello", "world " + i);
        }, 50)

});



httpServer.listen(3000);
