#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

#define MAX_RESPONSE_SIZE 1024
#define MAX_LINE_LENGTH 516
#define TOKEN_SIZE 256

char jwt_token[TOKEN_SIZE] = "";

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


int extractJsonValue(const char *json, const char *key, char *value, size_t max_length) {
    // Construct the search pattern with quotes around the key
    char pattern[256];
    snprintf(pattern, sizeof(pattern), "\"%s\":\"", key);

    const char *start = strstr(json, pattern);

    if (start != NULL) {
        start += strlen(pattern); // Move past the key and the characters `":`

        const char *end = strchr(start, '\"');
        if (end != NULL) {
            // Calculate the length of the value
            size_t length = end - start;

            // Check if the value fits in the buffer, including the null terminator
            if (length < max_length) {
                // Copy the value to the output buffer
                strncpy(value, start, length);
                value[length] = '\0'; // Add the null terminator

                printf("%s\n", value);

                return 0; // Success
            } else {
                fprintf(stderr, "Value size exceeds the provided buffer.\n");
            }
        } else {
            fprintf(stderr, "Invalid JSON format.\n");
        }
    } else {
        fprintf(stderr, "Key '%s' not found in JSON.\n", key);
    }

    return 1; // Failure
}

size_t store_token(void *buffer, size_t size, size_t nmemb, void *userdata) {

    size_t realsize = size * nmemb;

    extractJsonValue(buffer,"token",jwt_token,256);

    return realsize;

}

const char *get_properties(const char *key) {
    static char auth_path[MAX_LINE_LENGTH] = "";
    static char auth_user[MAX_LINE_LENGTH] = "";
    static char auth_pass[MAX_LINE_LENGTH] = "";

    FILE *file = fopen("example.properties", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];

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
            } else if (strcmp(token, "AUTH_PASS") == 0) {
                token = strtok(NULL, " =\n");
                if (token != NULL) {
                    strcpy(auth_pass, token);
                }
            }
        }
    }

    fclose(file);

    if (strcmp(key, "path") == 0) {
        return auth_path;
    } else if (strcmp(key, "user") == 0) {
        return auth_user;
    } else if (strcmp(key, "pass") == 0) {
        return auth_pass;
    } else {
        fprintf(stderr, "Invalid key\n");
        exit(EXIT_FAILURE);
    }
}

void http_auth(CURL *curl) {

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    CURLcode res; 

    curl_easy_setopt(curl, CURLOPT_URL, get_properties("path"));
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, store_token);

    char data[MAX_LINE_LENGTH];
    snprintf(data, sizeof(data), "{\"email\":\"%s\",\"pass\":\"%s\"}", get_properties("user"), get_properties("pass"));

    while (res != CURLE_OK) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        res = curl_easy_perform(curl);
        sleep(5);
    }

    curl_slist_free_all(headers);
    curl_global_cleanup();
}

void httpGet(CURL *curl, const char *url) {

    if (jwt_token[0] == '\0') {
        http_auth(curl);
    }

    char auth_header[MAX_LINE_LENGTH] = "Authorization: Bearer ";
    strcat(auth_header, jwt_token);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_chunk);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
}

void httpPost(CURL *curl, const char *url, const char *data) {
    if (jwt_token[0] == '\0') {
        http_auth(curl);
    }

    char auth_header[MAX_LINE_LENGTH] = "Authorization: Bearer ";
    strcat(auth_header, jwt_token);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_chunk);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
}

void httpDelete(CURL *curl, const char *url) {
    if (jwt_token[0] == '\0') {
        http_auth(curl);
    }

    char auth_header[MAX_LINE_LENGTH] = "Authorization: Bearer ";
    strcat(auth_header, jwt_token);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_chunk);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
}

void httpPut(CURL *curl, const char *url, const char *data) {
    if (jwt_token[0] == '\0') {
        http_auth(curl);
    }

    char auth_header[MAX_LINE_LENGTH] = "Authorization: Bearer ";
    strcat(auth_header, jwt_token);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_chunk);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
}

int main(int argc, char *argv[]) {
    CURL *curl;
    CURLcode res = CURLE_OK;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <operation> <url>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char operation = *argv[1];
    const char *url = argv[2];
    const char *data = "{}";

    curl = curl_easy_init();
    if (curl) {
        switch (operation) {
            case 'g':
                httpGet(curl, url);
                break;
            case 'p':
                httpPost(curl, url, data);
                break;
            case 'd':
                httpDelete(curl, url);
                break;
            case 'u':
                httpPut(curl, url, data);
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
