#ifndef EXAMPLEEDITTAB_HPP
#define EXAMPLEEDITTAB_HPP

#include <QWidget>

namespace Ui {
    class exampleEditTab;
    }

class exampleEditTab : public QWidget
    {
    Q_OBJECT

public:
    explicit exampleEditTab(QWidget *parent = 0);
    ~exampleEditTab();

private:
    Ui::exampleEditTab *ui;
    };

#endif // EXAMPLEEDITTAB_HPP
