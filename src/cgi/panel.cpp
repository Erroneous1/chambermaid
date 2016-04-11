/*
 * Distrubuted under The MIT License (MIT)
 *
 * Copyright (c) 2016 Aaron Bishop
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Options.hpp"
#include <iostream>
#include <json/json.h>
#include <cstdlib>
#include <fcgi_stdio.h>
#include "authorities.hpp"
#include "util/json.hpp"
#include "util/QEntry.hpp"
#include "tables/lcd.hpp"
#include "tables/groups.hpp"
#include "sqlpp11/sqlpp11.h"

int main(int argc, const char** argv) {
	while(FCGI_Accept()) {
		try {
				brewpipp::Options ops;
			if(ops.secure_only && !brewpipp::Options::is_secure()) {
				std::string uri(getenv("REQUEST_URI"));
				const auto pos = uri.find(':');
				if(pos != std::string::npos) {
					uri.insert(uri.begin()+pos,1,'s');
				}
				qcgires_redirect(nullptr, uri.c_str());
				FCGI_Finish();
				continue;
			}
			brewpipp::util::QEntry req(qcgireq_parse(nullptr, Q_CGI_ALL));
			sqlpp::odbc::connection db(ops.sql_config);
			brewpipp::Authorities auth(req, db);
			if(!auth.viewStatus()) {
				FCGI_SetExitStatus(403);
				FCGI_Finish();
				continue;
			}
			const brewpipp::tables::Groups::group_id_t group_id = req.req->getint(req.req, "G");
			const brewpipp::tables::LCD lcd;
			bool no_results(true);
			auto select_lines = db(sqlpp::select(lcd.lcd_line_1, lcd.lcd_line_2, lcd.lcd_line_3, lcd.lcd_line_4)
					.from(lcd)
					.where(lcd.group_id == group_id));
			if(select_lines.empty()) {
				FCGI_SetExitStatus(403);
				FCGI_Finish();
				continue;
			} else {
				qcgires_setcontenttype(req.req, "application/json");
				const auto& lines = *select_lines.begin();
				std::cout <<
					Json::FastWriter().write(
						Json::FromValueSet(Json::ValueSet({
							{"lines",
							Json::FromValueList(Json::ValueList({
								Json::Value(lines.lcd_line_1),
								Json::Value(lines.lcd_line_2),
								Json::Value(lines.lcd_line_3),
								Json::Value(lines.lcd_line_4)
							}))}
						}))
					) << std::endl;
				FCGI_Finish();
				continue;
			}
		} catch(const std::exception& e) {
			std::cerr << "Error encountered in " __FILENAME__ ": " << e.what() << std::endl;
			FCGI_SetExitStatus(500);
			FCGI_Finish();
		}
	}
	std::cerr << "finished";
	if(argc != 0) {
		std::cerr << ' ' << argv[0];
	}
	std::cerr << std::endl;
	return 0;
}