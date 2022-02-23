#include "dbcontroller.h"
#include "libxl/libxl.h"

class dbController dbcontroller;

dbBaseObject::dbBaseObject(QObject *parent): QObject (parent)
{
}
void dbBaseObject::save(){
}

dbSingleChart::dbSingleChart(QMap<QString,QMap<QString,QVector<QString>>> &tp,QVector<QString> &time,QString company_type)
{
    this->map.swap(tp);
    this->time.swap(time);
    this->company_type=company_type;

    this->title_col=settings.company_col_title;
    this->use_dates=settings.company_usedate_names;
}

dbDoubleChart::dbDoubleChart(QMap<QString,QMap<QString,QVector<QString>>> &tp1,
                             QMap<QString,QMap<QString,QVector<QString>>> &tp2,
                             QVector<QVector<QString>> &time,
                             QString date,
                             QString compandy_type)
{
    QMap<QString,QMap<QString,QVector<QString>>> tps1;
    tps1.swap(tp1);
    QMap<QString,QMap<QString,QVector<QString>>> tps2;
    tps2.swap(tp2);
    this->map.append(tps1);
    this->map.append(tps2);
    this->time.swap(time);

    this->date=date;
    this->compandy_type=compandy_type;

    this->title_col=settings.company_col_title;
    this->use_dates=settings.company_usedate_names;
}

dbSingleTable::dbSingleTable(QVector<QString> &content,QVector<QString> &timestamp,QVector<uint> &id,
                             QVector<QString> &type,QVector<QString> &name,QVector<uint> &len,
                             QVector<bool> &ifsend,QString date)
{
    this->content.swap(content);

    this->timestamp.swap(timestamp);
    this->id.swap(id);
    this->type.swap(type);
    this->name.swap(name);
    this->len.swap(len);
    this->ifsend.swap(ifsend);
    this->date=date;
}

dbMergeFile::dbMergeFile(QString tempDir,QString targetDir)
{
    this->tempDir=tempDir;
    this->targetDir=targetDir;
//    this->savename=savename;
}


void createSingleChartXlsx(libxl::Sheet *sheet){
    if(sheet==nullptr){return;}
    sheet->writeStr(0,0, L"时间戳");
    int linenums=settings.company_usedate_names.size();
    int colnums=1;
    for(int i=0;i<linenums;i++){
        sheet->writeStr(0,colnums++,QString("温度_"+settings.company_usedate_names[i]).toStdWString().c_str());
    }
    for(int i=0;i<linenums;i++){
        sheet->writeStr(0,colnums++,QString("状态_"+settings.company_usedate_names[i]).toStdWString().c_str());
    }
    for(int i=0;i<linenums;i++){
        sheet->writeStr(0,colnums++,QString("上升时间_"+settings.company_usedate_names[i]).toStdWString().c_str());
    }
    sheet->writeStr(0,colnums++,QString("产品代码").toStdWString().c_str());
}

void createDoubleChartXlsx(libxl::Sheet *sheet){
    if(sheet==nullptr){return;}
    sheet->writeStr(0,0, L"时间戳");
    int linenums=settings.company_usedate_names.size();
    int colnums=1;
    for(int i=0;i<linenums;i++){
        sheet->writeStr(0,colnums++,QString("温度_"+settings.company_usedate_names[i]).toStdWString().c_str());
    }
    for(int i=0;i<linenums;i++){
        sheet->writeStr(0,colnums++,QString("状态_"+settings.company_usedate_names[i]).toStdWString().c_str());
    }
    for(int i=0;i<linenums-2;i++){
        sheet->writeStr(0,colnums++,QString("上升时间_"+settings.company_usedate_names[i]).toStdWString().c_str());
    }
    for(int i=0;i<linenums-2;i++){
        sheet->writeStr(0,colnums++,QString("高温稳定测试_"+settings.company_usedate_names[i]).toStdWString().c_str());
    }
    for(int i=0;i<linenums-2;i++){
        sheet->writeStr(0,colnums++,QString("准确度测试_"+settings.company_usedate_names[i]).toStdWString().c_str());
    }
    sheet->writeStr(0,colnums++,QString("产品代码").toStdWString().c_str());
}


