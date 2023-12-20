#include <stdio.h>
#include <curl/curl.h>

// Callback function to handle incoming data
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    // Handle the received data here, for example, print it to stdout
    fwrite(contents, size, nmemb, stdout);
    return realsize;
}

int main() {
    CURL *curl;
    CURLcode res;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Create a curl handle
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error initializing libcurl\n");
        return 1;
    }

    // Set the URL to stream data from
    curl_easy_setopt(curl, CURLOPT_URL, "http://10.0.1.118/api/v1/data/stream");

    // Set the callback function to handle incoming data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    // Set the username and password for basic authentication
    curl_easy_setopt(curl, CURLOPT_USERPWD, "root:root");

    // Perform the request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    // Clean up and close libcurl
    curl_easy_cleanup(curl);

    // Cleanup global state
    curl_global_cleanup();

    return 0;
}
