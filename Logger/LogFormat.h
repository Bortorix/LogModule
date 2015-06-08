
#ifndef _LOG_FORMAT_H_
#define _LOG_FORMAT_H_

namespace LoggerSp {

	class LogFormat {
	public:
		enum LogFmtColumns {
			Time    = 1,  // Write fixed time of message
			Prior   = 2,  // Write Priority Level
			Fname   = 4,  // Filename of source code
			FunName = 8,  // Function name
			RowNum  = 16  // Row number
		};

		LogFormat () {clm = (LogFmtColumns)0;}
		LogFormat (const LogFormat &lf) {clm = lf.clm;}
		LogFormat (const unsigned int &_clm) {clm = _clm;}

		void addColumns (const LogFmtColumns &fmtColumns) {clm |= fmtColumns;}
		void remColumns (const LogFmtColumns &fmtColumns) {clm &= ~fmtColumns;}
		
		bool isContains (const LogFmtColumns &fmtColumns) const {return (clm & fmtColumns) > 0;}
		unsigned int columns () const {return clm;}

	private:
		unsigned int clm;
	};
}

#endif // _LOG_FORMAT_H_