#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#define MAX_RESPONSE_SIZE 1024

size_t got_data(char *buffer, size_t itemsize, size_t nitems, void* ignorethis) {
    size_t bytes = itemsize * nitems;

    int linenumber = 1;

    printf("New chunk (%zu bytes)\n\n", bytes);

    printf("%d:", linenumber);

    for(int i = 0; i < bytes; i++) {
        printf("%c", buffer[i]);

        if (buffer[i] == '\n') {
            linenumber++;
            printf("%d:", linenumber);
        }
    }

    printf("\n\n");

    return bytes;
}

void getError(CURLcode res) {
    fprintf(stderr, "Error: %s\n", curl_easy_strerror(res));
}

void httpGet(CURL *curl, CURLcode res, const char *url) {

    struct curl_slist *headers = NULL;

    // BEARER TOKEN
    //headers = curl_slist_append(headers, "Authorization: Bearer <TOKEN>");

    // BASIC AUTH
    //curl_easy_setopt(curl, CURLOPT_USERPWD, "root:root");
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);

    /*        PASS RESPONSE AS VARIABLE
 
    char response[MAX_RESPONSE_SIZE] = "";
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    
    */
    
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        getError(res);
    }

    curl_slist_free_all(headers);
}

void httpPost(CURL *curl, CURLcode res, const char *url, const char *data) {

    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    // BEARER TOKEN
    //headers = curl_slist_append(headers, "Authorization: Bearer <TOKEN>");

    // BASIC AUTH
    //curl_easy_setopt(curl, CURLOPT_USERPWD, "root:root");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);

    // Set the data to be sent
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        getError(res);
    }

    curl_slist_free_all(headers);
}

void httpDelete(CURL *curl, CURLcode res, const char *url) {

    struct curl_slist *headers = NULL;

    // BEARER TOKEN
    //headers = curl_slist_append(headers, "Authorization: Bearer <TOKEN>");

    // BASIC AUTH
    //curl_easy_setopt(curl, CURLOPT_USERPWD, "root:root");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        getError(res);
    }

    curl_slist_free_all(headers);
}

void httpPut(CURL *curl, CURLcode res, const char *url, const char *data) {

    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    // BEARER TOKEN
    //headers = curl_slist_append(headers, "Authorization: Bearer <TOKEN>");

    // BASIC AUTH
    //curl_easy_setopt(curl, CURLOPT_USERPWD, "root:root");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);

    // Set the data to be sent
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        getError(res);
    }

    curl_slist_free_all(headers);
}

int main(int argc, char *argv[]) {

    CURL *curl;
    CURLcode res;

    if (argc < 3) {
        perror("Usage: ./your_program <operation> <url>");
        exit(EXIT_FAILURE);
    }

    char operation = *argv[1];
    const char *url = argv[2];

    // TO-DO
    const char *data = "{}";

    curl = curl_easy_init();

    if (curl) {
        if (operation == 'g') {
            httpGet(curl, res, url);
        } else if (operation == 'p') {
            httpPost(curl, res, url, data);
        } else if (operation == 'd') {
            httpDelete(curl, res, url);
        } else if (operation == 'u') {
            httpPut(curl, res, url, data);
        } else {
            perror("Invalid operation");
            exit(EXIT_FAILURE);
        }

        curl_easy_cleanup(curl);
    }

    return 0;
}
