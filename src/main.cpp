/******************************************************************************

InetVis - Internet Visualisation

version: 0.9.5
release date: 2007/11/21

Computer Science Honours Project - 2005
Computer Science Masters Project - 2006/7

author: Jean-Pierre van Riel
supervisor: Barry Irwin

Copyright
---------

Creative Commons 'Attribution-NonCommercial-ShareAlike 2.5'
http://creativecommons.org/licenses/by-nc-sa/2.5/
You are free:
* to copy, distribute, display, and perform the work
* to make derivative works
Under the following conditions:
* [by] Attribution. You must attribute the work in the manner specified by the
author or licensor.
* [nc] Noncommercial. You may not use this work for commercial purposes.<br>
* [sa] Share Alike. If you alter, transform, or build upon this work, you may
distribute the resulting work only under a license identical to this one.
- For any reuse or distribution, you must make clear to others the license
terms of this work.
- Any of these conditions can be waived if you get permission from the
copyright holder.<br>

******************************************************************************/


//Platform directives

#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <qgl.h>
#include "logui.h"
#include "ui_controlpanel.h"
#include "ui_visdisplay.h"
#include "ui_plottersettingsdialog.h"
#include "ui_referenceframesettingsdialog.h"
#include "dataproc.h"


class ControlPanelWidget : public QMainWindow, public Ui::ControlPanel {

   Q_OBJECT

   public:
      ControlPanelWidget(QMainWindow *parent = 0) : QMainWindow(parent) {
      setupUi(this);
   }
};


class VisDisplayWidget : public QWidget, public Ui::VisDisplay {

   Q_OBJECT

   public:
      VisDisplayWidget(QWidget *parent = 0) : QWidget(parent) {
      setupUi(this);
   }
};


class PlotterSettingsDialogWidget : public QDialog, public Ui::PlotterSettingsDialog {

   Q_OBJECT

   public:
      PlotterSettingsDialogWidget(QDialog *parent = 0) : QDialog(parent) {
      setupUi(this);
   }
};


class ReferenceFrameSettingsDialogWidget : public QDialog, public Ui::ReferenceFrameSettingsDialog {

   Q_OBJECT

   public:
      ReferenceFrameSettingsDialogWidget(QDialog *parent = 0) : QDialog(parent) {
      setupUi(this);
   }
};


