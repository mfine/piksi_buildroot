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

#ifndef SWIFTNAV_LIBSKYLARK_H
#define SWIFTNAV_LIBSKYLARK_H

typedef enum {
  E_OK = 0,
  E_INITIALIZATION_ERROR,
  E_RETRIES_ERROR,
  E_MAX_ERROR,
} RC;

typedef struct {
  const char *url;
  const char *device_uuid;
  int fd;
} skylark_config_t;

RC skylark_init(void);

RC skylark_teardown(void);

RC skylark_download(const skylark_config_t *config);

RC skylark_upload(const skylark_config_t *config);

#endif /* SWIFTNAV_LIBSKYLARK_H */
