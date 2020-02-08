#ifndef SRC_DATALOGGER_H_
#define SRC_DATALOGGER_H_
#include "frc/WPILib.h"
#include <iostream>
#include <fstream>
#include <ctime>

class DataLogger {
public:
	DataLogger();
	DataLogger(std::string name);
	static std::string GetTimestamp();
	void Flush();
	void Update();
	void StartSession();
	void EndSession();
    void VisionLogger(std::string name);
    void WriteVisionData(bool targetLocked, bool visionDriveActive, double distance, double distanceError, double angleError, double speed, double omega);
	virtual ~DataLogger();
private:
	std::ofstream tLog;
	std::string mFilename;
	frc::Timer LogTimer;
};

#endif /* SRC_DATALOGGER_H_ */