/*
    Shiv Malli
    Khylei Neal
    Allison Okomski

    Operating Systems Web Crawler
    April 27, 2023
*/



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <curl/curl.h>
#include <tidy/tidy.h>
#include <tidy/tidybuffio.h>
#include <unistd.h>

#define MAX_URL_LEN 1024


// defining struct url_node to store urls
typedef struct {
    // create url array to store the urls
    char url[MAX_URL_LEN];
} url_node;

// defining  struct url_queue to store a queue of urls
typedef struct {
    // create an array of url nodes
    url_node urls[MAX_URL_LEN];

    int front;
    int rear;
} url_queue;

// initalizing a queue of type url_queue
url_queue queue;

// declaring mutex for concurrent access to queue
pthread_mutex_t queue_mutex;

/*
    The init_queue function initalizes the queue
*/
void init_queue() {
    queue.front = -1;
    queue.rear = -1;

}

/*
    The is_empty function checks if the queue is empty
*/
int is_empty() {
    if (queue.front == -1) {
        return 1;
    }
    return 0;
}

/*
    The is_full function checks if the queue is full
*/
int is_full() {
    // if the front of the queue is at the beginning of the array
    // and the rear of the queue is at the end of the array,or if
    // the front of the queue is a higher index than the rear
    if ((queue.front == 0 && queue.rear == MAX_URL_LEN - 1) ||
        (queue.front == queue.rear + 1)) {
        return 1;
    }
    return 0;
}

/*
    The enqueue function adds a url to the rear of the queue
*/
void enqueue(char* url) {

    if (is_full()) {
        //printf("Error: Queue is full\n");
        return;
    }

    if (url[0] == '\0') {
        printf("Error: Empty URL\n");
        return;
        }

    if (is_empty()) {
        queue.front = 0;
        queue.rear = 0;
    }
    // if full queue
    else if (queue.rear == MAX_URL_LEN - 1) {
        // set the rear of the queue to the beginning of the array
        queue.rear = 0;
    }
    // store the url in the urls array at the rear position
    else {
        queue.rear++;
    }

    // copy the url to the queue
    strcpy(queue.urls[queue.rear].url, url);
}

/*
    The dequeue function removes a url from the front queue
*/
url_node dequeue() {
    // create a new url node
    url_node url;
//    pthread_mutex_lock(&queue_mutex);

    if (is_empty()) {
        printf("Error: Queue is empty\n");
  //      pthread_mutex_unlock(&queue_mutex);
        return url;
        }

    // copy the url from the front of the queue to url_node variable "url"
    strcpy(url.url, queue.urls[queue.front].url);
    // if only one url
    if (queue.front == queue.rear) {
        // reset the queue
        queue.front = -1;
        queue.rear = -1;

    } else if (queue.front == MAX_URL_LEN - 1) {
        // move the front of the queue to the beginning of the array
        queue.front = 0;
    // move the front of the queue to the next url
    } else {
        queue.front++;
    }
   // pthread_mutex_unlock(&queue_mutex);

   // return the dequeued url_node
    return url;
}

/*
    The write_cb function is a callback for the curl library's write functionality
    It handles the response data that is recieved from a URL request.
    Appends the recieved data to data buffer "output" which is of type TidyBuffer.
    params:
        ptr: a pointer to the recieved data buffer that contains the data
            recieved from the URL request.
        size: the size of each data element in the recieved data buffer (bytes)
        nmeb: number of data elements in the recieved data buffer
        data: a pointer to a user-defined data buffer that will be used to
            store the response data. (the downloaded html data)
*/
size_t write_cb(char* ptr, size_t size, size_t nmemb, void* data) {
    // cast the data argument to a TidyBuffer pointer
    TidyBuffer* output = (TidyBuffer*)data;

    // calculate the size of recieved data buffer
    size_t new_len = size * nmemb;

    // append the new data to the TidyBuffer
    tidyBufAppend(output, ptr, new_len);

    // return the size of the data
    return new_len;
}


/*
    check_ahref checks if the HTML element is an anchor tag and extracts
    the value of 'href' if it exists.
    param:
        node: an html element | TidyNode Object
*/
void check_ahref(TidyNode node)
{
    // if the node is an anchor tag (hyperlink)
    if (tidyNodeGetId(node) == TidyTag_A) {
        //printf("found a tag\n");
        // set href to the HTML attribute's value (the url)
        const char* href = tidyAttrValue(tidyAttrGetById(node, TidyAttr_HREF));
        // if its an http link
        if (href && strncmp(href, "https",5) == 0)
            {
                // dynamically allocate memory for the link
                char* link = (char*) malloc(sizeof(char) * strlen(href)+1);
                strcpy(link, href);
                printf("Link: %s\n\n", href);

                enqueue(link);

            }
    }
}

/*
    recurse_nodes recursively processes the child and sibling nodes of
    the HTML. It traverses through the entire HTML document.
    param:
        node: an html element | TidyNode Object
*/
void recurse_nodes(TidyNode node) {
    if (!node)
        return;

    // check if node is an anchor tag
    check_ahref(node);

    TidyNode next;

    // recurse through all the child nodes
    if (next = tidyGetChild(node))
        recurse_nodes(next);
    // recurse through all of the sibling nodes
    if (next = tidyGetNext(node))
        recurse_nodes(next);
}

