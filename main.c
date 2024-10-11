#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_RESPONSE_SIZE 1024

typedef struct {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers;
} HttpRequest;

size_t got_data(char *buffer, size_t itemsize, size_t nitems, void* ignorethis) {
    size_t bytes = itemsize * nitems;
    int linenumber = 1;
    printf("New chunk (%zu bytes)\n\n", bytes);
    printf("%d:", linenumber);
    for (int i = 0; i < bytes; i++) {
        printf("%c", buffer[i]);
        if (buffer[i] == '\n') {
            linenumber++;
            printf("%d:", linenumber);
        }
    }
    printf("\n\n");
    return bytes;
}

void get_error(CURLcode res) {
    fprintf(stderr, "Error: %s\n", curl_easy_strerror(res));
}

void init_request(HttpRequest *req, const char *url) {
    req->curl = curl_easy_init();
    req->headers = NULL;
    curl_easy_setopt(req->curl, CURLOPT_URL, url);
    curl_easy_setopt(req->curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(req->curl, CURLOPT_WRITEFUNCTION, got_data);
    curl_easy_setopt(req->curl, CURLOPT_FOLLOWLOCATION, 1L); // Permite redirecionamento
    curl_easy_setopt(req->curl, CURLOPT_DEFAULT_PROTOCOL, "https");
}

void print_response_status_code(const HttpRequest *req) {
    if(req->res == CURLE_OK) {
        long http_code = 0;
        CURLcode info_res = curl_easy_getinfo(req->curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (info_res != CURLE_OK) {
            printf("Failed to get HTTP response code: %s\n", curl_easy_strerror(info_res));
            return;
        }
        switch (http_code) {
            case 200:
                printf("Request successful (HTTP 200)\n");
                break;
            case 204:
                printf("Request successful, but no content (HTTP 204)\n");
                break;
            case 401:
                printf("Unauthorized access (HTTP 401)\n");
                break;
            case 404:
                printf("Resource not found (HTTP 404)\n");
                break;
            case 500:
                printf("Internal server error (HTTP 500)\n");
                break;
            default:
                printf("Request successful, HTTP code: %ld\n", http_code);
                break;
        }
    } else {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(req->res));
    }
}

void perform_request(HttpRequest *req) {
    req->res = curl_easy_perform(req->curl);

    if (req->res != CURLE_OK) {
        get_error(req->res);
    }

    print_response_status_code(req);

    curl_slist_free_all(req->headers);
    curl_easy_cleanup(req->curl);
}

void set_headers(HttpRequest *req, const char **headers, size_t num_headers) {
    for (size_t i = 0; i < num_headers; i++) {
        req->headers = curl_slist_append(req->headers, headers[i]);
    }
    curl_easy_setopt(req->curl, CURLOPT_HTTPHEADER, req->headers);
}

void set_auth(HttpRequest *req, int auth_type) {
    char username[100], password[100], token[256], api_key[128], cookie[256];
    switch (auth_type) {
        case 1:
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);
            curl_easy_setopt(req->curl, CURLOPT_USERPWD, strcat(strcat(username, ":"), password));
            break;
        case 2:
            printf("Enter Bearer token: ");
            scanf("%s", token);
            char bearer_header[300] = "Authorization: Bearer ";
            strcat(bearer_header, token);
            req->headers = curl_slist_append(req->headers, bearer_header);
            break;
        case 3:
            printf("Enter API Key: ");
            scanf("%s", api_key);
            char api_key_header[300] = "API-Key: ";
            strcat(api_key_header, api_key);
            req->headers = curl_slist_append(req->headers, api_key_header);
            break;
        default:
            printf("No authentication selected\n");
    }
    curl_easy_setopt(req->curl, CURLOPT_HTTPHEADER, req->headers);
}

void http_post(HttpRequest *req, const char *data) {
    const char *headers[] = {"Content-Type: application/json"};
    set_headers(req, headers, 1);
    curl_easy_setopt(req->curl, CURLOPT_POSTFIELDS, data);
    perform_request(req);
}

int main() {
    char url[256], method[10], data[1024] = "{}";
    int auth_type;

    printf("Enter the URL: ");
    scanf("%s", url);

    printf("Enter the method (GET, POST, PUT, DELETE): ");
    scanf("%s", method);

    printf("Enter the type of authentication (0 for None, 1 for Basic Auth, 2 for Bearer Token, 3 for API Key): ");
    scanf("%d", &auth_type);

    if (strcasecmp(method, "POST") == 0 || strcasecmp(method, "PUT") == 0) {
        printf("Enter the data to be sent: ");
        scanf(" %[^\n]%*c", data); // Lê a linha inteira, incluindo espaços
    }

    HttpRequest req;
    init_request(&req, url);
    set_auth(&req, auth_type);

    if (strcasecmp(method, "POST") == 0) {
        http_post(&req, data);
    } else {
        fprintf(stderr, "Invalid method\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
