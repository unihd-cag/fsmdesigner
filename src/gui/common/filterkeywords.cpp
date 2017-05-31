/***********************************************************************
 *                                                                      *
 * (C) 2009, Frank Lemke, Computer Architecture Group,                  *
 * University of Mannheim, Germany                                      *
 *                                                                      *
 * This program is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation; either version 2 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program; if not, write to the Free Software          *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 *
 * USA                                                                  *
 *                                                                      *
 * For informations regarding this file contact                         *
 *			      office@mufasa.informatik.uni-mannheim.de *
 *                                                                      *
 ***********************************************************************/


// Includes
//-----------------------

//-- Std
#include <string>
using namespace std;

//-- Qt
#include <QtCore>


#include "filterkeywords.h"

FilterKeywords::FilterKeywords() {
}

FilterKeywords::~FilterKeywords() {

}

QStringList FilterKeywords::InitHDLKeywordslist() {
	QStringList sl = QStringList();
// Verilog Keywords
	sl << "always" << "and" << "assign" << "begin";
	sl << "buf" << "bufif0" << "bufif1" << "case" << "casex" << "casez"
			<< "cmos" << "deassign" << "default" << "defparam";
	sl << "disable" << "edge" << "else";
	sl << "end" << "endcase" << "endmodule" << "endfunction" << "endprimitive"
			<< "endspecify" << "endtable" << "endtask" << "event" << "for"
			<< "force" << "forever" << "for" << "function" << "highz0"
			<< "highz1" << "if";
	sl << "ifnone" << "initial" << "inout" << "input" << "integer" << "join"
			<< "large" << "macromodule" << "medium" << "module" << "nand"
			<< "negedge" << "nmos" << "nor" << "not" << "notif0" << "notif1";
	sl << "or" << "output" << "parameter" << "pmos" << "posedge" << "primitive"
			<< "pull0" << "pull1" << "pullup" << "pulldown" << "rcmos" << "real"
			<< "realtime" << "reg" << "release" << "repeat" << "rnmos";
	sl << "rpmos" << "rtran" << "rtranif0" << "rtranif1" << "scalared"
			<< "small" << "specify" << "specparam" << "strong0" << "strong1"
			<< "supply0" << "supply1" << "table" << "task" << "time" << "tran"
			<< "tranif0";
	sl << "tranif1" << "tri" << "tri0" << "tri1" << "triand" << "trior"
			<< "trireg" << "vectored" << "wait" << "wand" << "weak0" << "weak1"
			<< "while" << "wire" << "wor" << "xnor" << "xor";

// VHDL Keywords
	sl << "abs" << "access" << "after" << "alias" << "all" <<
//"and"<<  // Double, see Verilog list
			"architecture" << "array" << "assert" << "attribute" <<
//"begin"<<
			"block" << "body" << "buffer" << "bus" <<
//"case"<<
			"component" << "configuration" << "constant";
	sl << "disconnect" << "downto"
			<<
//"else"<<
			"elsif"
			<<
//"end"<<
			"entity" << "exit"
			<< "file"
			<<
//"for"<<
//"function"<<
			"generate" << "generic" << "group" << "guarded"
			<<
//"if"<<
			"impure" << "in" << "inertial"
			<<
//"inout";
			sl << "is" << "label" << "library" << "linkage" << "literal"
			<< "loop" << "map" << "mod" <<
//"nand"<<
			"new" << "next" <<
//"nor"<<
//"not"<<
			"null" << "of" << "on" << "open" <<
//"or"<<
			"others";
	sl << "out" << "package" << "port" << "postponed" << "procedure"
			<< "process" << "pure" << "range" << "record" << "register"
			<< "reject" << "return" << "rol" << "ror" << "select" << "severity"
			<< "signal" << "shared" << "sla";
	sl << "sli" << "sra" << "srl" << "subtype" << "then" << "to" << "transport"
			<< "type" << "unaffected" << "units" << "until" << "use"
			<< "variable" <<
//"wait"<<
			"when" <<
//"while"<<
			"with" <<
//"xnor"<<
//"xor";

// C Keywords
			sl << "auto" << "break" <<
//"case"<<
			"char" << "const" << "continue" <<
//"default"<<
			"do";
	sl << "double" <<
//"else"<<
			"enum" << "extern" << "float" <<
//"for"<<
			"goto";
//"if";
	sl << "int" << "long" <<
//"register"<<
//"return"<<
			"short" << "signed" << "sizeof" << "static";
	sl << "struct" << "switch" << "typedef" << "union" << "unsigned" << "void"
			<< "volatile";
//"while";

// System Verilog Keywords
	sl << "alias"
			<<
//"always"<<
			"always_comb" << "always_ff" << "always_latch"
			<<
//"and"<<
			"assert"
			<<
//"assign"<<
			"assume" << "automatic" << "before"
			<<
//"begin"<<
			"bind"
			<< "bins"
			<< "binsof"
			<< "bit"
			<< "break"
			<<
//"buf"<<
//"bufif0"<<
//"bufif1"<<
			"byte"
			<<
//"case"<<
//"casex"<<
//"casez"<<
			"cell" << "chandle" << "class" << "clocking"
			<<
//"cmos"<<
			"config" << "const" << "constraint" << "context" << "continue"
			<< "cover" << "covergroup" << "coverpoint" << "cross" <<
//"deassign"<<
//"default"<<
//"defparam"<<
			"design" <<
//"disable"<<
			"dist" << "do" <<
//"edge"<<
//"else"<<
//"end"<<
//"endcase"<<
			"endclass" << "endclocking" << "endconfig" <<
//"endfunction"<<
			"endgenerate" << "endgroup" << "endinterface";

	sl
			<<
//"endmodule"<<
			"endpackage"
			<<
//"endprimitive"<<
			"endprogram"
			<< "endproperty"
			<<
//"endspecify"<<
			"endsequence"
			<<
//"endtable"<<
//"endtask"<<
			"enum"
			<<
//"event"<<
			"expect"
			<< "export"
			<< "extends"
			<< "extern"
			<< "final"
			<< "first_match"
			<<
//"for"<<
//"force"<<
			"foreach"
			<<
//"forever"<<
			"fork"
			<< "forkjoin"
			<<
//"function"<<
			"generate"
			<< "genvar"
			<<
//"highz0"<<
//"highz1"<<
//"if"<<
			"iff"
			<<
//"ifnone"<<
			"ignore_bins"
			<< "illegal_bins"
			<< "import"
			<< "incdir"
			<< "include"
			<<
//"initial"<<
//"inout"<<
//"input"<<
			"inside" << "instance" << "int" << "integer" << "interface"
			<< "intersect"
			<<
//"join"<<
			"join_any" << "join_none"
			<<
//"large"<<
			"liblist" << "library" << "local" << "localparam" << "logic"
			<< "longint";
//"macromodule";

	sl
			<< "matches"
			<<
//"medium"<<
			"modport"
			<<
//"module"<<
//"nand"<<
//"negedge"<<
			"new"
			<<
//"nmos"<<
//"nor"<<
			"noshowcancelled"
			<<
//"not"<<
//"notif0"<<
//"notif1"<<
			"null"
			<<
//"or"<<
//"output"<<
			"package"
			<< "packed"
			<<
//"parameter"<<
//"pmos"<<
//"posedge"<<
//"primitive"<<
			"priority"
			<< "program"
			<< "property"
			<< "protected"
			<<
//"pull0"<<
//"pull1"<<
//"pulldown"<<
//"pullup"<<
			"pulsestyle_onevent"
			<< "pulsestyle_ondetect"
			<< "pure"
			<< "rand"
			<< "randc"
			<< "randcase"
			<< "randsequence"
			<<
//"rcmos"<<
//"real"<<
//"realtime"<<
			"ref"
			<<
//"reg"<<
//"release"<<
//"repeat"<<
			"return"
			<<
//"rnmos"<<
//"rpmos"<<
//"rtran"<<
//"rtranif0"<<
//"rtranif1"<<
//"scalared"<<
			"sequence" << "shortint" << "shortreal" << "showcancelled"
			<< "signed";

	sl
			<<
//"small"<<
			"solve"
			<<
//"specify"<<
//"specparam"<<
			"static"
			<< "string"
			<<
//"strong0"<<
//"strong1"<<
			"struct"
			<< "super"
			<<
//"supply0"<<
//"supply1"<<
//"table"<<
			"tagged"
			<<
//"task"<<
			"this"
			<< "throughout"
			<<
//"time"<<
			"timeprecision"
			<< "timeunit"
			<<
//"tran"<<
//"tranif0"<<
//"tranif1"<<
//"tri"<<
//"tri0"<<
//"tri1"<<
//"triand"<<
//"trior"<<
//"trireg"<<
			"type" << "typedef" << "union" << "unique" << "unsigned" << "use"
			<< "var" <<
//"vectored"<<
			"virtual" << "void" <<
//"wait"<<
			"wait_order" <<
//"wand"<<
//"weak0"<<
//"weak1"<<
//"while"<<
			"wildcard" <<
//"wire"<<
			"with" << "within";
//"wor"<<
//"xnor"<<
//"xor";

	return sl;
}

