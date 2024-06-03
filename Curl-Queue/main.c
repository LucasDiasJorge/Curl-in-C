#include <stdio.h>
#include <curl/curl.h>

#define MAX_SIZE 100

typedef struct {
    CURL *curls[MAX_SIZE];
    int front, rear;
} Queue;

// Função para inicializar a fila
void initializeQueue(Queue *queue) {
    queue->front = -1;
    queue->rear = -1;
}

// Função para verificar se a fila está vazia
int isEmpty(Queue *queue) {
    return (queue->front == -1);
}

// Função para verificar se a fila está cheia
int isFull(Queue *queue) {
    return ((queue->rear + 1) % MAX_SIZE == queue->front);
}

// Função para enfileirar um elemento
void enqueue(Queue *queue, CURL *value) {
    if (isFull(queue)) {
        printf("Erro: Fila cheia, não é possível enfileirar.\n");
        return;
    }

    if (isEmpty(queue)) {
        queue->front = 0;
    }
    queue->rear = (queue->rear + 1) % MAX_SIZE;
    queue->curls[queue->rear] = value;
}

// Função para desenfileirar um elemento
CURL* dequeue(Queue *queue) {
    if (isEmpty(queue)) {
        printf("Erro: Fila vazia, não é possível desenfileirar.\n");
        return NULL;
    }

    CURL *value = queue->curls[queue->front];
    if (queue->front == queue->rear) {
        // Último elemento na fila
        initializeQueue(queue);
    } else {
        queue->front = (queue->front + 1) % MAX_SIZE;
    }
    return value;
}

// Função para performar todas as requisições CURL na fila
void performAllCurls(Queue *queue) {
    while (!isEmpty(queue)) {
        CURL *curl = dequeue(queue);
        if (curl) {
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            curl_easy_cleanup(curl);
        }
    }
}

// Callback para escrever dados recebidos
size_t print_chunk(char *buffer, size_t itemsize, size_t nitems, void *ignorethis) {
    size_t bytes = itemsize * nitems;
    printf("New chunk (%zu bytes)\n\n", bytes);

    int linenumber = 1;
    for (size_t i = 0; i < bytes; i++) {
        printf("%c", buffer[i]);
        if (buffer[i] == '\n') {
            printf("%d:", ++linenumber);
        }
    }
    printf("\n\n");

    return bytes;
}

// Função para inicializar um CURL e configurá-lo
CURL* init_curl(const char* url) {
    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_chunk);
    }
    return curl;
}

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    Queue myQueue;
    initializeQueue(&myQueue);

    const char* url = "http://localhost:8889/api/v2/core/ping";
    enqueue(&myQueue, init_curl(url));
    enqueue(&myQueue, init_curl(url));
    enqueue(&myQueue, init_curl(url));

    performAllCurls(&myQueue);

    curl_global_cleanup();

    return 0;
}
