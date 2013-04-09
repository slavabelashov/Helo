#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main(EFI_HANDLE handler, EFI_SYSTEM_TABLE *tab_system)
{
	InitializeLib(handler, tab_system);
	EFI_MEMORY_DESCRIPTOR *MemMap;
	EFI_STATUS status;
	UINTN handler_of_mem, size_of_description, key_of_map, size_of_MeMemMap;
	UINT32 ver_of_desription;
	EFI_MEMORY_TYPE type_of_mem = EfiLoaderData;
	loader = tab_system->BootService;

	status = uefi_call_wrapper(loader->GetMemoryMap, 5, &size_of_MeMemMap, MemMap, &key_of_map, &size_of_description, &ver_of_desription);
	if(status != EFI_BUFFER_TOO_SMALL)
	{
		Print (L"Don't memory map\n");
	}
	else
	{
		status = uefi_call_wrapper(loader->AllocatePool, 3, type_of_mem, size_of_MeMemMap, &MemMap);  
		if(status != EFI_SUCCESS)
		{
			Print(L"Can't allocate memory\n");
			return EFI_SUCCESS;
		}
		status = uefi_call_wrapper(loader->GetMemoryMap, 5, &size_of_MeMemMap, MemMap, &key_of_map, &size_of_description, &ver_of_desription);  
		if(status != EFI_SUCCESS)
		{
			Print(L"Don't get memory map\n");
			return EFI_SUCCESS;
		}
		N = size_of_MeMemMap/sizeof(EFI_MEMORY_DESCRIPTOR);
		for (int i=0; i<N; i++)
		{
			switch(MemMap[i].Type)
			{
				case EfiConventionalMemory : handler_of_mem = (handler_of_mem + MemMap[i].NumberOfPages*4096);
				case EfiBootServicesData   : handler_of_mem = (handler_of_mem + MemMap[i].NumberOfPages*4096);
				case EfiBootServicesCode   : handler_of_mem = (handler_of_mem + MemMap[i].NumberOfPages*4096);
			}
		}
		Print(L"%d bytes of free memory\n", handler_of_mem);
		status = uefi_call_wrapper(loader->FreePool, 1, &MemMap);		
	}
	return EFI_SUCCESS;
}