void createTableXlsx(libxl::Sheet *sheet)
{
    if(sheet==nullptr){
        return;
    }
     sheet->writeStr(0,0, L"时间戳");
     sheet->writeStr(0,1, L"发送/接受");
     sheet->writeStr(0,2, L"公司名称");
     sheet->writeStr(0,3, L"帧种类");
     sheet->writeStr(0,4, L"帧ID");
     sheet->writeStr(0,5, L"数据长度");
     sheet->writeStr(0,6, L"数据内容");
}

/// 保存单个图标
/// \brief dbSingleChart::createXlsx
/// \param filename
/// \param page
///
void dbSingleChart::save(){

     QString current_date_time=getCurrentDateTime();
     QString filename=QCoreApplication::applicationDirPath()+"/savefiles/tempcharts/"+current_date_time+".xls";
     qDebug()<<"single chart start to write";
     int mx=INT_MAX;
     for (int j=0;j<use_dates.size();j++) {
         for(int k=0;k<title_col.size();k++){
             if(j>=use_dates.size()-2&&k>=title_col.size()-3){
                 continue;
             }
             mx=std::min(mx,map[use_dates[j]][title_col[k]].size());
         }
     }

     libxl::Book* book=xlCreateBook();
     book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");
     book->load(filename.toStdWString().c_str());
     libxl::Sheet* sheet=nullptr;
     if(!book->sheetCount()){
         sheet=book->addSheet(QString::number(0).toStdWString().c_str());
         createSingleChartXlsx(sheet);
     }else {
         sheet=book->getSheet(book->sheetCount()-1);
     }
     int base=sheet->lastRow();

     int title_size=title_col.size();
     int usedate_size=use_dates.size();

     for (long line=base,i=0;i<mx;i++) {
         int col=0;
         if(line>settings.xlsxMaxline){
             sheet=book->addSheet(QString::number(book->sheetCount()).toStdWString().c_str());
             createSingleChartXlsx(sheet);
             line=1;
         }
         sheet->writeStr(line, col++,time[i].toStdWString().c_str());
         for (int j=0;j<title_size;j++) {
             for (int k=0;k<usedate_size;k++) {
                 //排除几项不存储的内容
                 if(j>=title_size-2&&k>=usedate_size-3){
                     continue;
                 }
                 if(col<8){
                     sheet->writeNum(line, col++,map[use_dates[k]][title_col[j]][i].toDouble());
                 }else{
                     sheet->writeStr(line, col++,map[use_dates[k]][title_col[j]][i].toStdWString().c_str());
                 }
             }
         }
         sheet->writeStr(line, col++,company_type.toStdWString().c_str());
         line++;
     }
     book->save(filename.toStdWString().c_str());
     book->release();
     qDebug()<<"图标保存完成";
     emit finish();
}


/// 用于保存table
/// \brief dbSingleTable::createXlsx
/// \param filename
/// \param page
///
void dbSingleTable::save(){

    QString current_date_time=getCurrentDateTime();
    QString filename=QCoreApplication::applicationDirPath()+"/savefiles/tempframes/"+current_date_time+".xls";
    qDebug()<<"table 开始写入";

    libxl::Book* book=xlCreateBook();
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");
    book->load(filename.toStdWString().c_str());
    libxl::Sheet* sheet=nullptr;
    if(!book->sheetCount()){
        sheet=book->addSheet(L"0");
        createTableXlsx(sheet);
    }else {
        sheet=book->getSheet(book->sheetCount()-1);
    }
    int size=timestamp.length();
    int base=sheet->lastRow();

    for (long i=0,line=base;i<size;i++,line++) {
        int col=0;
        if(line>settings.xlsxMaxline){
            sheet=book->addSheet(QString::number(book->sheetCount()).toStdWString().c_str());
            createTableXlsx(sheet);
            line=1;
        }
        sheet->writeStr(line,col++, timestamp[i].toStdWString().c_str());
        if(ifsend[i]){
            sheet->writeStr(line,col++, L"发送帧");
        }
        else{
            sheet->writeStr(line,col++, L"接受帧");
        }
        sheet->writeStr(line,col++, name[i].toStdWString().c_str());
        sheet->writeStr(line,col++, type[i].toStdWString().c_str());
        sheet->writeStr(line,col++, QString::number(id[i],16).toStdWString().c_str());
        sheet->writeStr(line,col++, QString::number(len[i],16).toStdWString().c_str());
        sheet->writeStr(line,col++, content[i].toStdWString().c_str());

    }

    book->save(filename.toStdWString().c_str());
    book->release();

    qDebug()<<"表格保存完成.";
    emit finish();
}

