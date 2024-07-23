#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define USERPWD_FILE "userpwd.txt"
#define USERPWD_MAX_LEN 128

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
    // Handle the response data here if needed
    return size * nmemb;
}

int main(void) {
    CURL *curl;
    CURLcode res;
    FILE *file;
    char userpwd[USERPWD_MAX_LEN];

    // Open the file containing the username and password
    file = fopen(USERPWD_FILE, "r");
    if (!file) {
        perror("Failed to open userpwd file");
        return EXIT_FAILURE;
    }

    // Read the username and password from the file
    if (fgets(userpwd, USERPWD_MAX_LEN, file) == NULL) {
        perror("Failed to read userpwd file");
        fclose(file);
        return EXIT_FAILURE;
    }
    fclose(file);

    // Remove the newline character if it exists
    size_t len = strlen(userpwd);
    if (len > 0 && userpwd[len - 1] == '\n') {
        userpwd[len - 1] = '\0';
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/example");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return 0;
}
