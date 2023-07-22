#include "pcg_variants.h"
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
         "-r, --human-readable \tPrint password in human readable format.\n"
         "--seed0=<number>,\n"
         "--seed1=<number> \tUse this number as seed of generator.\n"
         "--without-header \tDont print header info.\n"
         "-h, --help\tShow help\n",
         PASSWDGEN_VERSION_MAJOR, PASSWDGEN_VERSION_MINOR,
         PASSWDGEN_VERSION_PATCH);
}

#define FL_HUMAN_READABLE 0b0001

void passwdgen(int len, const char *alphabet, int flags) {
  int strlen = strnlen(alphabet, 255);
  int i;
  int index = 0;
  for (i = 0; i < len; i++) {
    index = pcg32_boundedrand(strlen);
    if (flags & FL_HUMAN_READABLE && i > 0 && i % 4 == 0) {
      putc('-', stdout);
    }
    putc(alphabet[index], stdout);
  }
  putc('\n', stdout);
}

int init_seed() {
  int rc;
  int fd;
  int64_t seed[2];
  fd = open("/dev/random", O_RDONLY);
  if (fd < 0) {
    fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
      perror("Open /dev/random error: ");
      return -1;
    }
  }
  rc = read(fd, (void *)seed, sizeof(seed));
  close(fd);
  pcg32_srandom(seed[0], seed[1]);
  return 0;
}

static struct option long_options[] = {{"len", 1, 0, 'l'},
                                       {"count", 1, 0, 'c'},
                                       {"alphabet", 1, 0, 'a'},
                                       {"human-readable", 0, 0, 'r'},
                                       {"help", 0, 0, 'h'},
                                       {"seed0", 1, 0, '0'},
                                       {"seed1", 1, 0, '1'},
                                       {"without-header", 0, 0, 'w'},
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
  int flags;
  int64_t seed0;
  int64_t seed1;
};

#define OPT_NEED_HELP 1
#define OPT_ERR 2

int init_opts(int argc, char **argv, struct opts *opts) {
  int rc;
  int opt_index = 0;
  memset(opts, 0, sizeof(struct opts));
  opts->len = DEFAULT_LEN;
  opts->count = DEFAULT_COUNT;
  opts->gen_flags = 0;
  opts->flags = 0;
  strcpy(opts->alphabet, DEFAULT_ALPHABET);
  while (1) {
    rc = getopt_long(argc, argv, "l:c:a:h0:1:w", long_options, &opt_index);
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
    case 'r':
      opts->gen_flags = FL_HUMAN_READABLE;
      break;
    case '0':
      opts->seed0 = atoll(optarg);
      opts->flags = FL_USER_SEED;
      break;
    case '1':
      opts->seed1 = atoll(optarg);
      opts->flags = FL_USER_SEED;
      break;
    case 'w':
      opts->flags |= FL_WITHOUT_HEADER;
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
  } else if (rc == OPT_ERR) {
    show_usage();
    return EXIT_FAILURE;
  }
  if (opts.flags & FL_USER_SEED) {
    pcg32_srandom(opts.seed0, opts.seed1);
    if(!(opts.flags & FL_WITHOUT_HEADER)) {
      printf("seed0=%llu, seed1=%llu, ", opts.seed0, opts.seed1);
    }
  } else {
    init_seed();
  }
  if(!(opts.flags & FL_WITHOUT_HEADER)) {
    printf("count=%d, len=%d, alphabet=%s\n", opts.count, opts.len,
          opts.alphabet);
  }
  int i;
  for (i = 0; i < opts.count; i++) {
    passwdgen(opts.len, opts.alphabet, opts.gen_flags);
  }
  return EXIT_SUCCESS;
}