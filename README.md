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
5. Install GLEW

   a. [Download](https://glew.sourceforge.net/) the windows pre-compiled binaries
   
   b. Extract the contents of the downloaded zip into `C:\GLEW`
   
![image-1](https://github.com/user-attachments/assets/d617eff4-4001-448a-ba5e-2b5108d9c54f)

5. Install GLM

   a. [Download](https://glm.g-truc.net/0.9.9/index.html) the windows pre-compiled binaries
   
   b. Extract the contents of the downloaded zip into `C:\GLM`
   
![image](https://github.com/user-attachments/assets/3c03097a-390f-4fc5-b5aa-9498ce22f3b8)

6. Install Qt

   a. [Download](https://www.qt.io/download-qt-installer-oss) the windows installer for the open source version (you may have to create an account)

   b. Run the installer and follow the installation steps.
   
   c. Enter `C:\Qt` as the install location when prompted

   d. Select `custom installation` and select `Qt 6.8`.  You can deselect the design studio and creator stuff

   e. Finish installing

   f. Add the `QT VS Tools` extension to visual studio

   g. Add QT version to the extension via the path to the installation, likely `C:\Qt\6.8.0\msvc2022_64`.  [More info](https://doc.qt.io/qtvstools/qtvstools-how-to-add-qt-versions.html)

   
### Macos Setup
1. Clone the repository
2. Install Boost

   a. install with brew `brew install boost`
3. Install GLFW

   a. install with brew `brew install glfw`
4. Install GLEW

   a. install with brew `brew install glew`
5. Install GLM

   a. install with brew `brew install glm`
6. Install Qt

   a. install with brew `brew install qt`

## Running
### Running with Windows
1. Open and run in Visual Studio

### Running with MacOS
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
