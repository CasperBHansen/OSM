/*
 * TLB handling
 *
 * Copyright (C) 2003 Juha Aatrokoski, Timo Lilja,
 *   Leena Salmela, Teemu Takanen, Aleksi Virtanen.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: tlb.c,v 1.6 2004/04/16 10:54:29 ttakanen Exp $
 *
 */

#include "kernel/assert.h"
#include "kernel/panic.h"
#include "kernel/thread.h"
#include "vm/tlb.h"
#include "vm/pagetable.h"

int tlb_load_store_exception(void) {
    // load exception state into state structure
    tlb_exception_state_t state;
    _tlb_get_exception_state(&state);
    uint32_t badvpn2 = state.badvpn2;

    // access page entries array in pagetable struct in thread struct
    thread_table_t *t_table = thread_get_current_thread_entry();
    pagetable_t *p_table = t_table->pagetable;
    tlb_entry_t *p_entries = p_table->entries; // PAGETABLE_ENTRIES entries in array

    int i;
    for (i = 0; i < PAGETABLE_ENTRIES; i++) {
        if (p_entries[i].VPN2 == badvpn2) {
            _tlb_write_random(&p_entries[i]);
            kprintf("page table entry %d found\n", i);
            return 0;
        }
    }

    kprintf("badvpn %d not found!\n", badvpn2);

    return 1;
}

void tlb_modified_exception(void)
{
    KERNEL_PANIC("Unhandled TLB modified exception");
}

void tlb_load_exception(void)
{
    if (tlb_load_store_exception())
        KERNEL_PANIC("TLB load exception: page entry not found or invalid.");
}

void tlb_store_exception(void)
{
    if (tlb_load_store_exception())
        KERNEL_PANIC("TLB store exception: page entry not found or invalid.");
}

/**
 * Fill TLB with given pagetable. This function is used to set memory
 * mappings in CP0's TLB before we have a proper TLB handling system.
 * This approach limits the maximum mapping size to 128kB.
 *
 * @param pagetable Mappings to write to TLB.
 *
 */

void tlb_fill(pagetable_t *pagetable)
{
    if(pagetable == NULL)
	return;

    /* Check that the pagetable can fit into TLB. This is needed until
     we have proper VM system, because the whole pagetable must fit
     into TLB. */
    KERNEL_ASSERT(pagetable->valid_count <= (_tlb_get_maxindex()+1));

    _tlb_write(pagetable->entries, 0, pagetable->valid_count);

    /* Set ASID field in Co-Processor 0 to match thread ID so that
       only entries with the ASID of the current thread will match in
       the TLB hardware. */
    _tlb_set_asid(pagetable->ASID);
}
