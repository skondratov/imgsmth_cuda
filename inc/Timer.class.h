#include <sys/time.h>
#include <map>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
/**
 * Helper class for algorithm debugging
 */
class CalcTimer
{
	typedef vector< pair<string, double> > map;
	typedef vector< pair<string, double> >::iterator map_it;
	double FStart;
	map FSeqMes;
public:

	//==============================================================================
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	CalcTimer()
	{
	   FStart = GetCurrentTime();
	}

	//==============================================================================
	// Show diffs on object destroying
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	~CalcTimer()
	{
	   CalcMainDiff();
	   std::cout << "Point diff(in seconds): " << endl;
	   PrintPointsDiff();
	}

	//==============================================================================
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	void CalcMainDiff()
	{
	   double diff = GetCurrentTime();
	   diff -= FStart;
	   std::cout << "Main timediff(in seconds): " << diff << endl;
	}

	//==============================================================================
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	void PrintPointsDiff()
	{
	   for(map_it i = FSeqMes.begin(); i != FSeqMes.end();++i)
	   {
		   if (i == FSeqMes.begin()) cout << i->first << ": " << (i->second - FStart) << endl;
		   else
		   {
			   cout<< i->first << ": " << (i->second - ((--i)++)->second) << endl;
		   }
	   }
	}

	//==============================================================================
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	inline static double GetCurrentTime()
	{
		struct timezone tz;
		struct timeval t;
		gettimeofday(&t, &tz) ;
		double d = double(t.tv_sec) + double( t.tv_usec ) / 1000000.0;
		return  d ;
	}

	//==============================================================================
	//------------------------------------------------------------------------------
	// Author         Date        Action
	// S. Kondratov   05.02.2012  Implementation
	//==============================================================================
	void AddPoint(string Key)
	{
	   FSeqMes.push_back(pair<string, double>(Key, GetCurrentTime()));
	}
};
