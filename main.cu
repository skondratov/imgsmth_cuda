#include <iostream>
#include "inc/Timer.class.h"
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include "inc/Helper.class.h"
#include <thrust/transform_reduce.h>

using namespace std;
/**
 * Functor for applying to image vector
 */
struct copyelt2xp : public thrust::unary_function<int,int>
{
	int w, h, param;
    bool* m;
    bool* mxp;

    //==============================================================================
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
    copyelt2xp(bool* _m, bool* _mxp, int _w, int _h, int _param):
    m(_m), mxp(_mxp), w(_w), h(_h), param(_param)
	{
    }

    //==============================================================================
    // Algorithm implementation
    //------------------------------------------------------------------------------
    // Author         Date        Action
    // S. Kondratov   05.02.2012  Implementation
    //==============================================================================
    __device__
    int operator()(int i)
	{
        if (!m[i])
        {
	        int w_i = i % w;
	        int h_i = __float2int_rd(i / w);
	        int sum = 0;
	        
	        sum += chc(w_i - 1, w) && chc(h_i - 1, h) ? !m[i - 1 - w] : 1;
	        sum += chc(h_i - 1, h)                    ? !m[i - w]     : 1;
	        sum += chc(w_i + 1, w) && chc(h_i - 1, h) ? !m[i + 1 - w] : 1;
	        sum += chc(w_i - 1, w)                    ? !m[i - 1]     : 1;
	        sum += chc(w_i + 1, w) 		              ? !m[i + 1]     : 1;
	        sum += chc(w_i - 1, w) && chc(h_i + 1, h) ? !m[i - 1 + w] : 1;
	        sum += chc(h_i + 1, h)                    ? !m[i + w]     : 1;
	        sum += chc(w_i + 1, w) && chc(h_i + 1, h) ? !m[i + 1 + w] : 1;
	        
	        if (8 - sum > param)
	        {
	        	mxp[i] = true;
	    		return 1;
	        }
	    }
	    mxp[i] = m[i];
	    return 0;
    }
    //==============================================================================
	// Swap pointer to make new algorithm step
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	__device__ __host__
    void Replace()
    {
    	bool* buff = m;
    	m = mxp;
    	mxp = buff;
    }

	//==============================================================================
	// Alias for logic functions
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
 	__device__
    inline bool chc(const int i, const int n)
    {
		return (n > i) && (i >= 0);
    }
};

//==============================================================================
// Function which implements main part of algorithm, which is ran on CPU
//------------------------------------------------------------------------------
// Author         Date        Action
// S. Kondratov   05.02.2012  Implementation
//==============================================================================
void StartAlgorithm(thrust::host_vector<bool>& m, const int width, const int height, const int param)
{
	// Allocate memory to use pointer arithmetic
	bool* mat = (bool*)malloc(width * height * sizeof(bool));
	bool* matxp = (bool*)malloc(width * height * sizeof(bool));
	
	// Create vectors on allocated memory
	thrust::device_vector<bool> devVec(mat, mat + width * height);
	thrust::device_vector<bool> devVecResult(matxp, matxp + width * height);

	// Fill vector
	devVec.assign(m.begin(), m.end());
	
	thrust::counting_iterator<int>seqb(0);
	thrust::counting_iterator<int>seqe = seqb + width * height;
	
	// Is that save iterate through raw pointer,
	// which allocated using advanced allocator..?
	thrust::device_vector<bool>::iterator m_input = devVec.begin();
	thrust::device_vector<bool>::iterator m_output = devVecResult.begin();

	copyelt2xp functor(thrust::raw_pointer_cast(&m_input[0]),
					   thrust::raw_pointer_cast(&m_output[0]),
					   width,
					   height,
					   param);
	int repeats = 0;
	int res = 1;
	while (res)
	{
		res = thrust::transform_reduce(seqb, seqe, functor, (int)0, thrust::plus<int>());
		functor.Replace();
		if( ++repeats == 150) break;
	}

	if (repeats % 2)
	{
		m.assign(devVec.begin(), devVec.end());
	}
	else
	{
		m.assign(devVecResult.begin(), devVecResult.end());
	}
}

int main( int argc, char* argv[] )
{
	CalcTimer timer1;

	if( argc != 4 )
	{
	  cout << "Usage: imgsmth <input_filename> <output_filename> <param_value>"
	  << endl << endl;
	  return 1;
	}

	// declare and read the bitmap
	BMP Input;
	Input.ReadFromFile(argv[1]);
	thrust::host_vector<bool> result(0);
	Helper::MkeMonochrome(Input);
	Helper::CreateHostVectorFromImage(result, Input);

	StartAlgorithm(result, Input.TellWidth(), Input.TellHeight(), atoi(argv[3]));

	Helper::CreateImageFromHostVector(result, Input);
	Input.WriteToFile(argv[2]);
	return 0;
}




