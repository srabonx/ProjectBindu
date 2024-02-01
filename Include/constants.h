#pragma once
#include <d3d11.h>
#include <exception>
#include <stdio.h>


constexpr float PI = 3.14159265f;

// Safely release pointers
#define SafeRelease(ptr){if(ptr!=NULL)(ptr)->Release();ptr=NULL;}

// template<class Interface>
// inline void SafeRelease(Interface** ppInterfaceToRelease)
// {
// 	if ((*ppInterfaceToRelease) != NULL)
// 	{
// 		(*ppInterfaceToRelease)->Release();
// 		(*ppInterfaceToRelease) = NULL;
// 	}
// }

namespace DX
{
	// Helper class for COM exceptions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) : result(hr) {}

		virtual const char* what() const override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X",
				static_cast<unsigned int>(result));
			return s_str;
		}

	private:
		HRESULT result;
	};

	// Helper utility converts D3D API failures into exceptions.
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}
}

template<UINT TNameLength>
inline void SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_z_ const char(&name)[TNameLength])
{
#if defined(_DEBUG)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#else
	UNREFERENCED_PARAMETER(resource);
	UNREFERENCED_PARAMETER(name);
#endif
}



