#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curl/curl.h>
#include <jansson.h>

#define BUFFER_SIZE (256 * 1024) /* 256 KB */

struct memory {
   char *response;
   size_t size;
 };

static size_t cb(void *data, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct memory *mem = (struct memory *)userp;

  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if(ptr == NULL)
	return 0;  /* out of memory! */

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;

  return realsize;
}

struct memory chunk = {0};

char* returnupper(char* s) {
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = toupper((unsigned char) *tmp);
    }

    return s;
}

static void *request(const char *url) {
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
//	printf("%s\n",chunk);
	json_t *cardjson = NULL;
	json_error_t error;
	cardjson = json_loads(chunk.response,0,&error);
    if (json_is_object(cardjson)) {
	json_t *name = json_object_get(cardjson,"name");
	json_t *manacost = json_object_get(cardjson,"mana_cost");
	json_t *typeline = json_object_get(cardjson,"type_line");
	json_t *oracle = json_object_get(cardjson,"oracle_text");
	json_t *setname = json_object_get(cardjson,"set_name");
	json_t *set = json_object_get(cardjson,"set");
	char *lowercode = json_string_value(set);
	char *tmp = lowercode;
	char *setcode = returnupper(tmp);
	printf("%s\n",json_string_value(name));
	printf("%s\n",json_string_value(manacost));
	printf("%s\n",json_string_value(typeline));
	printf("%s - %s\n",setcode,json_string_value(setname));
	printf("%s\n",json_string_value(oracle));
	} else if (json_is_array(cardjson)) {
		for (int i = 0; i < json_array_size(cardjson); i++) {
			json_t *card = json_array_get(cardjson, i);
			json_t *name = json_object_get(card, "name");
			json_t *manacost = json_object_get(card, "mana_cost");
			json_t *typeline = json_object_get(card, "type_line");
			json_t *oracle = json_object_get(cardjson,"oracle_text");
			json_t *setname = json_object_get(cardjson,"set_name");
			json_t *set = json_object_get(cardjson,"set");
			char *lowercode = json_string_value(set);
			char *tmp = lowercode;
			char *setcode = returnupper(tmp);
			printf("%s\n",json_string_value(name));
			printf("%s\n",json_string_value(manacost));
			printf("%s\n",json_string_value(typeline));
			printf("%s - %s\n",setcode,json_string_value(setname));
			printf("%s\n",json_string_value(oracle));
		}
	}
}

int main() {
	int n,i = 0;
	char card[32];
	char url[76] = "https://api.scryfall.com/cards/named?fuzzy=";
	printf("Enter your card: ");
	scanf("%[^\n]%*c",card);
	for(int i = 0;card[i] != '\0';i++) {
		if(card[i] == ' ')
			card[i] = '+';
	}
	n = strlen(card);
	strncat(url,card,n);
	request(url);

	return 0;
}