QString FilterKeywords::FilterSpecialChars(QString q) {
	QString oldString = q;
	// remove Special Characters for XML, Verilog, VHDL and System C
	// XML
	q.replace(QString("<"), QString("_")); // <
	q.replace(QString(">"), QString("_")); // >
	q.replace(QString("\""), QString("_")); // "
	q.replace(QString("&"), QString("_")); // &
	q.replace(QString("\'"), QString("_")); // '
	// Verilog
	q.replace(QString("\\"), QString("_")); // \
    q.replace(QString("/*"), QString("_"));  // /*
	q.replace(QString("*/"), QString("_")); // */
	q.replace(QString("//"), QString("_")); // //
	q.replace(QString("="), QString("_")); // =
	q.replace(QString("-"), QString("_")); // -
	q.replace(QString("+"), QString("_")); // +
	// VHDL
	q.replace(QString(","), QString("_")); // ,
	q.replace(QString("."), QString("_")); // .
	q.replace(QString(";"), QString("_")); // ;
	//q.replace(QString(":"), QString("_"));  // :
	//q.replace(QString("["), QString("_"));  // [
	//q.replace(QString("]"), QString("_"));  // ]
	q.replace(QString("{"), QString("_")); // {
	q.replace(QString("}"), QString("_")); // }

	q.replace(QString("!"), QString("_")); // !
	q.replace(QString("@"), QString("_")); // @
	q.replace(QString("#"), QString("_")); // #
	q.replace(QString("$"), QString("_")); // $
	q.replace(QString("%"), QString("_")); // %
	q.replace(QString("^"), QString("_")); // ^
	q.replace(QString("*"), QString("_")); // *
	q.replace(QString("\("), QString("_")); // (
	//q.replace(QString("\)"), QString("_"));  // ) does not work
	q.replace(QChar(0x29), QString("_")); // )
	q.replace(QString("`"), QString("_")); // `
	q.replace(QString("~"), QString("_")); // ~
	q.replace(QString("|"), QString("_")); // |
	q.replace(QString("?"), QString("_")); // ?
	q.replace(QString("/"), QString("_")); // /
#ifdef DEBUGFILTERKEYWORDS
	if (oldString != q) {
		qDebug()
				<< "Name contained Special Chars of Verilog, VHDL or C. Replaced them with _.";
	}
#endif

	return q;
}

