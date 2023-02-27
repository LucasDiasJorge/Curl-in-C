#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
 
size_t got_data(char *buffer, size_t itemsize, size_t nitems, void* ignorethis){
  
  size_t bytes = itemsize * nitems;

  int linenumber = 1;

  printf("New chunck (%zu bytes)\n", bytes);

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


int main(int argc, char *argv[]){
  CURL *curl;
  CURLcode res;
 
  curl = curl_easy_init();
  
  char operation = *argv[1];

  //Get
  if(curl && operation == 'g') {
        curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.65:8888/api/v1/ping");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "Erro ao acessar a URL: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }

  //Post
  if(curl && operation == 'p') {

    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "user", "email@email.com");
    cJSON_AddStringToObject(root, "pass", "easypass");
    char *json_str = cJSON_Print(root);
    printf("%s\n", json_str);

    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.65:8888/api/v1/auth");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK){
      fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
    }
  
    cJSON_Delete(root);
    free(json_str);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  return 0;
}