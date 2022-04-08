#include "standarset.h"
#include "ui_standarset.h"
#include <qdebug.h>

standarSet::standarSet(QWidget *parent):
    ui(new Ui::standarset_ui)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&standarSet::click);
}


void standarSet::click(bool checked)
{
//    QStringList filenames={"C:/Users/Administrator/Desktop/Qt_master/DC_3_Qt_5_12_4_MinGW_32_Release/release/savefiles/temptestsB/2022_01_21.xls",
//    "C:/Users/Administrator/Desktop/Qt_master/DC_3_Qt_5_12_4_MinGW_32_Release/release/savefiles/temptestsA/2022_01_21.xls"};

//    QXlsx::Document doc2("C:/Users/Administrator/Desktop/Qt_master/DC_3_Qt_5_12_4_MinGW_32_Release/release/savefiles/testA/2022_01_21.xls");


//    doc2.addSheet("1");
//    doc2.selectSheet("1");

//    qDebug()<<"start merge";

//    long line=1;
//    //这里是 counts-1 的原因为：需要控制数量，这样可以避免对当前正在追加保存的文件进行操作
//    for (long i=0;i<filenames.size();i++) {
//        QXlsx::Document doc(filenames[i]);
//        doc.selectSheet(QString::number(doc.sheetNames().size()));
//        qDebug()<<"current_file:"<<filenames[i];
//        QXlsx::CellRange range=doc.dimension();
//        long rownums=range.rowCount();
//        long colnums=range.columnCount();
//        long row=(i==0?1:2);
//        for (;row<=rownums;row++) {
//            for (int col=1;col<=colnums;col++) {
//                doc2.write(line,col,doc.read(row,col));
//            }
//            line++;
//        }
//        qDebug()<<"Write "<<filenames[i]<<" finished.";
//    }
//    doc2.save();
//    qDebug()<<"save finish";
}

