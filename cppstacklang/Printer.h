#ifndef PRINTER_H
#define PRINTER_H

#include "Data.h"
#include "Token.h"
#include <boost/variant.hpp>
#include <sstream>

class Printer {
	public:
	static const char* out(token_t t) {
		switch(t) {
			case TOK_INT:
				return "int";
			break;
			case TOK_REF:
				return "ref";
			break;
			case TOK_REF_DEF:
				return "ref_def";
			break;
			case TOK_VAR_OPE:
				return "var_ope";
			break;
			case TOK_MAT_OPE:
				return "mat_ope";
			break;
			case TOK_BLO_REF:
				return "blo_ref";
			break;
			default:
				break;
		}
		return "";
	}
	
	static std::string out(data t) {
		int which = t.which();
		if(which == DATA_V_BLO_REF) {
			std::ostringstream ostr;
			ostr << boost::get<blo_ref_t>(t).ref << "r ";
    		return ostr.str();
		}
    	if(which == DATA_V_INT) {
			
			std::ostringstream ostr;
    		ostr << boost::get<int>(t) << " ";
    		return ostr.str();
    	
    	}
    	if(which == DATA_V_DOUBLE) {
			
			std::ostringstream ostr;
			ostr << boost::get<double>(t) << " ";
    		return ostr.str();
    	}
    	if(which == DATA_V_VAR) {
    		
			std::ostringstream ostr;
    		ostr << boost::get<var_t>(t).val << "v ";
    		return ostr.str();
		}
    	if(which == DATA_V_STRING) {
    	
			std::ostringstream ostr;
    		ostr << boost::get<std::string>(t) << "v ";
    		return ostr.str();
    	}
    	return std::string("");
	}
};

#endif
