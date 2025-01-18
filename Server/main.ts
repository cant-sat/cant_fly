import { formatURL } from "./util.ts";

// LOCAL IP
const localIp = "192.168.100.177" 

const redirects: { [key: string]: string } = {
  "/": "/index.html"
}

const headers: { [key: string]: { name: string, value: string }[] } = {
  ".html": [{
    name: "Content-Type",
    value: "text/html"
  }],
  ".js": [{
    name: "Content-Type",
    value: "text/javascript"
  }],
  ".svg": [{
    name: "Content-Type",
    value: "image/svg+xml"
  }],
  ".css": [{
    name: "Content-Type",
    value: "text/css"
  }]

}

const cache: { [key: string]: Response } = {}


Deno.serve(
  { port: 80, hostname: localIp},
  (req) => {

    let url = formatURL(req.url)

    if (redirects[url]) {
      url = redirects[url]
    }

    if (cache[url]) {
      return cache[url].clone()
    }

    try {
      const file = Deno.readFileSync("./files" + url)
      const decoder = new TextDecoder("utf-8");
      const resp = new Response(decoder.decode(file))

      const extension = url.substring(url.lastIndexOf('.'));
      if (headers[extension]) {
        headers[extension].forEach(header => {
          resp.headers.set(header.name, header.value);
        });
      }

      cache[url] = resp

      return resp.clone()

    } catch (error) {
      console.log(error)

      const resp = new Response(null, { status: 404 })
      cache[url] = resp
      return resp.clone()
    }

  }
);

const sockets: WebSocket[] = []
let authenticatedSocket: WebSocket | null = null

const token: string = "token"

const data : {temp : number, angle : number[], acc : number[]}[] = []

Deno.serve(
  { port: 443, hostname: localIp },
  (req) => {

    
    if (req.headers.get("upgrade") != "websocket") {
      return new Response(null, { status: 501 });
    }

    const { socket, response } = Deno.upgradeWebSocket(req);

    socket.addEventListener("open", () => {
      sockets.push(socket)

      setTimeout(()=>{
        if(authenticatedSocket != socket && socket.readyState == socket.OPEN){
          socket.send(JSON.stringify(data))
          
        }
      }, 1000)
    });

    socket.addEventListener("message", (event) => {
      if(authenticatedSocket && authenticatedSocket.readyState != authenticatedSocket.OPEN){
        authenticatedSocket = null
      }
      console.log(event.data)

      if (authenticatedSocket == null && event.data == token) {
        authenticatedSocket = socket
        console.log("authenticated websocket")
      }
      else if (authenticatedSocket == null) {
        socket.close()
        console.log("token failed")
      } else if (authenticatedSocket == socket) {
        let t : {temp : number, angle : number[], acc : number[], timestamp: number}
        
        try {
          t = JSON.parse(event.data)

          if(!(t.temp && t.angle.length == 3 && t.acc.length == 3)){
            throw "all values are required"
          }
          data.push(t)
          
        } catch (error) {
          authenticatedSocket = null
          socket.close()
        }

        
        sockets.forEach((tSocket, i) => {
          if (tSocket != authenticatedSocket) {
            if (tSocket.readyState==tSocket.OPEN) {
              tSocket.send(event.data)
            }
            else {
              sockets.splice(i, 1)
            }
          }
        })
        console.log("sent data to "+sockets.length + " clients")


      } else {


        console.log("authenticated socket exists and this one is not it")
        socket.close()
      }
    });

    socket.addEventListener("close", (event) => {
      if (authenticatedSocket == socket) {
        authenticatedSocket == null
        console.log("unauthenticated websocket")
      }
      const index = sockets.indexOf(socket, 0);
      if (index > -1) {
        sockets.splice(index, 1);
      }


    })

    socket.addEventListener("error", (event) => {
      if (authenticatedSocket == socket) {
        authenticatedSocket == null
        
      }
      const index = sockets.indexOf(socket, 0);
      if (index > -1) {
        sockets.splice(index, 1);
      }
    })

    return response;
  }
);