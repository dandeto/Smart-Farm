#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <string>
#include <iostream>
#include <QFileDialog>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QFile file("LogData.csv");
    QString data;
    QStringList rowOfData;
    QStringList rowData;
    data.clear();
    rowOfData.clear();
    rowData.clear();

    if (file.open(QFile::ReadOnly))
    {
        data = file.readAll();
        rowOfData = data.split("\n");
        file.close();
    }
    qDebug()<<rowOfData;
    for(int x = 0; rowOfData.size(); x++)
    {
        rowData = rowOfData.at(x).split(";");
        qDebug()<<rowData;
        for(int y = 0; y < rowData.size(); y++)
        {
            ui->tableWidget->item(x,y)->setText(rowData[y]);
        }
    }
}

