#****************************************************************
#* lite-manager                                                 *
#* NOTE: do not edit this file as it is automatically generated *
#****************************************************************

COMPILER = gcc
CONFIG_DEBUG = y
CONFIG_LOG_LEVEL = 0
CONFIG_MEM_POOL_SIZE = 20
CONFIG_MACRO_CACHE_SIZE = 100

# Variables provided for Makefile
C_SOURCE := lm_macro.c lm_mem.c lm_parser.c lm_string.c lm_array.c lm_log.c lm_gen.c lm_cmd.c main.c heap_tlsf.c

C_PATH := -I.

C_FLAG :=  -O2 -std=c99

