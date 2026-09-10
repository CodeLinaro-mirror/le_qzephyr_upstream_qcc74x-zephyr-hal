/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "utils_getopt.h"

void utils_al_parse_number_adv(const char *str, char sep, uint8_t *buf, int buflen,
			       int base, int *count)
{
	utils_parse_number_adv(str, sep, buf, buflen, base, count);
}

void utils_al_parse_number(const char *str, char sep, uint8_t *buf, int buflen, int base)
{
	utils_parse_number(str, sep, buf, buflen, base);
}

int utils_al_getopt_init(getopt_env_t *env, int opterr)
{
	return utils_getopt_init(env, opterr);
}

int utils_al_getopt(getopt_env_t *env, int argc, char *const argv[], const char *optstring)
{
	return utils_getopt(env, argc, argv, optstring);
}