/// 双通道保存
/// \brief dbDoubleChart::createXlsx
/// \param filename
/// \param page
///
void dbDoubleChart::save(){

    QString current_date_time=getCurrentDateTime();
    QString filename[2]={
            QCoreApplication::applicationDirPath()+"/savefiles/temptestsChannel1/"+current_date_time+".xls",
            QCoreApplication::applicationDirPath()+"/savefiles/temptestsChannel2/"+current_date_time+".xls"
    };


    libxl::Book* book=xlCreateBook();
    book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");


    for(int mpidx=0;mpidx<map.size();mpidx++)
    {
        int mx=INT_MAX;

        for (int j=0;j<use_dates.size();j++) {
            for(int k=0;k<title_col.size();k++){
                if(j>=use_dates.size()-2&&k>=title_col.size()-3){
                    continue;
                }
                mx=std::min(mx,map[mpidx][use_dates[j]][title_col[k]].size());
            }
        }
        qDebug()<<"chart 开始写入"<<"  mx:"<<mx;

        libxl::Sheet* sheet=nullptr;
        book->load(filename[mpidx].toStdWString().c_str());
        if(book->sheetCount()==0){
            sheet=book->addSheet(L"0");
            createDoubleChartXlsx(sheet);
        }else{
            sheet=book->getSheet(book->sheetCount()-1);
        }

        int base=sheet->lastRow();
        int title_size=title_col.size();
        int usedate_size=use_dates.size();
        int numcols=settings.company_usedate_names.size()+1;
        for (long line=base,i=0;i<mx;i++) {
            int col=0;
            if(line>settings.xlsxMaxline){
               sheet=book->addSheet(QString::number(book->sheetCount()).toStdWString().c_str());
               createDoubleChartXlsx(sheet);
               line=1;
            }
            sheet->writeStr(line, col++,time[mpidx][i].toStdWString().c_str());
            for (int j=0;j<title_size;j++) {
                for (int k=0;k<usedate_size;k++) {
                    //排除几项不存储的内容
                    if(j>=title_size-2&&k>=usedate_size-3){
                        continue;
                    }
                    if(col<numcols){
                        sheet->writeNum(line, col++,map[mpidx][use_dates[k]][title_col[j]][i].toDouble());
                    }else{
                        sheet->writeStr(line, col++,map[mpidx][use_dates[k]][title_col[j]][i].toStdWString().c_str());
                    }
                }
            }
            sheet->writeStr(line, col++,(compandy_type+QString::number(mpidx+1)).toStdWString().c_str());
            line++;
         }
        book->save(filename[mpidx].toStdWString().c_str());
    }
    book->release();
    qDebug()<<"图表保存完成.";
    emit finish();
}


/// 聚合分散的文件
/// \brief dbMergeFile::save
///


