#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

int main(int argc, char* argv[])
{
  CURL *curl;
  CURLcode res;

  if (argc != 2) {
    printf("Usage: %s <timeoutInMs>\n", argv[0]);
    return 1;
  }

  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8889/api/v2/core/ping");
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, atol(argv[1]));

  res = curl_easy_perform(curl);

  if (CURLE_OK == res)
    printf("Success.\n");
  else if (CURLE_OPERATION_TIMEDOUT == res)
    printf("Operation timed out.\n");

  curl_easy_cleanup(curl);
  return 0;
}