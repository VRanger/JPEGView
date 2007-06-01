#pragma once

namespace Helpers {

	// Capabilities of CPU
	enum CPUType {
		CPU_Unknown,
		CPU_Generic,
		CPU_MMX,
		CPU_SSE
	};

	// Sorting order of image files
	enum ESorting {
		FS_LastModTime,
		FS_CreationTime,
		FS_FileName
	};

	// Navigation mode over directories
	enum ENavigationMode {
		NM_LoopDirectory,
		NM_LoopSubDirectories,
		NM_LoopSameDirectoryLevel
	};

	// Auto zoom modes
	enum EAutoZoomMode {
		ZM_FitToScreenNoZoom,
		ZM_FillScreenNoZoom,
		ZM_FitToScreen,
		ZM_FillScreen
	};

	// Scaling factor from screen DPI, 96 dpi -> 1.0, 120 dpi -> 1.2
	extern float ScreenScaling;

	// Maximum and minimum allowed zoom factors for images
	const double ZoomMax = 16.0;
	const double ZoomMin = 0.1;

	// Round to integer
	inline int RoundToInt(double d) {
		return (d < 0) ? (int)(d - 0.5) : (int)(d + 0.5);
	}

	// Gets the image size to be used when fitting the image to screen, either using 'fit to screen'
	// or 'fill with crop' method. If 'fill with crop' is used, the bLimitAR can be set to avoid
	// filling when to less pixels remain visible
	CSize GetImageRect(int nWidth, int nHeight, int nScreenWidth, int nScreenHeight, 
		bool bAllowZoomIn, bool bFillCrop, bool bLimitAR);

	// Gets the image size to be used when fitting the image to screen according to the auto zoom mode given
	CSize GetImageRect(int nWidth, int nHeight, int nScreenWidth, int nScreenHeight, EAutoZoomMode eAutoZoomMode);

	// Tests if the CPU supports SSE or MMX(2)
	CPUType ProbeCPU(void);

	// Gets the path where JPEGView stores its application data, including a trailing backslash
	LPCTSTR JPEGViewAppDataPath();

	// Checks if the given string matches the pattern definition given.
	// Pattern: * - will be matched by any number of characters
	//          ; - separates patterns
	//          other chars - must match (ignoring case)
	bool PatternMatch(LPCTSTR sString, LPCTSTR sPattern);

	// Pad the given value to the next multiple of padvalue
	inline int DoPadding(int value, int padvalue) {
		return (value + padvalue - 1) & ~(padvalue - 1);
	}

	// Difference of next padded value and value
	inline int PaddedBytes(int value, int padvalue) {
		return (padvalue - ((value & (padvalue - 1)))) & (padvalue - 1);
	}

	// calculate CRT table
	void CalcCRCTable(unsigned int crc_table[256]);

	// Calculates a hash value over the given JPEG stream having the given length (in bytes).
	// All blocks and tables in the JPEG stream are not included into the hash to allow
	// e.g. commenting the JPEG or changing some EXIF information without changing the hash.
	__int64 CalculateJPEGFileHash(void* pJPEGStream, int nStreamLength);

	// Conversion class that replaces the | by null character in a string.
	// Caution: Uses a static buffer and can therefore only one string can be replaced concurrently
	const int MAX_SIZE_REPLACE_PIPE = 256;

	class CReplacePipe {
	public:
		CReplacePipe(LPCTSTR sText);;

		operator LPCTSTR() const {
			return sm_buffer;
		}

	private:
		static TCHAR sm_buffer[MAX_SIZE_REPLACE_PIPE];
	};
	//------------------------------------------------------------------------------------------------


	// Critical section that is acquired on construction and freed on destruction
	class CAutoCriticalSection {
	public:
		// The critical section mush have been initialized prior to using it with this class
		CAutoCriticalSection(CRITICAL_SECTION & cs) : m_cs(cs) {
			::EnterCriticalSection(&m_cs);
		}

		~CAutoCriticalSection() {
			::LeaveCriticalSection(&m_cs);
		}
	private:
		CRITICAL_SECTION & m_cs;
	};
}