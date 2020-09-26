// network framework
// - rlyeh, public domain

int download(FILE *out, const char *url);

// int send_game_state(void *ptr, int len, int flags); PROTOCOL_V1|QUANTIZE|COMPRESS|RLE
// int recv_game_state(); compensate, extrapolate, intrapolate(); lerp();

#ifdef NETWORK_C
#pragma once

int download(FILE *out, const char *url) {
    https_t *h = https_get("https://www.google.com/", NULL);
    while (!https_process(h)) sleep_ms(1);
        //printf("%d %s\n\n%.*s\n", h->status_code, h->content_type, (int)h->response_size, (char*)h->response_data);
        bool ok = fwrite(h->response_data, 1, h->response_size, out) == 1;
    https_release(h);
    return ok;
}

#endif // NETWORK_C
