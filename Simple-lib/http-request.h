#define HTTP_REQUEST_H
#ifndef HTTP_REQUEST_H

#define MAX_RESPONSE_SIZE 1024

char response[MAX_RESPONSE_SIZE];

send_curl_request(const char *method, const char *url, char response_ref[MAX_RESPONSE_SIZE]);

#endif