typedef void* (*MALLOC)(size_t);
MALLOC old_malloc;

typedef void (*FREE)(intptr_t);
FREE old_free;

void* new_malloc(size_t s)
{
    void *addr = old_malloc(s+16);

    *(size_t*)addr = s;
    *(uint32_t*)((char*)addr+8) = 0x11223344;

    return (char*)addr+16;
}

void new_free(intptr_t p)
{
}

void patch_malloc()
{
    // 43.05 64bit
    intptr_t faddr = 0x140c49750 + Core::getInstance().vinfo->getRebaseDelta();
    intptr_t maddr = 0x140c49758 + Core::getInstance().vinfo->getRebaseDelta();

    // 44.02 64bit
    intptr_t faddr = 0x140cd9750 + Core::getInstance().vinfo->getRebaseDelta();
    intptr_t maddr = 0x140cd9758 + Core::getInstance().vinfo->getRebaseDelta();

    old_malloc = *(MALLOC*)maddr;
    old_free = *(FREE*)faddr;

    (new MemoryPatcher(Core::getInstance().p))->makeWritable((void*)maddr, sizeof(intptr_t));
    intptr_t nm = (intptr_t)&new_malloc;
    memcpy((void*)maddr, &nm, sizeof(intptr_t));

    (new MemoryPatcher(Core::getInstance().p))->makeWritable((void*)faddr, sizeof(intptr_t));
    intptr_t nf = (intptr_t)&new_free;
    memcpy((void*)faddr, &nf, sizeof(intptr_t));
}
