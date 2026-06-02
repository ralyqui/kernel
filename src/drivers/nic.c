/*
 * Intel® 82540EM polling network driver
 */

#include "nic.h"
#include "core/cpu.h"
#include "core/pmm.h"
#include "io/pci.h"
#include <core/fmt.h>
#include <stdint.h>

extern volatile uint64_t lm_hhdm_offset;
extern volatile uint64_t lm_kernel_phys;
extern volatile uint64_t lm_kernel_virt;

#define I82540EM_VENDOR_ID 0x8086
#define I82540EM_DEVICE_ID 0x100e
#define I82540EM_BAR0_OFFSET 0x10

#define I82540EM_CTRL_REG 0x00000
#define TCTL_REG 0x00400
#define TDBAL_REG 0x03800
#define TDBAH_REG 0x03804
#define TDLEN_REG 0x03808
#define TDH_REG 0x03810
#define TDT_REG 0x03818

#define DESCRIPTOR_SIZE 0x10
#define NUM_TX_DESCRIPTORS 0x10
#define NUM_RX_DESCRIPTORS 0x10
#define TX_BUFFER_SIZE 4096

#define I82540EM_RESET 0x02000000

typedef struct {
    uint64_t buffer_phys_;
} tx_descriptor;

static nic_info_t _device_info;
// static uint8_t _mac_addr[6];

static void dump_packet() {}

static inline void write_register(uint32_t reg, uint32_t val) {
    *((volatile uint32_t *)(_device_info.mmio_base_ + reg)) = val;
}

static inline uint32_t read_register(uint64_t offset) {
    return *((volatile uint32_t *)(_device_info.mmio_base_ + offset));
}

static void init_transmit_ring() {
    uint32_t tring_size = NUM_TX_DESCRIPTORS * DESCRIPTOR_SIZE;
    uint64_t tring_phys = pmm_alloc(tring_size);
    void *tring_virt = phys_to_virt(tring_phys);

    for (uint32_t i = 0; i < NUM_TX_DESCRIPTORS; i++) {
        tx_descriptor *tx_desc = (tx_descriptor *)tring_virt + i;
        tx_desc->buffer_phys_ = pmm_alloc(TX_BUFFER_SIZE);
    }

    print_f("tring_phys is %l", tring_phys);

    write_register(TDBAL_REG, tring_phys & 0xffffffff);
    write_register(TDBAH_REG, tring_phys >> 32);
    write_register(TDLEN_REG, tring_size);
    write_register(TDT_REG, 0x0);
    write_register(TDT_REG, 0x0);

    write_register(TCTL_REG, 0xa); // enable + pad
}

void nic_init(uint32_t bus, uint32_t slot) {
    uint32_t base = pci_read32(bus, slot, 0, I82540EM_BAR0_OFFSET) & ~0xF;

    _device_info.mmio_base_ = lm_hhdm_offset + base;
    _device_info.bus = bus;
    _device_info.slot = slot;

    uint32_t device_status = read_register(0x08);
    uint32_t nic_ctrl_val = read_register(I82540EM_CTRL_REG);
    nic_ctrl_val |= I82540EM_RESET;
    write_register(I82540EM_CTRL_REG, nic_ctrl_val);

    while (!(read_register(I82540EM_CTRL_REG) & I82540EM_RESET))
        cpu_nop();

    init_transmit_ring();

    print_f("device status is: %l\n", device_status);
    print_f("mmio base is %l\n", _device_info.mmio_base_);
    print_f("tdt is %l", read_register(TDT_REG));
    print_f("tdt is %l", read_register(TDH_REG));

    dump_packet();
}
