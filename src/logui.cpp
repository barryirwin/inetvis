#include <logui.h>
#include <iostream>

using namespace std;


bool LogUI::enabled = false; //by default
QString LogUI::logFileName;
QFile LogUI::logFile;
Q3TextStream LogUI::logStream;      


bool LogUI::isEnabled()
{
   return enabled;

}


bool LogUI::enable()
{
   if (openFile())
      enabled = true;
   else 
      enabled = false;

   return enabled;

}


void LogUI::disable()
{

   enabled = false;
   closeFile();

}


bool LogUI::openFile()
{
   //check that logui dir exists and make the dir if necessary
   if (!QDir::current().exists("logui"))
   {  if (!QDir::current().mkdir("logui"))
         return false;
   }

   //set log file name according to the starting timestamp
   QString startTime = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");
   logFileName = QString("ui-" + startTime + ".log");
   logFile.setName("logui/" + logFileName);
   
   //open log file - note we do not need to append to pre-existing logs, since 
   //we open one log file per user session and never append to it again
   if (logFile.open(QIODevice::WriteOnly))
   {  
      //connect logstream to file
      logStream.setDevice(&logFile);

      //write opening tag to file
      logEvent("UI log file opened");

      return true;
   }
   else
      return false;
   

}


void LogUI::closeFile()
{
   //write closing tag to file - this will indicate successful closure
   logEvent("UI log file closed");
   logStream.unsetDevice();
   logFile.close();

}


void LogUI::logEvent(QString msg)
{
   //get current timestamp
   QString ts = QDateTime::currentDateTime().toString("yyyy/MM/dd-hh:mm:ss.zzz");

   logStream << ts << " := " << msg << '\n';

}


void LogUIQuit::close()
{
   //a slot meant to recieve qt application call to quit - should be connected
   //in main.c

   if (LogUI::isEnabled())
      LogUI::disable();

}