QString FilterKeywords::FilterBusChars(QString q) {
	QString oldString = q;
	// remove Special Characters used for bus generation

	q.replace(QString(":"), QString("_")); // :
	q.replace(QString("["), QString("_")); // [
	q.replace(QString("]"), QString("_")); // ]

#ifdef DEBUGFILTERKEYWORDS
	if (oldString != q) {
		qDebug()
				<< "Name contained Special Chars of Verilog, VHDL or C. Replaced them with _.";
	}
#endif

	return q;
}

QString FilterKeywords::FilterHDLKeywords(QString q) {
	// remove keywords of Verilog, VHDL and C if they interfere
	// Verilog
	QStringList hdlKeywords = InitHDLKeywordslist();
	QString blank = QString(" ");
	QString blankbefore = "";
	QString blankbeforeandafter = "";
	QString blankafter = "";
	for (int i = 0; i < hdlKeywords.size(); i++) {
		QString curKeyword = hdlKeywords.at(i).toLocal8Bit().constData();
		blankbefore = " " + curKeyword;
		blankbeforeandafter = " " + curKeyword + " ";
		blankafter = curKeyword + " ";
		if (q == curKeyword) //if (q =="and" )
				{
			q.replace(QString(curKeyword), QString("_" + curKeyword));
#ifdef DEBUGFILTERKEYWORDS
			qDebug()
					<< "Name contained Verilog, System Verilog, VHDL or C  exact keyword : "
					<< curKeyword << "Renamed it.";
#endif
		} else if (q.contains(blankbeforeandafter)) //if ( q.contains(" and ") )
				{
			q.replace(QString(curKeyword), QString("_" + curKeyword + "_"));
#ifdef DEBUGFILTERKEYWORDS
			qDebug()
					<< "Name contained Verilog, System Verilog, VHDL or C keyword _key_ : "
					<< curKeyword << "Renamed it.";
#endif
		} else if (q.contains(blankafter)) //if ( q.contains("and ") ) //Problem: Puts 4 Blanks before keyword: Add a decision if it contains "and" at the beginning
				{
			q.replace(QString(curKeyword), QString("_" + curKeyword + "_"));
#ifdef DEBUGFILTERKEYWORDS
			qDebug()
					<< "Name contained Verilog, System Verilog, VHDL or C keyword key_ : "
					<< curKeyword << "Renamed it.";
#endif
		} else if ((q.contains(
				blankbefore) /* && q.length()>=blankbefore.length()*/)) //q.contains(" and")
				{
			q.replace(QString(curKeyword), QString("_" + curKeyword));
#ifdef DEBUGFILTERKEYWORDS
			qDebug()
					<< "Name contained Verilog, System Verilog, VHDL or C  keyword _key : "
					<< curKeyword << "Renamed it.";
#endif
		}
#ifdef DEBUGFILTERKEYWORDS
		//qDebug()<<" Verilog, System Verilog, VHDL or C keyword :"<< hdlKeywords.at(i).toLocal8Bit().constData();
#endif
	}

	return q;
}

