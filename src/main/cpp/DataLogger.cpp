#include "DataLogger.h"

DataLogger::DataLogger() {
	// TODO Auto-generated constructor stub
}

std::string DataLogger::GetTimestamp() {
	time_t rawtime;
  	struct tm * timeinfo;
  	char buffer[80];

  	time (&rawtime);
  	timeinfo = localtime(&rawtime);

  	strftime(buffer,sizeof(buffer),"%F_%H_%M",timeinfo);
  	std::string timeString(buffer);
	return timeString;
}

void DataLogger::Update() {
	if (tLog.is_open()) {
		tLog << LogTimer.Get() << "," 
				<< std::endl;
	}
}

void DataLogger::Flush() {
	tLog.flush();
}

void DataLogger::StartSession() {
	if (!tLog.is_open()) {
		tLog.open(mFilename, std::ofstream::out | std::ofstream::app);
	}
	LogTimer.Reset();
	LogTimer.Start();
}

void DataLogger::EndSession() {
	if (tLog.is_open()) {
		//Flush();
		tLog.close();
	}
	LogTimer.Stop();
}

DataLogger::~DataLogger() {
	// TODO Auto-generated destructor stub
	if (tLog.is_open()) {
		tLog.close();
	}
}

void DataLogger::VisionLogger(std::string name) {
    printf("construct\n");
	mFilename = name;
	tLog.open(mFilename, std::ofstream::out | std::ofstream::trunc);
	tLog << "Time,TargetLocked,VisionDriveActive,Distance,DistanceError,AngleError,Speed,Omega" << std::endl;
}

void DataLogger::WriteVisionData(bool targetLocked, bool visionDriveActive, double distance, double distanceError, double angleError, double speed, double omega) {
    tLog << std::to_string(LogTimer.Get()) + "," +  std::to_string(targetLocked) + "," + std::to_string(visionDriveActive) + "," + std::to_string(distance) + "," + std::to_string(distanceError) + "," + std::to_string(angleError) + "," + std::to_string(speed) + "," + std::to_string(omega) << std::endl;
}