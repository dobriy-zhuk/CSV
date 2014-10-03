#ifndef CSV_H
#define CSV_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

class CSV : public QWidget
{
    Q_OBJECT
public:
    explicit CSV(QWidget *parent = 0);
    ~CSV();
    void Read(QString fileName);
    QString clearCSV(QString item);

private:
            QTextEdit *textEdit;
            QPushButton *openButton;
            QPushButton *saveButton;
            QPushButton *findButton;
            QPushButton *quitButton;
            QTableWidget *tableWidget;

            QList<QStringList> _lines_list;
            QChar _separator;

private slots:
            void open();
            void save();
            void find();
            void quit();

};

#endif // CSV_H
