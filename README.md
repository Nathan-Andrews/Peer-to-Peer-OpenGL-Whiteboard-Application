# Peer-to-Peer-OpenGL-Whiteboard-Application

## Setup
### Windows Setup
1. Clone the repository
2. Install Boost

   a. [Download](https://www.boost.org/users/history/version_1_86_0.html) the 1.86.0 version for windows
   
   b. Extract the contents of the downloaded zip into `C:\boost_1_86_0`
4. Install GLFW

   a. [Download](https://glfw.org/download) the windows pre-compiled binaries
   
   b. Extract the contents of the downloaded zip into `C:\GLFW`

### Macos Setup
1. Clone the repository
2. Install Boost *Note: this is likely already installed*

   a. install with brew `brew install boost`
4. Install GLFW

   a. install with brew `brew install glfw`


## Running
### Running with visual studio
1. Just run like any other project in Visual Studio

### Running without visual studio
1. Compile code `make`
2. Run code `./app`

   

## Development
### Network communication
**Server Side**

To create a server, instantiate a Server class.  No whiteboard data is sent through this server.  It is used to let peers know what IP and port they should use to connect to each other.  These connections are handled automatically.

`Server.GetIP()` and `Server.GetPort()` are used to get the IP and port that can be used for clients to connect to the server.  This can be used to generate a the code that lets users join.

Example:
```
void serverThreadFunction() {
   Server server;
   std::cout "server running on "
      << server.GetIP()
      << ":"
      << server.GetPort()
      << std::endl;
}

int main() { std::thread(serverThreadFunction).detach()); }
```


**Client Side**

All of the client-side operations are done through the ConnectionManager class.  This can be constructed using the IP and port of the server.  For example `ConnectionManager manager("192.168.1.186",50318);`.

The connection manager will automatically connect to, and disconnect from any other peers, and can send and receive messsages from them.

`ConnectionManager.Write(std::string)` and `ConnectionManager.Write(char*)` both broadcast a message to all connected peers.  This can be used to send information about actions on the whiteboard.

`std::string ConnectionManager.Read()` and `ConnectionManager.Read(char*)` both read the next message from the other peers.  This can be used to recieve information about actions on the whiteboard from another user.

`bool isConnected` is true if the client is connected to the server, and false if the server has disconnected.  When the server is disconnected, the client manager will automatically disconnect from the other clients.

Example:
```
int writeThread(ConnectionManager& manager) {
    std::string message;

    while (true) {
        std::getline(std::cin, message);
        
        manager.Write(message); // sending message to all other peers
    }
}

int main() {
   ConnectionManager manager("192.168.1.186",50318);

   std::thread(writeThread,std::ref(manager)).detach();

   while (true) {
       std::string message = manager.Read();  // receiving message from another peer
       std::cout << "recieved messsage: " << message << std::endl;
   }
}

```
