#include "csv.h"

CSV::CSV(QWidget *parent) :
    QWidget(parent), _separator(';')
{
                textEdit = new QTextEdit;
                textEdit->setMaximumHeight(30);
                textEdit->setMaximumWidth(200);

                openButton = new QPushButton(tr("Open"));
                saveButton = new QPushButton(tr("Save"));
                findButton = new QPushButton(tr("Find"));
                quitButton = new QPushButton(tr("Quit"));
                tableWidget = new QTableWidget(this);

                connect(openButton, SIGNAL(clicked()), this, SLOT(open()));
                connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
                connect(findButton, SIGNAL(clicked()), this, SLOT(find()));
                connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));

                QHBoxLayout *hLayout = new QHBoxLayout;
                hLayout->addWidget(openButton);
                hLayout->addWidget(saveButton);
                hLayout->addWidget(quitButton);

                QHBoxLayout *findLayout = new QHBoxLayout;
                findLayout->addWidget(textEdit);
                findLayout->addWidget(findButton);

                QVBoxLayout *layout = new QVBoxLayout;
                layout->addLayout(hLayout);
                layout->addLayout(findLayout);
                layout->addWidget(tableWidget);
                this->resize(500,500);
                setLayout(layout);

                setWindowTitle(tr("CSVReader"));
}

//Open file and show tableWidget
void CSV::open()
     {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
            tr("CSV Files (*.csv)"));

        if (fileName != ""){
            this->Read(fileName);
        }
    }

//Save to file *.csv
void CSV::save()
     {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
        tr("CSV Files (*.csv)"));

        if (fileName != "")  {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly))  {
                QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
                return;
            } else  {
                //write tableWidget to file
                QTextStream stream(&file);
                QStringList list_elem;
                int k = 0;
                foreach(list_elem,_lines_list){
                     for(int i = 0 ; i < list_elem.size(); ++i){
                         stream<<tableWidget->item(k,i)->text() <<";";
                         if(i==list_elem.size()-1) stream<<"\n";
                     }
                     k++;
                 }
                stream.flush();
                file.close();
            }
        }
    }

//Find string
void CSV::find(){

QString str;
    if(!textEdit->toPlainText().size()){
        textEdit->setStyleSheet("background-color: #ff2500");
    }
    else{
        textEdit->setStyleSheet("background-color: #ffffff");
        str = textEdit->toPlainText();

        QStringList list_elem;
        QString elem;
        int i = 0, k = 0;
        foreach(list_elem,_lines_list){
            i=0;
            foreach(elem,list_elem){
                 if(tableWidget->item(k,i)->text() == str) {
                     tableWidget->selectionModel()->select(tableWidget->model()->index(k, i), QItemSelectionModel::Select);
                 }
                 i++;
             }
            k++;
          }
        }

}

//Exit
void CSV::quit(){
    close();
}


//Delete comments
QString CSV::clearCSV(QString item){
    if((!item.isEmpty())&&(item[0] == QChar(34)))
        item.remove(0,1);
    if((!item.isEmpty())&&(!item.isNull())&(item[item.count()-1] == QChar(34)))
        item.remove(item.count()-1,1);
    if(!item.isEmpty())
        item = item.replace("\"\"","\"");
    return item;

}

//Read and Parsing file with fileName
void CSV::Read(QString fileName)
{
    _lines_list.clear();
    int count_column = 0, count_row = 0;

    if (fileName != ""){
        bool Quote = false;
        QList<QString> ItemList;
        QString item = "";
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))  {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        else {
            //Parsing CSV
            QTextStream out(&file);
            while(!out.atEnd()){
                QString line(out.readLine().simplified());
                int count = line.count();
                for (int i = 0;i < count; i++){
                    if (line[i] == QChar(34)){
                        Quote = (Quote) ? false : true;
                    }
                    if ((Quote != true && line[i] == _separator)){
                        ItemList.append(this->clearCSV(item));
                        item = "";
                    }else{
                        item += line[i];
                    }

                    if ((count-1 == i)&(Quote != true)){
                        item = this->clearCSV(item);
                        if (item != "")
                            ItemList.append(item);
                        _lines_list.append(ItemList);
                        count_column=ItemList.size();
                        ItemList.clear();
                        item = "";
                    }
                }
                file.close();
              }
       }
    }

    //Set Column and Row for tableWidget
    count_row=_lines_list.size();
    tableWidget->setColumnCount(count_column);
    tableWidget->setRowCount(count_row);

    //Write to tableWidget
    QStringList list_elem;
    QString elem;
    int i = 0, k = 0;
    foreach(list_elem,_lines_list){
         i = 0;
         foreach(elem,list_elem){
             tableWidget->setItem(k,i,new QTableWidgetItem(elem));
             i++;
         }
         k++;
     }
}

CSV::~CSV(){
    close();
}
