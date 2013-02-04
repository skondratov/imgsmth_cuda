#include <thrust/host_vector.h>
#include "easybmp/EasyBMP.h"

/**
 * Helper functions
 */
namespace Helper
{

	//==============================================================================
	// Checks that given pixel is black
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
    inline bool IsBlackPixel(BMP& Image, const int i, const int j)
	{
		if (((int)Image(i,j)->Green + (int)Image(i,j)->Blue + (int)Image(i,j)->Red) == 0)
		return true;
		return false;
	}

    //==============================================================================
	// Short alias for prev.
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	inline bool isbp(BMP& Image, const int i, const int j)
	{
		return IsBlackPixel(Image, i, j);
	}
	//==============================================================================
	// Write color(0-255, in all rgb components)
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	void Make256GrayColorPixel(BMP& Image, const int i, const int j, const int color)
	{
		ebmpBYTE TempBYTE = (ebmpBYTE) color;
		Image(i,j)->Red = TempBYTE;
		Image(i,j)->Green = TempBYTE;
		Image(i,j)->Blue = TempBYTE;
	}

	//==============================================================================
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	void MakePixelBlack(BMP& Image, const int i, const int j)
	{
		Make256GrayColorPixel(Image, i, j, 0);
	}

	//==============================================================================
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	void MakePixelWhite(BMP& Image, const int i, const int j)
	{
		Make256GrayColorPixel(Image, i, j, 255);
	}

	//==============================================================================
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	void CreateHostVectorFromImage(thrust::host_vector<bool>& DevArray, BMP& Image)
	{
		size_t ImagePixelCount = Image.TellHeight() * Image.TellWidth();
		DevArray.reserve(ImagePixelCount);
		for( int j=0 ; j < Image.TellHeight() ; j++)
		{
			for( int i=0 ; i < Image.TellWidth() ; i++)
			{
				DevArray.push_back(IsBlackPixel(Image, i,j));
			}
		}
	}

	//==============================================================================
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	void CreateImageFromHostVector(thrust::host_vector<bool>& DevArray, BMP& Image)
	{
		typedef thrust::host_vector<bool>::iterator Iterator;
		const size_t w = Image.TellWidth();
		const size_t h = Image.TellHeight();
		int i, j = 0;
		for (Iterator x = DevArray.begin(); x != DevArray.end(); ++x)
		{
			if (++i >= w)
			{
				i = 0;
				if (++j >= h) break;
			}
			Make256GrayColorPixel(Image, i, j, ((*x) ? 0 : 255));
		}
	}

	//==============================================================================
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	void MkeMonochrome(BMP& Image)
		{
			Image.SetBitDepth(1);

			// convert each pixel to 1 bit
			for( int j=0 ; j < Image.TellHeight() ; j++)
			{
				for( int i=0 ; i < Image.TellWidth() ; i++)
				{
					if ((int)Image(i,j)->Green != 0 || (int)Image(i,j)->Blue != 0 || (int)Image(i,j)->Red != 0)
					{
						MakePixelWhite(Image, i, j);
					}
				}
			}
		}
}
