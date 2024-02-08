#include <stdio.h>
#include "http-request.h"

#define MAX_RESPONSE_SIZE 1024

char request_response[MAX_RESPONSE_SIZE] = "";

int main() {
    const char *url = "http://localhost:8891/api/v1/ping";

    if (send_curl_request("GET", url, request_response)) {
        printf("Failed to perform request.\n");
        return 1;
    }

    printf("Response: %s\n", request_response);

    return 0;
}