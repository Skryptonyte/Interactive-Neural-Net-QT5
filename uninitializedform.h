#ifndef UNINITIALIZEDFORM_H
#define UNINITIALIZEDFORM_H

#include <QDialog>

namespace Ui {
class uninitializedForm;
}

class uninitializedForm : public QDialog
{
    Q_OBJECT

public:
    explicit uninitializedForm(QWidget *parent = nullptr);
    ~uninitializedForm();

private:
    Ui::uninitializedForm *ui;
};

#endif // UNINITIALIZEDFORM_H
