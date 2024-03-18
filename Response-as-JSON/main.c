#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

// Write callback function to handle response data from cURL
size_t writeCallback(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

int main() {
    CURL *curl;
    CURLcode res;
    FILE *file;
    const char *url = "https://example.com/data"; // Replace with your URL
    const char *filename = "response.txt"; // Output filename

    curl = curl_easy_init();
    if (curl) {
        file = fopen(filename, "wb"); // Open file for writing in binary mode
        if (file) {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }

            fclose(file); // Close the file
        } else {
            fprintf(stderr, "Failed to open file %s for writing\n", filename);
        }
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Failed to initialize cURL\n");
    }

    return 0;
}
