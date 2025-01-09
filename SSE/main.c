#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define URL "http://localhost:8892/sse/black-list/1"
#define AUTH_TOKEN "Bearer eyJhbGciOiJSUzI1NiIsInR5cCIgOiAiSldUIiwia2lkIiA6ICJxZ1FhZDZUUi1JMXVUNm40Tlo3YUh5LTZTSjltcXNxd3lSbkZMUmU0OUUwIn0.eyJleHAiOjE3MzY0Njc0OTMsImlhdCI6MTczNjQ0OTQ5MywianRpIjoiNmYwNjViMDQtNzlhMy00Njk2LWFkZmYtNDhjOTQ3NDVjMzUzIiwiaXNzIjoiaHR0cDovL2xvY2FsaG9zdDo4MDgwL3JlYWxtcy9teS1yZWFsbSIsImF1ZCI6ImFjY291bnQiLCJzdWIiOiJmNmYwODQ2NC0wNGY5LTQwMmItYjI3Zi0yYzJkOWQ4Yjk3NTQiLCJ0eXAiOiJCZWFyZXIiLCJhenAiOiJteS1jbGllbnQiLCJzZXNzaW9uX3N0YXRlIjoiZjMwYTMwZjEtODA5NC00MDhmLThhZDEtNTRlMTk0N2Q1MzFmIiwiYWNyIjoiMSIsImFsbG93ZWQtb3JpZ2lucyI6WyIvKiJdLCJyZWFsbV9hY2Nlc3MiOnsicm9sZXMiOlsib2ZmbGluZV9hY2Nlc3MiLCJ1bWFfYXV0aG9yaXphdGlvbiIsImRlZmF1bHQtcm9sZXMtbXktcmVhbG0iXX0sInJlc291cmNlX2FjY2VzcyI6eyJhY2NvdW50Ijp7InJvbGVzIjpbIm1hbmFnZS1hY2NvdW50IiwibWFuYWdlLWFjY291bnQtbGlua3MiLCJ2aWV3LXByb2ZpbGUiXX19LCJzY29wZSI6Im9wZW5pZCBlbWFpbCBwcm9maWxlIiwic2lkIjoiZjMwYTMwZjEtODA5NC00MDhmLThhZDEtNTRlMTk0N2Q1MzFmIiwiZW1haWxfdmVyaWZpZWQiOmZhbHNlLCJuYW1lIjoiTHVjYXMgSm9yZ2UiLCJwcmVmZXJyZWRfdXNlcm5hbWUiOiJsdWNhcyIsImdpdmVuX25hbWUiOiJMdWNhcyIsImZhbWlseV9uYW1lIjoiSm9yZ2UiLCJlbWFpbCI6Imx1Y2FzQGdtYWlsLmNvbSJ9.CEDCQsK4eh5WHCpJNAxO80avwjGPc-nEaThe0ZVcqTqyOJV5g_7UJ3fv8o8QVrCE1hLCMhw3x3WlmAlcrvwWzVhZ3JA-GQO_Ihrl8SKCA0B8VhXH2K2fM_JmYEk74lABjUNcg-HVa7hk7knXnO9fzJqxcgJ0rhFUxvQVXe-WGjp4G7ojx-ChuBqixDz_Pz0StJ5pEfy90cA-OLhazYqZhN8jbi5Xj2uTenl4qnOa5hIVID6oMZdkWGoo6dcw_ijygC4KtRdRzQasveYYPlk_OKLJaVj6GCVJbLPBWNH9Nsyw5gomeq5ZCjZo5VTopATg8EyLpprUHgYfS5ty6ZH0Ig"

struct Memory {
    char *data;
    size_t size;
};

// Função para armazenar os dados recebidos
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    char *ptr = realloc(mem->data, mem->size + total_size + 1);
    if (ptr == NULL) {
        fprintf(stderr, "Error reallocating memory\n");
        return 0;
    }

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, total_size);
    mem->size += total_size;
    mem->data[mem->size] = '\0';

    printf("%s\n",mem->data);

    return total_size;
}

int main() {
    CURL *curl;
    CURLcode res;

    struct Memory chunk = {0};

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Configurar URL
        curl_easy_setopt(curl, CURLOPT_URL, URL);

        // Adicionar cabeçalhos
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: text/event-stream");
        headers = curl_slist_append(headers, "Authorization: " AUTH_TOKEN);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Configurar callback para capturar os dados
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

        // Executar requisição
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("Received data:\n%s\n", chunk.data);
        }

        // Limpar
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    free(chunk.data);
    curl_global_cleanup();

    return 0;
}
