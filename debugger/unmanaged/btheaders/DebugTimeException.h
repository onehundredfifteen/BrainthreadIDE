#pragma once

#include <exception>
#include <stdexcept>
#include <sstream>
#include <string>


class DebugTimeException : public std::exception {
public:

  DebugTimeException(const char * msg, int ec = 0)
    : exception(""), message(msg), error_code(ec)
    {}

  virtual const char* what() const throw()
  {
	  cnvt.str("");
	  if(error_code == 0)
		  cnvt << "Debugger exception: " << message;
	  else
		  cnvt << "Debugger exception: " << message << ". Error code: " << error_code;
	  
    
      s = cnvt.str();
      return s.c_str();
  }

protected:
    static std::ostringstream cnvt;
	static std::string s;
	int error_code;
	const char * message;
};


