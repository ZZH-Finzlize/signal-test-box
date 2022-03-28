#pragma once
#include "ui_mainwindow.h"

#include <QString>
#include <QDebug>
#include <QMainWindow>
#include <QtCore>
#include <QMessageBox>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    Ui::MainWindow ui;
public:
    MainWindow(QWidget* parent = nullptr);
};
