# Using lib curl in C

### Done:

  - [x] Print html of "https://example.com/"
  
  - [x] Arduino application to make a get on a project of my own 

### Next step:

1. Making REST methods with curl
  
      - [x] Post
  
      - [x] Get
  
      - [ ] Put
  
      - [ ] Delete
    
      - [ ] Patch
  
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
   ./main
   ```   
 

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

Used in embedded projects with Arduino and similar.

_For more examples, please refer to the [Documentation](https://curl.se/docs/)_

_For post, used [Curl post](https://curl.se/libcurl/c/http-post.html)_

<p align="right">(<a href="#readme-top">back to top</a>)</p>
