# WebServer

This WebServer project is an open-source initiative aimed at building a high-performance web server in C++. It's designed to handle multiple simultaneous connections, making it suitable for educational purposes and small to medium scale deployments.

## Features

- **HTTP/1.1 Support:** Handles basic HTTP/1.1 requests.
- **Concurrency:** Utilizes multi-threading to manage multiple connections simultaneously.
- **Logging:** Basic logging functionalities to track requests and responses.
- **Error Handling:** Proper handling of client and server errors.
- **Static Content:** Serves static web pages and files.

## Installation

Clone the repository to get started with the WebServer project:

```bash
git clone https://github.com/turman17/WebServer.git
cd WebServer
```

### Requirements

- C++ compiler (e.g., g++, clang++)
- Make
- Linux environment

### Compiling the project

Compile the project using:

```bash
make
```

This will create the executable named `webserver`.

## Usage

Run the server by executing:

```bash
./webserver
```

Once running, the server will listen on a predefined port and serve requests. You can visit `http://localhost:PORT/` in your web browser where `PORT` is the port number on which the server is listening.

## Authors

- [tlouro-c](https://github.com/tlouro-c)
- [turman17](https://github.com/turman17)
