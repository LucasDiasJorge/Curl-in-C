#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_RESPONSE_SIZE 4096

// Variável global para armazenar a resposta
char global_response[MAX_RESPONSE_SIZE];

// Função callback para receber os dados da resposta HTTP
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    if (total_size > MAX_RESPONSE_SIZE - 1) {
        // Não há espaço suficiente para armazenar toda a resposta
        fprintf(stderr, "Resposta muito grande para armazenar\n");
        return 0;
    }
    memcpy(global_response, ptr, total_size);
    global_response[total_size] = '\0'; // Adicionando terminador de string
    return total_size;
}

// Função para realizar uma solicitação HTTP usando libcurl
void perform_http_request(const char *url) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Erro ao realizar a solicitação: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
}

int main() {
    // Exemplo de uso da função para realizar uma solicitação HTTP
    const char *url = "http://example.com";
    perform_http_request(url);

    // Exemplo de acesso à variável global com a resposta
    printf("Resposta da solicitação HTTP: %s\n", global_response);

    return 0;
}
