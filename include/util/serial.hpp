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

#ifndef BREWPIPP_UTIL_SERIAL_HPP
#define BREWPIPP_UTIL_SERIAL_HPP

#include <string>
#include <json/value.h>
#include "util/BufferedIO.hpp"


namespace brewpipp {
	namespace util {
		struct FwVersionInfo {
			Json::Value version;
			Json::Value is_simulator;
			Json::Value log;
			Json::Value build;
			Json::Value commit;
			Json::Value shield;
			Json::Value board;
			
			FwVersionInfo(const Json::Value& reply);
			FwVersionInfo() = default;
			~FwVersionInfo() {};
			std::string to_string() const;
		};
		
		std::string strip_carriage_return(const std::string& s);
		
		struct Temperatures {
			double beerReading;
			double beerSetting;
			std::string beerAnnotation;
			double fridgeReading;
			double fridgeSetting;
			std::string fridgeAnnotation;
			double roomTemp;
			enum class State : int8_t {
				Idle = 0,
				StateOff = 1,
				DoorOpen = 2,
				Heating = 3,
				Cooling = 4,
				WaitingToCool = 5,
				WaitingToHeat = 6,
				WaitingForPeakDetect = 7,
				CoolingMinTime = 8,
				HeatingMinTime = 9,
				MAX_VALUE = 10,
				Null = -1
			} state;
			std::string to_string();
			Temperatures(const Json::Value& reply);
			Temperatures();
		};
		
		struct ControlSettings {
			enum class Mode : char {
				FridgeConstant = 'f',
				BeerConstant = 'b',
				BeerProfile = 'p',
				Off = 'o',
				Test = 't',
				Null = 0
			} mode;
			double beerSetting;
			double fridgeSetting;
			double heatEstimator;
			double coolEstimator;
			std::string to_string();
			ControlSettings(const Json::Value& reply);
			ControlSettings() = default;
		};
		
		struct ControlConstants {
			enum class TempFormat : char {
				Celcius = 'C',
				Fahrenheit = 'F'
			} tempFormat;
			typedef uint8_t filter_t;
			double tempSettingMin;
			double tempSettingMax;
			double pidMax;
			double Kp;
			double Ki;
			double Kd;
			double iMaxError;
			double idleRangeHigh;
			double idleRangeLow;
			double heatingTargetUpper;
			double heatingTargetLower;
			double coolingTargetUpper;
			double coolingTargetLower;
			time_t maxHeatTimeForEstimate;
			time_t maxCoolTimeForEstimate;
			filter_t fridgeFastFilter;
			filter_t fridgeSlowFilter;
			filter_t fridgeSlopeFilter;
			filter_t beerFastFilter;
			filter_t beerSlowFilter;
			filter_t beerSlopeFilter;
			bool lightAsHeater;
			bool rotaryHalfSteps;
			std::string to_string();
			ControlConstants(const Json::Value& reply);
			ControlConstants() = default;
		};
		
		struct ControlVariables {
			double beerDiff;
			double diffIntegral;
			double beerSlope;
			double p;
			double i;
			double d;
			double estPeak;
			double negPeakEst;
			double posPeakEst;
			double negPeak;
			double posPeak;
			std::string to_string();
			ControlVariables(const Json::Value& reply);
			ControlVariables() = default;
		};
		
		FwVersionInfo get_version_info(BufferedIO& buffered_io) throw(boost::system::system_error,BufferedIO::TimeoutException);
		Temperatures get_temperatures(BufferedIO& buffered_io) throw(boost::system::system_error,BufferedIO::TimeoutException);
		ControlSettings get_control_settings(BufferedIO& buffered_io) throw(boost::system::system_error,BufferedIO::TimeoutException);
		ControlConstants get_control_constants(BufferedIO& buffered_io) throw(boost::system::system_error,BufferedIO::TimeoutException);
		ControlVariables get_control_variables(BufferedIO& buffered_io) throw(boost::system::system_error,BufferedIO::TimeoutException);
	}
}

#endif //BREWPIPP_UTIL_SERIAL_HPP