//q.replace(QString("module"), QString("_"));
//    if (q =="always"  ||  q.contains(" always") || q.contains(" always ") )
//    {
//        q.replace(QString("always"), QString("_"));
//#ifdef DEBUGFILTERKEYWORDS
//        qDebug()<<"Input Name contained Verilog keyword : always";
//#endif
//    }
//    if (q =="and"  ||  q.contains(" and") || q.contains(" and ") )
//    {
//        q.replace(QString("and"), QString("_"));
//#ifdef DEBUGFILTERKEYWORDS
//        qDebug()<<"Input Name contained Verilog keyword : and";
//#endif
//    }

QString FilterKeywords::RemoveBlanks(QString q) {
	// remove blanks from name
	q.replace(QString(" "), QString("_")); // blank
#ifdef DEBUGFILTERKEYWORDS
	qDebug() << " Removed blanks from name.";
#endif

	return q;
}

QStringList FilterKeywords::ExpandBusNames(string s) {
	// returns a list of names constructed from a bus[7:0] string
	//s = "bus[7:0]";
	QString q = (s.c_str() );
	QStringList busnames = QStringList();
	QString namebeginning;
	if ((q.contains("[") && q.contains(":") && q.contains("]"))) {
		QRegExp rx("[A-Z,a-z,0-9]*\\[[0-9]*:[0-9]*\\]"); // bus[7:0]
		if (rx.indexIn(q) != -1) {
			q.replace(QString("["), QString(":")); // [
			q.replace(QString("]"), QString(":")); // ]  //"bus:7:0:"
			QStringList ql = q.split(":");
			int busstart = 0;
			int busend = 0;
			namebeginning = ql.at(0).toLocal8Bit().constData();
			if (ql.at(1).toInt() > ql.at(2).toInt()) {
				busend = ql.at(1).toInt();
				busstart = ql.at(2).toInt();
			} else {
				busend = ql.at(2).toInt();
				busstart = ql.at(1).toInt();
			}
			int sli;
			for (sli = busstart; sli <= busend; ++sli) {
				QString curName = namebeginning;
				curName.append(QString("%1").arg(sli));
#ifdef DEBUGFILTERKEYWORDS
				qDebug() << " Bus name expanded to: " << curName
						<< " Consists of : " << sli;
#endif
				busnames << curName;
			}
		}
#ifdef DEBUGFILTERKEYWORDS
		qDebug() << " Bus name expanded to individual names.";
#endif
	} else {
		busnames << q;
#ifdef DEBUGFILTERKEYWORDS
		qDebug() << " No bus name to be expanded.";
#endif
	}

	return busnames;
}

