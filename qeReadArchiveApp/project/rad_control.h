/* rad_control.h
 *
 * This file is part of the EPICS QT Framework, initially developed at the
 * Australian Synchrotron.
 *
 * Copyright (c) 2013-2022 Australian Synchrotron
 *
 * The EPICS QT Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The EPICS QT Framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the EPICS QT Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author:
 *    Andrew Starritt
 * Contact details:
 *    andrews@ansto.gov.au
 */

#ifndef RAD_CONTROL_H
#define RAD_CONTROL_H

#include <QObject>
#include <QString>
#include <QTimer>

#include <QCaDateTime.h>
#include <QCaDataPoint.h>
#include <QEArchiveInterface.h>
#include <QEArchiveManager.h>
#include <QEOptions.h>

class Rad_Control : QObject {
Q_OBJECT
public:
   Rad_Control ();
   ~Rad_Control ();

private:
   static const int MaximumPVNames = 20;

   struct PVData {
      QString pvName;
      bool isOkayStatus;
      int responseCount;
      QCaDataPointList archiveData;
   };

   // The rad program is managaed as a simple state machine.
   //
   enum States { setup,
                 initialWait,
                 waitArchiverReady,
                 initialiseRequest,
                 sendRequest,
                 waitResponse,
                 printAll,
                 allDone,
                 errorExit };

   PVData pvDataList [MaximumPVNames];
   int numberPVNames;

   Qt::TimeSpec timeZoneSpec;
   QEArchiveInterface::How how;
   bool useFixedTime;
   double fixedTime;

   QString outputFile;
   QCaDateTime startTime;
   QCaDateTime nextTime;
   QCaDateTime endTime;

   States state;
   int pvIndex;
   int timeout;

   QEOptions *options;
   QTimer* tickTimer;
   QEArchiveAccess * archiveAccess;

   void usage (const QString & message);
   void help ();

   void initialise ();
   void readArchive ();
   void postProcess (struct PVData* pvData);

   void putDatumSet (QTextStream& target, QCaDataPoint p [], const int j, const QCaDateTime & firstTime);
   void putArchiveData ();

   QDateTime value (const QString& s, bool& okay);

   void setTimeout (const double delay);

   // Convert time to timeZoneSpec zone.
   //
   QDateTime toRadTime (const QDateTime dateTime) const;

private slots:
   static void printFile (const QString&  filename,
                          std::ostream& stream);         // Print file to stream

   void tickTimeout ();
   void setArchiveData (const QObject* userData, const bool okay,
                        const QCaDataPointList& archiveData,
                        const QString& pvName, const QString& supplementary);

};

#endif  // RAD_CONTROL_H 
