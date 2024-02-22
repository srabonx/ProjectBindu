#include "Include/BitmapLoader.h"
#include "Include/constants.h"

#include <d2d1_1.h>
#include <wincodec.h>

#include "Include/Bindu.h"

namespace BINDU
{
	BitmapLoader::BitmapLoader()
	{
	}

	BitmapLoader::~BitmapLoader()
	{
	}

	bool BitmapLoader::LoadFromFile(const wchar_t* filename, ID2D1Bitmap** ppBitmap)
	{
		if (!(*ppBitmap))
		{
			ComPtr<IWICImagingFactory> pWicImagingFactory;							// WIC imaging factory
			ComPtr<IWICBitmapDecoder> pDecoder;//= NULL;							// Bitmap Decoder
			ComPtr<IWICBitmapFrameDecode> pFrame;//= NULL;							// Bitmap Frame 
			ComPtr<IWICFormatConverter> pConverter;

			HRESULT hr = S_OK;

			// Create WIC factory
			CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory,
				(LPVOID*)pWicImagingFactory.GetAddressOf());


			DX::ThrowIfFailed(

				pWicImagingFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder)

			);

			DX::ThrowIfFailed(

				pDecoder->GetFrame(0, &pFrame)									// Create initial frame	

			);

			DX::ThrowIfFailed(

				pWicImagingFactory->CreateFormatConverter(&pConverter)			// Convert the image format to 32bppPBGRA

			);

			DX::ThrowIfFailed(

				pConverter->Initialize(pFrame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL,
					0.0f, WICBitmapPaletteTypeCustom)

			);

			ComPtr<ID2D1Bitmap> tempBitmap;


			DX::ThrowIfFailed(

				g_engine->getGraphics()->getRenderTarget()->CreateBitmapFromWicBitmap(pConverter.Get(), tempBitmap.ReleaseAndGetAddressOf())

			);


			if(tempBitmap)
			{
				D2D1_SIZE_F tempSize = tempBitmap->GetSize();

				DX::ThrowIfFailed(
					g_engine->getGraphics()->getRenderTarget()->CreateBitmap(D2D1::SizeU(static_cast<UINT32>(tempSize.width),static_cast<UINT32>( tempSize.height)), nullptr, 0, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), ppBitmap)
				);

				(*ppBitmap)->CopyFromBitmap(nullptr,tempBitmap.Get(),nullptr);

			}

		}
			return true;
	}
}// Namespace