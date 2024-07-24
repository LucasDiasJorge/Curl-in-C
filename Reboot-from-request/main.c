#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <unistd.h>

// Callback function to handle the data received from the request
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    return realsize;
}

// Function to perform the HTTP request
int perform_request(const char *url, long timeout) {
    CURL *curl;
    CURLcode res;
    int success = 0;

    curl = curl_easy_init();
    if (curl) {
        // Set URL for the request
        curl_easy_setopt(curl, CURLOPT_URL, url);
        // Set callback function to handle the data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        // Set timeout for the request
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        // Authentication
        curl_easy_setopt(curl, CURLOPT_USERPWD, "portal:RFIDBrasil");

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            success = 1; // Set to 1 if there is an error (timeout or other)
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }

    return success;
}

int main(void) {
    const char *url = "https://spal.rfidfacil.com/api/v2/reader/ping";
    long timeout = 20; // Timeout in seconds

    int result = perform_request(url, timeout);
    if (result == 1) {
        // Reboot the machine if the request fails (timeout or other error)
        printf("Request failed, rebooting the machine...\n");
        system("reboot");
    } else {
        printf("Request succeeded.\n");
    }

    return 0;
}