void dbMergeFile::save(){
    //4个为一组融合  4个已经保存好了，2个为.与.. ,1个为当前正写入的excel
    QString basename=targetDir+getCurrentDate();
    QString filename=basename;
    int j=2;
    while(QFile::exists(filename+".xls")){
        filename=basename+"_"+QString::number(j);
        j++;
    }

    QDir dir(tempDir);
    for (int i=0;i<dir.count();i++) {
        qDebug()<<dir[i];
    }
    if(dir.count()<(settings.mergesize+2+1)){
        qDebug()<<"file not enough";
        emit finish();
        return;
    }

    //查看路径中后缀为.cfg格式的文件
    QStringList filters;
    filters<<QString("*.xls");
    //设置类型过滤器，只为文件格式
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    //设置文件名称过滤器，只为filters格式
    dir.setNameFilters(filters);

    QStringList filenames;
    for (uint i=0;i<dir.count();i++) {
        filenames<<dir[i].split(".")[0];
    }
    std::sort(filenames.begin(), filenames.end(), [](const QString& s1, const QString& s2){
                return  string2datetimeSeconds(s1)< string2datetimeSeconds(s2); });

    if(filenames.size()==0){
        return;
    }

    //创建新xls

    libxl::Book* book2 = xlCreateBook();
    book2->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");
    book2->load((filename+".xls").toStdWString().c_str());
    libxl::Sheet* sheet2=nullptr;
    qDebug()<<"target dir:"<<filename+".xls";
    qDebug()<<"start merge";

    int numcols=settings.company_usedate_names.size()+1;
    long line=settings.xlsxMaxline+1;

    for (long i=0;i<settings.mergesize;i++) {
        libxl::Book* book = xlCreateBook();
        book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");
        book->load((tempDir+filenames[i]+".xls").toStdWString().c_str());
        qDebug()<<"current_file:"<<tempDir+filenames[i]+".xls";

        for(int page=0;page<book->sheetCount();page++){
            libxl::Sheet* sheet=book->getSheet(page);
            qDebug()<<"current_page:"<<page;
            long rownums=sheet->lastRow();
            long colnums=sheet->lastCol();
            //从1开始，不写入头
            for (long row=1;row<rownums;row++) {
                if(line>settings.xlsxMaxline){
                    sheet2=book2->addSheet(QString::number(book2->sheetCount()).toStdWString().c_str());
                    for (int col=0;col<colnums;col++) {
                        sheet2->writeStr(0,col,sheet->readStr(0,col));
                    }
                    line=1;
                }
                for (int col=0;col<colnums;col++) {
                    if(col>0&&col<numcols)
                        sheet2->writeNum(line,col,sheet->readNum(row,col));
                    else
                        sheet2->writeStr(line,col,sheet->readStr(row,col));
                }
                line++;
            }
            qDebug()<<"page finish!";
        }
        qDebug()<<"Write "<<filenames[i]<<" finished.";
        book->release();
    }
    book2->save((filename+".xls").toStdWString().c_str());
    book2->release();
    for(int i=0;i<settings.mergesize;i++){
       dir.remove(tempDir+filenames[i]+".xls");
    }
    qDebug()<<"Merge Finish";
    emit finish();
}

dbController::dbController(QObject *parent) : QObject(parent)
{
    this->moveToThread(&thread);
    thread.start();
    ifruning=false;
}

dbController::~dbController(){
    thread.quit();
}

//原则：功能分离，不要加入功能中添加写入功能
void dbController::addObject(dbBaseObject* obj){
    qDebug()<<"add before size:"<<TaskQueue.size();
    obj->moveToThread(&thread);
    connect(obj,&dbBaseObject::finish,this,&dbController::writeFinish);
    TaskQueue.push_back(obj);
    qDebug()<<"add after size:"<<TaskQueue.size();
}

void dbController::writeFinish(){
    qDebug()<<"finish before size:"<<TaskQueue.size();
    delete TaskQueue.head();
    TaskQueue.pop_front();
    ifruning=false;
    qDebug()<<"finish after size:"<<TaskQueue.size();
}

void dbController::CheckWrite(){
    if(ifruning==false&&!TaskQueue.isEmpty()){
        ifruning=true;
        TaskQueue.head()->save();
    }
}