/*
    crawl is responsible for crawling the webpages. It dequeues the URLs
    from the queue, uses libcurl to download the page at that url, parses
    the HTML with libtidy, extracts links from the HTML, and enqueues
    the links for further processing.
*/
void* crawl(void* arg) {
    printf("crawl(%p) entered\n", arg);
    CURL* curl; // initialize a curl object
    TidyDoc doc; // initalize a TidyDoc object
    TidyBuffer output = {0}; // initalize a TidyBuffer Object
    char* url;

    while (1) {
        // lock the queue_mutex
        pthread_mutex_lock(&queue_mutex);
        // unlock the mutex, wait for 10ms and continue to next iteration
        if (is_empty()){
            pthread_mutex_unlock(&queue_mutex);
            usleep(10000);
            continue;
        }

        // dequeue a url from queue and store in url_node varible of type url_node
        url_node url_node = dequeue();
        // unlock the queue_mutex mutex
        pthread_mutex_unlock(&queue_mutex);
        // assign the url to the url variable
        url = url_node.url;
        printf("Processing URL: %s\n", url);
        // initalize a curl object
        curl = curl_easy_init();

        // if the curl address was initalized successfully
        if (curl) {
            // set the url to be crawled
            curl_easy_setopt(curl, CURLOPT_URL, url);
            // automatically follow HTTP redirects
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            // set the callback function for downloaded data
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
            // set the user pointer to be passed to write callback function
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);

            // execute the curl request
            CURLcode res = curl_easy_perform(curl);
            // if request was successful
            if (res == CURLE_OK) {
                // create a Tidy document object
                doc = tidyCreate();

                // save parsed and clean doc output to Tidy Doc 'output'
                tidyParseBuffer(doc,&output);
                tidySaveBuffer(doc, &output);

                // retrieve body and html info from doc
                TidyNode body = tidyGetBody(doc);
                TidyNode node = tidyGetChild(body);

                printf("entering while loop\n");

                // initialize a tidy buffer
                //TidyBuffer td;

                recurse_nodes(node);

                printf("after loop\n");

                // memory management and avoid leaks
                tidyBufFree(&output);
                printf("after free\n");
                tidyRelease(doc);
                printf("after release\n");
            } else {
                fprintf(stderr, "curl_easy_perform() failed: %s\n%s\n",curl_easy_strerror(res), url);
            }
            // release allocated resources
            curl_easy_cleanup(curl);
            printf("after cleanup\n");
        } else {
            fprintf(stderr, "curl_easy_init() failed\n");
        }
    }
    printf("done crawling");
}

/*
    The print_queue function prints the items in the queue.
*/
void print_queue() {
    int i;
    if (is_empty()) {
        printf("Queue is empty.\n");

    } else {
        printf("Queue items:\n");
        // iterate through the queue
        for (i = queue.front; i != queue.rear; i = (i + 1) % MAX_URL_LEN) {
            // print the urls
            printf("%s\n", queue.urls[i].url);
        }
        // print the last item in the queue
        printf("%s\n", queue.urls[i].url);
    }
}


int main(int argc, char** argv) {
    // initialize queue
    init_queue();
    // initialize mutex to default
    pthread_mutex_init(&queue_mutex, NULL);

    FILE* fp;
    char url[MAX_URL_LEN];
    // open urls.txt for reading
    fp = fopen("urls.txt", "r");
    // if the file does not exist, print error.
    if (fp == NULL) {
        printf("Error opening urls.txt\n");
        exit(1);
    }

    // redirect standard output to output file
    FILE* output_file = freopen("output.txt", "w", stdout);
    if (output_file == NULL) {
        printf("Error opening output file\n");
        exit(1);
    }

    // read the urls from the file and add them to the queue
    while (fgets(url, MAX_URL_LEN, fp) != NULL) {
        // remove newline character
        url[strcspn(url, "\n")] = 0;
        printf("Parsed URL: %s\n", url);
        enqueue(url);
    }

    //print_queue();
    // close the urls.txt file
    fclose(fp);

    pthread_t threads[10];
    int i;
    // create ten threads
    for (i = 0; i < 10; i++) {
        // allocate memory for thread_index
        int* thread_index = malloc(sizeof(int));
        *thread_index = i;
        printf("calling pthread_create...\n");
        // create thread and pass thread index to crawl function
        printf(
          "pthread_create returned %d\n",
          pthread_create(
            &threads[i], NULL, crawl,
            (void*)thread_index
          )
        );
    }

    // wait for all threads to finish
    for (i = 0; i < 10; i++) {
        printf("calling pthread_join threads[%d]...\n", i);
        // join threads so that it waits for each thread to finish
        // before the main thread exits
        printf(
          "pthread_join returned: %d\n",
          pthread_join(threads[i], NULL)
        );
    }
    // destroy mutex to avoid memory leaks
    pthread_mutex_destroy(&queue_mutex);

    // close the output file
    fclose(output_file);
    return 0;
}
