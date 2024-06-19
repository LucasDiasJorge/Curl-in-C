#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Estrutura para armazenar dados da resposta HTTP
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Função callback para escrever dados da resposta HTTP em memória
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userdata;

    char *ptr_memory = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr_memory == NULL) {
        fprintf(stderr, "Erro ao realocar memória\n");
        return 0;
    }

    mem->memory = ptr_memory;
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main(void) {
    CURL *hnd = curl_easy_init();
    CURLcode ret;

    if (hnd) {
        // URL da requisição
        curl_easy_setopt(hnd, CURLOPT_URL, "https://mio.app.br/api/v1/authenticate?=");

        // Método da requisição
        curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");

        // Headers da requisição
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: insomnia/9.2.0");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Basic YWN0ZW1pdW1fcmZpZGJyYV9xYTpAenE6UXt1Q2tRVXc6Kz5R");
        curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

        // Estrutura para armazenar os dados da resposta
        struct MemoryStruct chunk;
        chunk.memory = malloc(1);  // Inicializa com tamanho mínimo
        chunk.size = 0;

        // Configura a função callback para escrever os dados em memória
        curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *)&chunk);

        // Configura para incluir os cabeçalhos na saída
        curl_easy_setopt(hnd, CURLOPT_HEADER, 1L);

        // Executa a requisição HTTP
        ret = curl_easy_perform(hnd);
        if (ret != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() falhou: %s\n", curl_easy_strerror(ret));
        } else {
            // Captura o Content-Type da resposta
            char *content_type;
            ret = curl_easy_getinfo(hnd, CURLINFO_CONTENT_TYPE, &content_type);
            if (ret == CURLE_OK && content_type) {
                // Decide como salvar a resposta baseado no Content-Type
                if (strstr(content_type, "application/json")) {
                    FILE *response_file = fopen("response.json", "wb");
                    if (response_file) {
                        fwrite(chunk.memory, chunk.size, 1, response_file);
                        fclose(response_file);
                        printf("Resposta salva como 'response.json'\n");
                    } else {
                        fprintf(stderr, "Erro ao abrir arquivo 'response.json' para escrita\n");
                    }
                } else if (strstr(content_type, "application/xml")) {
                    FILE *response_file = fopen("response.xml", "wb");
                    if (response_file) {
                        fwrite(chunk.memory, chunk.size, 1, response_file);
                        fclose(response_file);
                        printf("Resposta salva como 'response.xml'\n");
                    } else {
                        fprintf(stderr, "Erro ao abrir arquivo 'response.xml' para escrita\n");
                    }
                } else if (strstr(content_type, "text/html")) {
                    FILE *response_file = fopen("response.html", "wb");
                    if (response_file) {
                        fwrite(chunk.memory, chunk.size, 1, response_file);
                        fclose(response_file);
                        printf("Resposta HTML salva como 'response.html'\n");
                    } else {
                        fprintf(stderr, "Erro ao abrir arquivo 'response.html' para escrita\n");
                    }
                }else {
                    FILE *response_file = fopen("response.txt", "wb");
                    if (response_file) {
                        fwrite(chunk.memory, chunk.size, 1, response_file);
                        fclose(response_file);
                        printf("Resposta salva como 'response.txt'\n");
                    } else {
                        fprintf(stderr, "Erro ao abrir arquivo 'response.txt' para escrita\n");
                    }
                }
            }
        }

        // Libera os recursos
        curl_easy_cleanup(hnd);
        curl_slist_free_all(headers);
        free(chunk.memory);
    }

    return 0;
}
