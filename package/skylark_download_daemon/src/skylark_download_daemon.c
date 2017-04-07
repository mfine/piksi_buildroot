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

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <libskylark.h>

int main(int argc, char *argv[])
{
  int fd = open("/tmp/skylak_download", O_WRONLY);
  if (fd < 0) {
    exit(EXIT_FAILURE);
  }

  RC rc = skylark_init();
  if (rc != E_OK) {
    close(fd);
    exit(EXIT_FAILURE);
  }

  skylark_config_t config = {
    .url  = "https://broker.skylark2.swiftnav.com",
    .uuid = "1930c93f-6db2-4ed0-b779-029e9e0c53e9",
    .fd   = fd,
  };

  rc = skylark_download(&config);
  if (rc != E_OK) {
    skylark_teardown();
    close(fd);
    exit(EXIT_FAILURE);
  }

  skylark_teardown();
  close(fd);
  exit(EXIT_SUCCESS);
}
