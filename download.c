#include <stdio.h>
#include <curl/curl.h>

int main(void)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;

    // Abre o arquivo para salvar o conteúdo
    fp = fopen("arquivo.zip", "wb");

    // Inicializa a biblioteca libcurl
    curl = curl_easy_init();
    if(curl) {
        // Define a URL de onde o arquivo será baixado
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/arquivo.zip");

        // Define a função para escrever os dados no arquivo
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);

        // Define o arquivo onde os dados serão escritos
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        // Executa a transferência
        res = curl_easy_perform(curl);

        // Finaliza a biblioteca libcurl
        curl_easy_cleanup(curl);

        // Fecha o arquivo
        fclose(fp);

        // Verifica se houve erro na transferência
        if(res != CURLE_OK) {
            fprintf(stderr, "Erro ao baixar arquivo: %s\n", curl_easy_strerror(res));
            return 1;
        }
    }

    return 0;
}
