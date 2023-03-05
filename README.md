# Using lib curl in C

### Done:

  - [x] Print html of "https://example.com/"
  
  - [x] Arduino application to make a get on a project of my own 

### Next step:

1. Making REST methods with curl (main)
  
      - [x] Post
  
      - [x] Get
  
      - [x] Put
  
      - [x] Delete
      
2. Making Post from txt file (feature/post-file)
    
      - [x] Used on data stored in embedded Rasp pi memory
      
3. Refactor ifs to functions
      - [ ] Done ?

4. Create a linux command to use specifically own paths (kind of debug)
      - [ ] Done ?
  
  ### Installation

1. Linux only, for now
2. Clone the repo
   ```sh
   git clone https://github.com/LucasDiasJorge/Curl-in-C.git
   ```
3. Install Lib Curl
   ```sh
   sudo apt-get install libcurl4-openssl-dev
   ```
4. Compile the project
    ```sh
   gcc -o main main.c -lcurl
   ```
5. Exec main
    ```sh
   ./main 'method g, p, d or u'
   ```   

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

_For more examples, please refer to the [Documentation](https://curl.se/docs/)_

_For more http tutorials and etc, please refer to the [Documentation](https://developer.mozilla.org/)_

_User Agent [Refer](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/User-Agent)_

_For post, used [Curl post](https://curl.se/libcurl/c/http-post.html)_

<p align="right">(<a href="#readme-top">back to top</a>)</p>
