#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <curl/curl.h>

int main() {
    CURL *curl;
    CURLcode res;
    char auth_token[512];

    // Solicitar o token de autorização via entrada do usuário
    printf("Digite o token de autorização: ");
    scanf("%511s", auth_token);  // Limitar a leitura para evitar overflow

    // Iniciar o libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "Falha ao inicializar libcurl\n");
        return 1;
    }

    while (1) {
        char* data = "data";

        // Criar dados JSON com o data
        char json_data[256];
        snprintf(json_data, sizeof(json_data),
                 "{\"id\": null, \"product\": {\"id\": 1}, \"location\": {\"id\": 1}, \"data\": \"%s\"}",
                 data);

        // Configurar a URL e os headers
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8889/api/v2/data");

        // Construir o cabeçalho de autorização
        char auth_header[600];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", auth_token);

        // Definir os cabeçalhos
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, auth_header);  // Adicionar o cabeçalho de autorização
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Definir os dados a serem enviados (POST)
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        // Fazer a requisição
        res = curl_easy_perform(curl);

        // Verificar se ocorreu algum erro
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() falhou: %s\n", curl_easy_strerror(res));
        }

        // Limpar a lista de headers
        curl_slist_free_all(headers);

        // Dormir por 60 segundos antes da próxima iteração
        sleep(60);
    }

    // Limpar e finalizar o libcurl
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}