int main(int argc, char **argv) {
   ios::sync_with_stdio(); //since both c++ streams and c printf are used for
         //debugging code output

   QApplication app(argc, argv);

   //Test and ensure that system has OpenGL support
   if (!QGLFormat::hasOpenGL()) {  
      qWarning( "This system has no OpenGL support. Exiting.");
      return -1;
   }

//*

   //uncomment the two lines below to diable user interface logging
   //if(!LogUI::enable())  // facilitates logging user interaction
   //   qWarning("Unable to open file for user interface logging - feature left disabled.");

   //declare support objects
   DataProcessor dp;  //provides backend for reading and parsing capture files
   LogUIQuit luiq; //small helper object to ensure UI logging closes gracefully

   //declare GUI forms/windows
   ControlPanelWidget cp; //control panel
   VisDisplayWidget vd; //visaulization window
   PlotterSettingsDialogWidget ps; //to set plotting features
   ReferenceFrameSettingsDialogWidget rfs; //to set reference frame features

   //setup object reference links
   GLVisWidget *vdw = vd.getPtrToVisPane();
   vdw->setDataProcLink(dp.getDataProcessorPtr());
   //dp.setGLVisWidgetLink(vdw);

   //connect signals and slots for control panel
   QObject::connect(vdw, SIGNAL(showControlPanel()), &cp, SLOT(showOnTop()));
   QObject::connect(&dp, SIGNAL(updateReplayTimeDisplay(const QDateTime &,
         const QDateTime &)), &cp, SLOT(updateReplayTimeDisplay(
         const QDateTime &, const QDateTime &)));
   QObject::connect(&dp, SIGNAL(playing()), &cp, SLOT(playing()));
   QObject::connect(&dp, SIGNAL(paused()), &cp, SLOT(paused()));
   QObject::connect(&dp, SIGNAL(setStatusBarMessage(QString)), &cp,
         SLOT(statusBarMessage(QString)));
   QObject::connect(&dp, SIGNAL(updateReplayRateDisplay(double)), &cp,
         SLOT(updateReplayRate(double)));
   QObject::connect(&dp, SIGNAL(updateTimeWindowDisplay(double)),
         &cp, SLOT(updateTimeWindowDisplay(double)));
   QObject::connect(&dp, SIGNAL(setTimeRange(const QDateTime, const QDateTime)),
         &cp, SLOT(setTimeRange(const QDateTime, const QDateTime)));
   QObject::connect(&dp, SIGNAL(setRecordButton(bool)),
         &cp, SLOT(setRecordButton(bool)));
   QObject::connect(&dp, SIGNAL(sendErrMsg(QString)), &cp, SLOT(reportErrorMessage(QString)));

   //connect signals and slots for visualization display window
   //and the visualization display widget
   QObject::connect(&cp, SIGNAL(showVisDisplayPanel()), &vd, SLOT(show()));
   QObject::connect(&dp, SIGNAL(updateGLVisWidget()), &vd, SLOT(updateGLVisWidget()));
   QObject::connect(&rfs, SIGNAL(setPerspectiveProjection()), vdw,
         SLOT(setPerspectiveProjection()));
   QObject::connect(&rfs, SIGNAL(setOrthographicProjection()), vdw,
         SLOT(setOrthographicProjection()));
   QObject::connect(&rfs, SIGNAL(setReferenceFrame(bool, bool, bool, bool, bool)),
         vdw, SLOT(setReferenceFrame(bool, bool, bool, bool, bool)));
   QObject::connect(&rfs,
         SIGNAL(setGridLines(bool, bool, bool, bool, bool, bool, bool, float)),
         vdw,
         SLOT(setGridLines(bool, bool, bool, bool, bool, bool, bool, float)));
   QObject::connect(&rfs, SIGNAL(setPartitions(int, int, int)), vdw,
         SLOT(setPartitions(int, int, int)));
   QObject::connect(&ps, SIGNAL(changePointSize(int)), vdw,
         SLOT(setPointSize(int)));
   QObject::connect(&cp, SIGNAL(captureFrames(bool)), vdw,
         SLOT(captureFrames(bool)));
   QObject::connect(&cp, SIGNAL(captureSingleFrame()), vdw,
         SLOT(captureCurrentFrame()));
   QObject::connect(&dp, SIGNAL(setXAxisLabels(const QString, const QString)), vdw,
         SLOT(setXAxisLabels(const QString, const QString)));
   QObject::connect(&dp, SIGNAL(setYAxisLabels(const QString, const QString)), vdw,
         SLOT(setYAxisLabels(const QString, const QString)));
   QObject::connect(&dp, SIGNAL(setZAxisLabels(const QString, const QString)), vdw,
         SLOT(setZAxisLabels(const QString, const QString)));
   QObject::connect(&rfs, SIGNAL(displayAxisLabels(bool)), vdw,
         SLOT(displayAxisLabels(bool)));
   QObject::connect(&rfs, SIGNAL(displayDateTime(bool)), vdw,
         SLOT(displayDateTime(bool)));
   QObject::connect(&rfs, SIGNAL(displayFramerate(bool)), vdw,
         SLOT(displayFramerate(bool)));
   QObject::connect(&dp, SIGNAL(setDateTimeReference(QDateTime*)), vdw,
         SLOT(setDateTimeReference(QDateTime*)));
   QObject::connect(&ps, SIGNAL(setTransparentDecay(bool)), vdw,
         SLOT(enableTransDecay(bool)));
   QObject::connect(&ps, SIGNAL(setPointSmoothing(bool)), vdw,
         SLOT(enablePointSmoothing(bool)));
   QObject::connect(&ps, SIGNAL(setBackgroundColour(int)), vdw,
         SLOT(setBackgroundCol(int)));

   //connect signals and slots for the plotting settings dialog
   QObject::connect(&cp, SIGNAL(showPlotterSettings()), &ps, SLOT(show()));
   QObject::connect(vdw, SIGNAL(showPlotterSettings()), &ps, SLOT(show()));
   QObject::connect(&dp, SIGNAL(updateHomeNetworkDisplay(int, int, int, int, int,
         QString)), &ps, SLOT(updateHomeNetworkAddress(int, int, int, int,
         int, QString)));
   QObject::connect(&dp, SIGNAL(updateSourceNetworkDisplay(int, int, int, int, int,
         QString)), &ps, SLOT(updateInternetNetworkRange(int, int, int, int,
         int, QString)));
   QObject::connect(&dp, SIGNAL(updatePortRangeDisplay(int, int)), &ps,
         SLOT(updatePortRange(int, int)));

   //connect siganls and slots for the reference frame dialog
   QObject::connect(&cp, SIGNAL(showReferenceFrameSettings()), &rfs, SLOT(show()));
   QObject::connect(vdw, SIGNAL(showReferenceFrameSettings()), &rfs, SLOT(show()));

   //connect signals and slots for data processor
   //dp.connect(&cp, SIGNAL(selectMode(int)), &dp, SLOT(setMode(int)));
   //from control panel
   QObject::connect(&cp, SIGNAL(play()), &dp, SLOT(play()));
   QObject::connect(&cp, SIGNAL(pause()), &dp, SLOT(pause()));
   QObject::connect(&cp, SIGNAL(togglePlayPause()), &dp, SLOT(togglePlayPause()));
   QObject::connect(&cp, SIGNAL(setReplayRate(double)), &dp,
         SLOT(setReplayRate(double)));
   QObject::connect(&cp, SIGNAL(setReplayPosition(const QDateTime)), &dp,
         SLOT(setReplayPosition(const QDateTime)));
   QObject::connect(&cp, SIGNAL(setTimeWindow(double)), &dp,
         SLOT(setTimeWindow(double)));
   QObject::connect(&cp, SIGNAL(selectReplayFile(const QString)), &dp,
         SLOT(selectReplayFile(const QString)));
   QObject::connect(&cp, SIGNAL(setFilter(const QString)), &dp,
         SLOT(setFilter(const QString)));
   QObject::connect(&cp, SIGNAL(setHomeNetwork(int, int, int, int, int)), &dp,
         SLOT(setHomeNetwork(int, int, int, int, int)));
   QObject::connect(&cp, SIGNAL(selectNetworkInterface(const QString)), &dp,
         SLOT(selectNetworkInterface(const QString)));
   QObject::connect(&cp, SIGNAL(recordToFile(bool)), &dp, SLOT(record(bool)));
   //from plotter settings
   //dp.connect(&ps, SIGNAL(), &dp, SLOT());
   QObject::connect(&ps, SIGNAL(setHomeNetwork(int, int, int, int, int)),
         &dp, SLOT(setHomeNetwork(int, int, int, int, int)));
   QObject::connect(&ps, SIGNAL(setSourceNetwork(int, int, int, int, int)),
         &dp, SLOT(setSourceNetwork(int, int, int, int, int)));
   QObject::connect(&ps, SIGNAL(setColourScheme(int)), &dp,
         SLOT(setColourScheme(int)));
   QObject::connect(&ps, SIGNAL(setPortRange(int, int, int, int)), &dp,
         SLOT(setPortRange(int, int, int, int)));
   QObject::connect(&ps, SIGNAL(guessHomeNetwork()), &dp, SLOT(guessHomeNetwork()));
   QObject::connect(&ps, SIGNAL(setTransparentDecay(bool)), &dp,
         SLOT(setTransparentDecay(bool)));
   QObject::connect(&ps, SIGNAL(setPointBulge(bool)), &dp,
         SLOT(enablePointBulge(bool)));
   QObject::connect(&ps, SIGNAL(setBackgroundColour(int)), &dp,
         SLOT(changeBackgroundColour(int)));

   //connect slot for reciving quit signal
   QObject::connect(&app, SIGNAL(lastWindowClosed()), &luiq, SLOT(close()));
   QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

   dp.init();

   // qt 4 requires linking uis to widget object, then calling show on the widget
   // not sure if this works with two, but trying it..
   vd.show(); //show display first so that control panel begins on top
   cp.show();

   return app.exec();
}
