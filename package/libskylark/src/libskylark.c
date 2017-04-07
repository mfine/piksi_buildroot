/*
 * Copyright (C) 2017 Swift Navigation Inc.
 * Contact: Swift Navigation <dev@swiftnav.com>
 *
 * This source is subject to the license found in the file 'LICENSE' which must
 * be be distributed together with this source. All other rights reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <unistd.h>
#include <curl/curl.h>

#include "libskylark.h"

#define AGENT_TYPE   "libskylark-agent/1.0"
#define ACCEPT_TYPE  "application/vnd.swiftnav.broker.v1+sbp2"
#define CONTENT_TYPE "application/vnd.swiftnav.broker.v1+sbp2"

static RC perform(CURL *curl)
{
  for (;;) {
    curl_easy_perform(curl);
  }

  return E_OK;
}

static size_t download(char *buf, size_t size, size_t n, void *data)
{
  int fd = *(int *)data;

  ssize_t ret = write(fd, buf, size * n);

  return ret;
}

static size_t upload(char *buf, size_t size, size_t n, void *data)
{
  int fd = *(int *)data;

  ssize_t ret = read(fd, buf, size * n);
  if (ret < 0) {
    return CURL_READFUNC_ABORT;
  }

  return ret;
}

RC skylark_init(void)
{
  CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
  if (res != CURLE_OK) {
    return -E_INITIALIZATION_ERROR;
  }

  return E_OK;
}

void skylark_teardown(void)
{
  curl_global_cleanup();
}

RC skylark_download(const skylark_config_t *config)
{
  CURL *curl = curl_easy_init();
  if (curl == NULL) {
    return -E_INITIALIZATION_ERROR;
  }

  char uuid_buf[256];
  snprintf(uuid_buf, sizeof(uuid_buf), "Device-Uid: %s", config->uuid);

  struct curl_slist *chunk = NULL;
  chunk = curl_slist_append(chunk, "Transfer-Encoding: chunked");
  chunk = curl_slist_append(chunk, "Accept: " ACCEPT_TYPE);
  chunk = curl_slist_append(chunk, uuid_buf);

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER,    chunk);
  curl_easy_setopt(curl, CURLOPT_USERAGENT,     AGENT_TYPE);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, download);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA,     &config->fd);
  curl_easy_setopt(curl, CURLOPT_URL,           config->url);

  RC rc = perform(curl);
  curl_easy_cleanup(curl);

  return rc;
}

RC skylark_upload(const skylark_config_t *config)
{
  CURL *curl = curl_easy_init();
  if (curl == NULL) {
    return -E_INITIALIZATION_ERROR;
  }

  char uuid_buf[256];
  snprintf(uuid_buf, sizeof(uuid_buf), "Device-Uid: %s", config->uuid);

  struct curl_slist *chunk = NULL;
  chunk = curl_slist_append(chunk, "Transfer-Encoding: chunked");
  chunk = curl_slist_append(chunk, "Content-Type: " CONTENT_TYPE);
  chunk = curl_slist_append(chunk, uuid_buf);

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER,   chunk);
  curl_easy_setopt(curl, CURLOPT_USERAGENT,    AGENT_TYPE);
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, upload);
  curl_easy_setopt(curl, CURLOPT_READDATA,     &config->fd);
  curl_easy_setopt(curl, CURLOPT_URL,          config->url);
  curl_easy_setopt(curl, CURLOPT_PUT,          1L);

  RC rc = perform(curl);
  curl_easy_cleanup(curl);

  return rc;
}
