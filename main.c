#include <stdio.h>
#include <curl/curl.h>
 
size_t got_data(char *buffer, size_t itemsize, size_t nitems, void* ignorethis){
  
  size_t bytes = itemsize * nitems;

  int linenumber = 1;

  printf("New chunck (%zu bytes)\n\n", bytes);

  printf("%d:",linenumber);

  for(int i=0; i < bytes; i++){
    
    printf("%c",buffer[i]);
    
    if(buffer[i] == '\n'){
      linenumber++;
      printf("%d:",linenumber);
    }
  }

  printf("\n\n");

  return bytes;
}

void getError(CURLcode res) {
  fprintf(stderr, "Error: %s\n", curl_easy_strerror(res));
}

int main(int argc, char *argv[]){
  CURL *curl;
  CURLcode res;
 
  curl = curl_easy_init();
  
  char operation = *argv[1];

  //Get
  if(curl && operation == 'g') {

    struct curl_slist *headers = NULL;

    //headers = curl_slist_append(headers, "Authorization: Bearer <TOKEN>");

    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.65:8888/api/v1/ping");
    //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);
    
    
    res = curl_easy_perform(curl);
    
    if(res != CURLE_OK) {
        getError(res);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  //Post
  if(curl && operation == 'p') {

    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    //headers = curl_slist_append(headers, "Authorization: Bearer <TOKEN>");

    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.65:8888/api/v1/auth");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"user\":\"email@email.com\",\"pass\":\"easypass\"}");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
        getError(res);
    }
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);  
  }

  //Delete
  if(curl && operation == 'd') {

    struct curl_slist *headers = NULL;

    //headers = curl_slist_append(headers, "Authorization: Bearer <TOKEN>");

    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.65:8888/api/v1/delete?id=3");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST,"DELETE");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
        getError(res);
    }
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  //Put
  if(curl && operation == 'u') {

    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    //headers = curl_slist_append(headers, "Authorization: Bearer <TOKEN>");

    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.65:8888/api/v1/update");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"key\":\"value\"}");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
        getError(res);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  return 0;
}