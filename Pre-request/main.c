#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

#define MAX_RESPONSE_SIZE 1024
#define MAX_LINE_LENGTH 100

size_t print_chunk(char *buffer, size_t itemsize, size_t nitems, void *ignorethis) {
    size_t bytes = itemsize * nitems;

    printf("New chunk (%zu bytes)\n\n", bytes);

    int linenumber = 1;
    for (size_t i = 0; i < bytes; i++) {
        printf("%c", buffer[i]);

        if (buffer[i] == '\n') {
            printf("%d:", ++linenumber);
        }
    }

    printf("\n\n");

    return bytes;
}

const char *get_properties(const char *key) {
    FILE *file = fopen("example.properties", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    char auth_path[MAX_LINE_LENGTH] = "";
    char auth_user[MAX_LINE_LENGTH] = "";

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " =\n");
        if (token != NULL) {
            if (strcmp(token, "AUTH_PATH") == 0) {
                token = strtok(NULL, " =\n");
                if (token != NULL) {
                    strcpy(auth_path, token);
                }
            } else if (strcmp(token, "AUTH_USER") == 0) {
                token = strtok(NULL, " =\n");
                if (token != NULL) {
                    strcpy(auth_user, token);
                }
            }
        }
    }

    fclose(file);

    if (strcmp(key, "path") == 0) {
        return auth_path;
    } else if (strcmp(key, "user") == 0) {
        return auth_user;
    } else {
        fprintf(stderr, "Invalid key\n");
        exit(EXIT_FAILURE);
    }
}

void http_auth(CURL *curl, CURLcode res, const char *data) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, get_properties("path"));
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_chunk);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
}

void httpGet(CURL *curl, CURLcode res, const char *url) {
    const char *token = "";

    http_auth(curl, res, token);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, strcat("Authorization: Bearer ", token));

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_chunk);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
}

void httpPost(CURL *curl, CURLcode res, const char *url, const char *data) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_chunk);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
}

void httpDelete(CURL *curl, CURLcode res, const char *url) {
    struct curl_slist *headers = NULL;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_chunk);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
}

void httpPut(CURL *curl, CURLcode res, const char *url, const char *data) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_chunk);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
}

int main(int argc, char *argv[]) {
    CURL *curl;
    CURLcode res;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <operation> <url>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char operation = *argv[1];
    const char *url = argv[2];
    const char *data = "{}";
    sprintf(data, "{\"email\":\"%s\",\"pass\":\"%s\"}", get_properties("user"), get_properties("path"));

    curl = curl_easy_init();
    if (curl) {
        switch(operation) {
            case 'g':
                httpGet(curl, res, url);
                break;
            case 'p':
                httpPost(curl, res, url, data);
                break;
            case 'd':
                httpDelete(curl, res, url);
                break;
            case 'u':
                httpPut(curl, res, url, data);
                break;
            default:
                fprintf(stderr, "Invalid operation\n");
                exit(EXIT_FAILURE);
        }
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Failed to initialize curl\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
