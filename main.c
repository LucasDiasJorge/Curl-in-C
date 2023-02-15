#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <unistd.h>

int main(void)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;

    curl = curl_easy_init();
    
    if (curl)
    {
        fp = fopen("lista.txt", "r");
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, fp)) != -1)
        {

            printf("%ld\n",len);

            curl_easy_setopt(curl, CURLOPT_URL, "http://10.0.0.155:8888/api/v1/ping");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, line);

            res = curl_easy_perform(curl);

            /* Verifica se a requisição foi bem-sucedida */
            if (res != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
                break;
            }

            remove("lista.txt");

            sleep(10);
            
        }

        curl_easy_cleanup(curl);
        free(line);
        fclose(fp);
    }
    return 0;
}
