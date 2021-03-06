#ifndef REFERENCEFRAMESETTINGSDIALOGWIDGET_H
#define REFERENCEFRAMESETTINGSDIALOGWIDGET_H

/*******************************************************************
InetVis - Internet Visualisation
Version: 2.1.0
release date: 2017/09/21

Original Authors: Jean-Pierre van Riel, Barry Irwin
Initvis 2.x Authors: Yestin Johnson, Barry Irwin
Rhodes University
Computer Science Honours Project - 2005
Computer Science Masters Project - 2006/7
Computer Science Masters Project - 2017
author: Jean-Pierre van Riel
supervisor: Barry Irwin

InetVis - Internet Visualisation for network traffic.
Copyright (C) 2006-2017, Jean-Pierre van Riel, Barry Irwin, Yestin Johnson

*******************************************************************/

#include <QDialog>
#include "ui_referenceframesettingsdialog.h"
#include "logui.h"

class ReferenceFrameSettingsDialogWidget : public QDialog, public Ui::ReferenceFrameSettingsDialog {
    Q_OBJECT
public:
    ReferenceFrameSettingsDialogWidget(QWidget *parent = 0);

signals:
    void setGridLines(bool, bool, bool, bool, bool, bool, bool, float);
    void setReferenceFrame(bool, bool, bool, bool, bool);
    void setPartitions(int, int, int);
    void setPerspectiveProjection();
    void setOrthographicProjection();
    void displayAxisLabels(bool);
    void displayDateTime(bool);
    void displayFramerate(bool);

public slots:
    void applyReferenceFrame();
    void applyGrid();
    void applyPartitions();
    void axisLabelsStateChange(bool);
    void dateTimeLabelStateChange(bool);
    void framerateLabelStateChange(bool);
    void projectionModeChanged();

private:

};
#endif // REFERENCEFRAMESETTINGSDIALOGWIDGET_H
