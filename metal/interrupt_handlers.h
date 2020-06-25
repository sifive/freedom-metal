/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef __INTERRUPT_HANDLERS_H
#define __INTERRUPT_HANDLERS_H

#include <metal/interrupt.h>

metal_interrupt_vector_handler_t __metal_vector_table[32];

void __metal_exception_handler();
void metal_riscv_cpu_intc_default_handler();
void metal_riscv_cpu_intc_default_handler();
void metal_riscv_cpu_intc_default_handler();
void metal_riscv_cpu_intc_msip_handler();
void metal_riscv_cpu_intc_default_handler();
void metal_riscv_cpu_intc_default_handler();
void metal_riscv_cpu_intc_default_handler();
void metal_riscv_cpu_intc_mtip_handler();
void metal_riscv_cpu_intc_meip_handler();
void metal_riscv_cpu_intc_default_handler();
void metal_riscv_cpu_intc_default_handler();
void metal_riscv_cpu_intc_default_handler();
void metal_riscv_cpu_intc_default_handler();
void metal_sifive_local_external_interrupts0_source_0_handler();
void metal_sifive_local_external_interrupts0_source_1_handler();
void metal_sifive_local_external_interrupts0_source_2_handler();
void metal_sifive_local_external_interrupts0_source_3_handler();
void metal_sifive_local_external_interrupts0_source_4_handler();
void metal_sifive_local_external_interrupts0_source_5_handler();
void metal_sifive_local_external_interrupts0_source_6_handler();
void metal_sifive_local_external_interrupts0_source_7_handler();
void metal_sifive_local_external_interrupts0_source_8_handler();
void metal_sifive_local_external_interrupts0_source_9_handler();
void metal_sifive_local_external_interrupts0_source_10_handler();
void metal_sifive_local_external_interrupts0_source_11_handler();
void metal_sifive_local_external_interrupts0_source_12_handler();
void metal_sifive_local_external_interrupts0_source_13_handler();
void metal_sifive_local_external_interrupts0_source_14_handler();
void metal_sifive_local_external_interrupts0_source_15_handler();

void metal_sifive_global_external_interrupts0_source_0_handler(int id, void *priv);
void metal_sifive_global_external_interrupts0_source_1_handler(int id, void *priv);
void metal_sifive_global_external_interrupts0_source_2_handler(int id, void *priv);
void metal_sifive_global_external_interrupts0_source_3_handler(int id, void *priv);
void metal_sifive_uart0_source_0_handler(int id, void *priv);
void metal_sifive_spi0_source_0_handler(int id, void *priv);
void metal_sifive_gpio0_source_0_handler(int id, void *priv);
void metal_sifive_gpio0_source_1_handler(int id, void *priv);
void metal_sifive_gpio0_source_2_handler(int id, void *priv);
void metal_sifive_gpio0_source_3_handler(int id, void *priv);
void metal_sifive_gpio0_source_4_handler(int id, void *priv);
void metal_sifive_gpio0_source_5_handler(int id, void *priv);
void metal_sifive_gpio0_source_6_handler(int id, void *priv);
void metal_sifive_gpio0_source_7_handler(int id, void *priv);
void metal_sifive_gpio0_source_8_handler(int id, void *priv);
void metal_sifive_gpio0_source_9_handler(int id, void *priv);
void metal_sifive_gpio0_source_10_handler(int id, void *priv);
void metal_sifive_gpio0_source_11_handler(int id, void *priv);
void metal_sifive_gpio0_source_12_handler(int id, void *priv);
void metal_sifive_gpio0_source_13_handler(int id, void *priv);
void metal_sifive_gpio0_source_14_handler(int id, void *priv);
void metal_sifive_gpio0_source_15_handler(int id, void *priv);
void metal_sifive_pwm0_source_0_handler(int id, void *priv);
void metal_sifive_pwm0_source_1_handler(int id, void *priv);
void metal_sifive_pwm0_source_2_handler(int id, void *priv);
void metal_sifive_pwm0_source_3_handler(int id, void *priv);

#endif 
