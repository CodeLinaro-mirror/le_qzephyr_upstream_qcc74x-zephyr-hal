/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* configure the CPU type */
OUTPUT_ARCH( "riscv" )

/* configure the entry point */
ENTRY(lp_fw_start)

StackSize = 0x400; /*  1KB */

MEMORY
{
    lpfw_memory (rwx)   : ORIGIN = 0x63010000,  LENGTH = 28K
}

PROVIDE(__itcm_load_addr = 0);
PROVIDE(__dtcm_load_addr = 0);
PROVIDE(__ram_load_addr = 0);
PROVIDE(__nocache_ram_load_addr = 0);
PROVIDE(__psram_load_addr = 0);
PROVIDE(__text_code_start__ = 0);
PROVIDE(__text_code_end__ = 0);
PROVIDE(__tcm_code_start__ = 0);
PROVIDE(__tcm_code_end__ = 0);
PROVIDE(__tcm_data_start__ = 0);
PROVIDE(__tcm_data_end__ = 0);
PROVIDE(__ram_data_start__ = 0);
PROVIDE(__ram_data_end__ = 0);
PROVIDE(__wifi_bss_start = 0);
PROVIDE(__wifi_bss_end = 0);
PROVIDE(__noinit_data_start__ = 0);
PROVIDE(__noinit_data_end__ = 0);
PROVIDE(__psram_data_start__ = 0);
PROVIDE(__psram_data_end__ = 0);
PROVIDE(__nocache_ram_data_start__ = 0);
PROVIDE(__nocache_ram_data_end__ = 0);
PROVIDE(__HeapBase = 0);
PROVIDE(__HeapLimit = 0);
PROVIDE(__LD_CONFIG_EM_SEL = 0);

/* PROVIDE( __RAM_WIFI_START_ADD = __RAM_WIFI_START_ADD ); */

SECTIONS
{
    __lpfw_memory_start = ORIGIN(lpfw_memory);

    .text :
    {
        . = ALIGN(4);
        __text_code_start__ = .;

        KEEP(*(.lp.text.entry))

        *(.text)
        *(.text.*)
        *(.tcm_code*)
        *(.hbn_code*)
        *(.romwifi_code*)

        /* section information for shell */
        . = ALIGN(4);
        __fsymtab_start = .;
        KEEP(*(FSymTab))
        __fsymtab_end = .;

        . = ALIGN(4);
        __vsymtab_start = .;
        KEEP(*(VSymTab))
        __vsymtab_end = .;

        *(.rodata*)
        *(.srodata)
        *(.srodata.*)
        *(.tcm_const*)

        /* global object for c++ */
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP(*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        KEEP(*(.init_array))
        __init_array_end = .;

        . = ALIGN(4);
        *(.gcc_except_table.*)

        . = ALIGN(4);
        __text_code_end__ = .;
    } > lpfw_memory

    .data :
    {
        . = ALIGN(32);
        PROVIDE(__ram_data_start__ = . );

        PROVIDE( __global_pointer$ = . + 0x800 );

        *(.data .data.*)
        *(.sdata .sdata.*)
        *(.sdata2 .sdata2.*)

        *(.gnu.linkonce.d.*)
        *(.gnu.linkonce.s.*)

        . = ALIGN(32);
        PROVIDE(__ram_data_end__ = . );

    } > lpfw_memory

    __lpfw_code_end = .;

    .data_copy (NOLOAD) :
    {
        . = ALIGN(32);
        PROVIDE(__ram_data_copy_start__ = . );

        . = . + SIZEOF(.data);

        . = ALIGN(32);
        PROVIDE(__ram_data_copy_end__ = . );
    } > lpfw_memory

    .bss (NOLOAD) :
    {
        . = ALIGN(32);
        __bss_start__ = .;

        *(.bss*)
        *(.sbss*)
        *(COMMON)

        . = ALIGN(32);
        __bss_end__ = .;
    } > lpfw_memory

    .wifi_rx_buffer (NOLOAD) :
    {
        . = ALIGN(32);
        __wifi_rx_buffer_start = .;
        KEEP(*(.wifi_rx_buffer*))
        . = ALIGN(32);
        __wifi_rx_buffer_end = .;
    } > lpfw_memory

    .stack (NOLOAD) :
    {
        __StackLimit = .;
        . = ALIGN(16);
        . = . + StackSize;
        __StackTop = .;

    } > lpfw_memory

    PROVIDE( _sp_main = __StackTop);
    PROVIDE( _sp_base = __StackLimit);

    __lpfw_memory_end = .;
    ASSERT(__lpfw_memory_end <= ORIGIN(lpfw_memory) + LENGTH(lpfw_memory),
           "LPFW exceeds the approved 28 KiB reservation")
}
