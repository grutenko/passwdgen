#include "version.h"
#include <fcntl.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void show_usage() {
  printf("passwdgen v%d.%d.%d (c) Oustoteam\n"
         "Usage:\n"
         "-l, --len=<number>\tLength of password\n"
         "-c, --count=<number>\tCount of variants\n"
         "-a, --alphabet=<chars>\tAlphabet for generated password\n"
         "-h, --help\tShow help\n"
         "-v, --version\tShow version\n",
         PASSWDGEN_VERSION_MAJOR, PASSWDGEN_VERSION_MINOR,
         PASSWDGEN_VERSION_PATCH);
}

#define FL_HUMAN_READABLE 0b0001

static int read_entropy(void *data, size_t size) {
  int rc;
#ifdef __MINGW32__
  HCRYPTPROV hCryptProv = NULL;
  LPCTSTR pszContainerName = TEXT("Entropy Container");
  rc = CryptAcquireContext(&hCryptProv, pszContainerName, NULL, PROV_RSA_FULL,
                           0);
  if (!rc && GetLastError() != NTE_BAD_KEYSET)
    return -1;
  rc = CryptAcquireContext(&hCryptProv, pszContainerName, NULL, PROV_RSA_FULL,
                           CRYPT_NEWKEYSET);
  if (!rc)
    return -1;
  rc = CryptGenRandom(hCryptProv, size, data);
  if (!rc) {
    CryptReleaseContext(hCryptProv, 0);
    return -1;
  }
#else
  int fd = open("/dev/random", O_RDONLY);
  if (fd < 0) {
    fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0)
      return -1;
  }

  rc = read(fd, data, size);
  if (rc < 0) {
    close(fd);
    return -1;
  }
  close(fd);
#endif
  return 0;
}

static int brandint(uint32_t b, uint32_t *v) {
  int rc;
  uint32_t buf;
  rc = read_entropy(&buf, sizeof(uint32_t));
  if (rc < 0) {
    return -1;
  }
  const uint32_t t = -b % b;
  for (;;) {
    rc = read_entropy(&buf, sizeof(uint32_t));
    if (rc < 0)
      return -1;
    if (buf >= t) {
      *v = buf % b;
      return 0;
    }
  }
}

static int passwdgen(int len, const char *alphabet) {
  int strlen = strnlen(alphabet, 255);
  int i;
  uint32_t index;
  int rc;
  for (i = 0; i < len; i++) {
    rc = brandint(strlen, &index);
    if (rc < 0)
      return -1;
    putc(alphabet[index], stdout);
  }
  putc('\n', stdout);
  return 0;
}

static struct option long_options[] = {{"len", 1, 0, 'l'},
                                       {"count", 1, 0, 'c'},
                                       {"alphabet", 1, 0, 'a'},
                                       {"human-readable", 0, 0, 'r'},
                                       {"help", 0, 0, 'h'},
                                       {"version", 0, 0, 'v'},
                                       {0, 0, 0, 0}};

#define DEFAULT_LEN 8
#define DEFAULT_COUNT 5
#define DEFAULT_ALPHABET                                                       \
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

#define FL_USER_SEED 0b0001
#define FL_WITHOUT_HEADER 0b0010

struct opts {
  int len;
  int count;
  char alphabet[256];
  int gen_flags;
};

#define OPT_NEED_HELP 1
#define OPT_NEED_VERSION 3
#define OPT_ERR 2

int init_opts(int argc, char **argv, struct opts *opts) {
  int rc;
  int opt_index = 0;
  memset(opts, 0, sizeof(struct opts));
  opts->len = DEFAULT_LEN;
  opts->count = DEFAULT_COUNT;
  opts->gen_flags = 0;
  strcpy(opts->alphabet, DEFAULT_ALPHABET);
  while (1) {
    rc = getopt_long(argc, argv, "l:c:a:hv", long_options, &opt_index);
    if (rc < 0)
      return 0;
    switch (rc) {
    case 'l':
      opts->len = atoi(optarg);
      break;
    case 'c':
      opts->count = atoi(optarg);
      break;
    case 'a':
      strncpy(opts->alphabet, optarg, 255);
      break;
    case 'h':
      return OPT_NEED_HELP;
    case 'v':
      return OPT_NEED_VERSION;
    case 'r':
      opts->gen_flags = FL_HUMAN_READABLE;
      break;
    case '?':
      return OPT_ERR;
    }
  }
}

int main(int argc, char *argv[]) {
  int rc;
  struct opts opts;
  rc = init_opts(argc, argv, &opts);
  if (rc == OPT_NEED_HELP) {
    show_usage();
    return EXIT_SUCCESS;
  } else if (rc == OPT_NEED_VERSION) {
    printf("passwdgen v%d.%d.%d\n", PASSWDGEN_VERSION_MAJOR,
           PASSWDGEN_VERSION_MINOR, PASSWDGEN_VERSION_PATCH);
    return EXIT_SUCCESS;
  } else if (rc == OPT_ERR) {
    show_usage();
    return EXIT_FAILURE;
  }
  int i;
  for (i = 0; i < opts.count; i++) {
    if(passwdgen(opts.len, opts.alphabet) < 0) {
      perror("IO Error:");
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}