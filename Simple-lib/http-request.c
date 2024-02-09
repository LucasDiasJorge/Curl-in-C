#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_RESPONSE_SIZE 1024
#define USERNAME "root"
#define PASSWORD "root"

char response[MAX_RESPONSE_SIZE];

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    size_t buffer_size = strlen((char*)userdata);
    if (buffer_size + total_size < MAX_RESPONSE_SIZE) {
        strcat((char*)userdata, (char*)ptr);
    } else {
        fprintf(stderr, "Response buffer overflow!\n");
    }
    return total_size;
}

int send_curl_request(const char *method, const char *url, char *response_ref) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the request type (method)
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);

        // Set the username and password for basic authentication
        curl_easy_setopt(curl, CURLOPT_USERPWD, USERNAME ":" PASSWORD);

        // Set the write callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Pass the response buffer as userdata to the write callback
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        // Clear the response buffer
        response[0] = '\0';

        // Perform the request and check for errors
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return 1;
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }

    return 0;
}
