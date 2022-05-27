#ifndef MULTIPLEUNION_H
#define MULTIPLEUNION_H

#include <QWidget>

namespace Ui {
class MultipleUnion;
}

class MultipleUnion : public QWidget
{
    Q_OBJECT

public:
    explicit MultipleUnion(QWidget *parent = nullptr);
    ~MultipleUnion();

private:
    Ui::MultipleUnion *ui;
};

#endif // MULTIPLEUNION_H
