#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/stat.h> // Include this header for struct stat
#include <unistd.h> // Include this header for fstat

#define URL "http://example.com/upload" // Replace with your URL
#define FILEPATH "file.txt" // Replace with your file path

int main(void) {
    CURL *curl;
    CURLcode res;
    FILE *file;
    struct stat file_info;
    curl_off_t file_size;

    // Open the file
    file = fopen(FILEPATH, "rb");
    if (!file) {
        perror("fopen");
        return 1;
    }

    // Get the file descriptor
    int fd = fileno(file);
    if (fd == -1) {
        perror("fileno");
        fclose(file);
        return 1;
    }

    // Get the file size
    if (fstat(fd, &file_info) != 0) {
        perror("fstat");
        fclose(file);
        return 1;
    }
    file_size = (curl_off_t)file_info.st_size;

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        // Set URL for the POST
        curl_easy_setopt(curl, CURLOPT_URL, URL);

        // Enable uploading
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        // Set the file to upload
        curl_easy_setopt(curl, CURLOPT_READDATA, file);

        // Set the size of the file to upload
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, file_size);

        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);
        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Always cleanup
        curl_easy_cleanup(curl);
    }

    // Close the file
    fclose(file);

    // Cleanup curl global environment
    curl_global_cleanup();

    return 0;
}
