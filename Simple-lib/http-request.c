#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

int send_curl_request(const char *url, const char *method) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the request type (method)
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);

        // Set the username and password for basic authentication
        curl_easy_setopt(curl, CURLOPT_USERPWD, "root:root");

        // Perform the request and check for errors
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            printf("\n");
            return 1;
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }

    printf("\n");

    return 0;
}

int main() {

    const char *url = "http://localhost:8891/api/v1/ping";

    if (send_curl_request(url,"GET")) {
        return 1;
    }

    return 0;
}